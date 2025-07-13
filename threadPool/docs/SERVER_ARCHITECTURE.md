# 服务器端架构说明

## 总体架构

ThreadPool服务器采用经典的主从架构模式，由一个主线程和多个工作线程组成：

```
┌─────────────────────────────────────────────────────────────┐
│                        主进程                                │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
│  │ Signal      │  │ Epoll       │  │ Task Queue         │  │
│  │ Handler     │  │ Event Loop  │  │ Management         │  │
│  └─────────────┘  └─────────────┘  └─────────────────────┘  │
├─────────────────────────────────────────────────────────────┤
│                      Worker Threads                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
│  │ Worker 1    │  │ Worker 2    │  │ Worker N           │  │
│  │ File        │  │ File        │  │ File               │  │
│  │ Transfer    │  │ Transfer    │  │ Transfer           │  │
│  └─────────────┘  └─────────────┘  └─────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

## 核心组件详解

### 1. 主线程 (main.c)

**职责：**
- 初始化TCP监听套接字
- 创建和管理线程池
- 运行epoll事件循环
- 处理新客户端连接
- 响应退出信号

**关键流程：**
```c
int main(int argc, char *argv[]) {
    // 1. 参数检查和初始化
    ThreadPool_t threadPool;
    ThreadPoolInit(&threadPool, workerNum);
    
    // 2. 创建退出管道和信号处理
    pipe(threadPool.exitPipe);
    signal(SIGUSR1, handler);
    signal(SIGINT, handler);
    
    // 3. 创建工作线程
    makeWorker(&threadPool);
    
    // 4. 初始化网络和epoll
    int sockfd = initTcp(ip, port);
    int epfd = epoll_create(1);
    epollAdd(epfd, sockfd);
    epollAdd(epfd, threadPool.exitPipe[0]);
    
    // 5. 主事件循环
    while (1) {
        int readyNum = epoll_wait(epfd, readySet, 1024, -1);
        // 处理就绪事件...
    }
}
```

### 2. 线程池管理 (ThreadPool.c)

**数据结构：**
```c
typedef struct ThreadPool_s {
    tidArr_t tidArr;        // 线程ID数组管理
    taskQueue_t taskQueue;  // 任务队列
    pthread_mutex_t mutex;  // 保护共享资源的互斥锁
    pthread_cond_t empty;   // 队列为空时的条件变量
    pthread_cond_t full;    // 队列满时的条件变量
    int exitFlag;           // 优雅退出标志
    int exitPipe[2];        // 退出通知管道
} ThreadPool_t;
```

**初始化流程：**
1. 初始化线程ID数组
2. 初始化任务队列
3. 创建互斥锁和条件变量
4. 设置退出标志为0

### 3. 工作线程 (worker.c)

**线程生命周期：**
```c
void* worker(void* arg) {
    ThreadPool_t* threadPool = (ThreadPool_t*)arg;
    
    while (1) {
        pthread_mutex_lock(&threadPool->mutex);
        
        // 检查退出条件
        if (threadPool->exitFlag) {
            pthread_mutex_unlock(&threadPool->mutex);
            pthread_exit(NULL);
        }
        
        // 等待任务
        while (threadPool->taskQueue.queueSize == 0 && !threadPool->exitFlag) {
            pthread_cond_wait(&threadPool->empty, &threadPool->mutex);
        }
        
        // 再次检查退出条件
        if (threadPool->exitFlag) {
            pthread_mutex_unlock(&threadPool->mutex);
            pthread_exit(NULL);
        }
        
        // 获取任务并处理
        int netfd = threadPool->taskQueue.front->netfd;
        deQueue(&threadPool->taskQueue);
        pthread_mutex_unlock(&threadPool->mutex);
        
        // 处理文件传输
        transFile(netfd);
    }
}
```

**并发控制机制：**
- **互斥锁**：保护任务队列的并发访问
- **条件变量**：实现高效的线程等待/唤醒
- **原子操作**：确保退出标志的线程安全访问

### 4. 任务队列 (taskQueue.c)

**队列结构：**
```c
typedef struct node_s {
    int netfd;              // 客户端套接字
    struct node_s* next;    // 指向下一个节点
} node_t;

typedef struct taskQueue_s {
    node_t* front;      // 队首指针
    node_t* rear;       // 队尾指针
    int queueSize;      // 当前队列大小
} taskQueue_t;
```

**操作复杂度：**
- 入队 (enQueue): O(1)
- 出队 (deQueue): O(1)
- 查询大小: O(1)

### 5. 文件传输模块 (transFile.c)

**传输流程：**
```c
int transFile(int netfd) {
    // 1. 接收文件名请求
    recv(netfd, &train.len, sizeof(train.len), 0);
    recv(netfd, train.data, train.len, 0);
    
    // 2. 打开文件并获取信息
    int fd = open(filename, O_RDONLY);
    struct stat statbuf;
    fstat(fd, &statbuf);
    off_t filesize = statbuf.st_size;
    
    // 3. 发送文件大小
    send(netfd, &filesize_packet, sizeof(filesize_packet), MSG_NOSIGNAL);
    
    // 4. 内存映射文件 (零拷贝)
    char* p = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    
    // 5. 分块传输文件内容
    off_t offset = 0;
    while(offset < filesize) {
        int chunk_size = min(1000, filesize - offset);
        send(netfd, &chunk_size, sizeof(chunk_size), MSG_NOSIGNAL);
        send(netfd, p + offset, chunk_size, MSG_NOSIGNAL);
        offset += chunk_size;
    }
    
    // 6. 发送结束标志
    int end_flag = 0;
    send(netfd, &end_flag, sizeof(end_flag), MSG_NOSIGNAL);
    
    // 7. 清理资源
    munmap(p, filesize);
    close(fd);
    close(netfd);
}
```

**零拷贝优化：**
- 使用 `mmap()` 将文件映射到内存
- 直接从内核缓冲区发送数据
- 避免用户态与内核态的数据复制

### 6. 网络事件处理 (epoll.c)

**Epoll优势：**
- O(1)时间复杂度的事件通知
- 支持大量并发连接
- 边缘触发和水平触发模式

**事件类型处理：**
```c
// 监听套接字就绪 - 新连接
if (readySet[i].data.fd == sockfd) {
    int netfd = accept(sockfd, NULL, NULL);
    // 将任务加入队列
    enQueue(&threadPool.taskQueue, netfd);
    pthread_cond_signal(&threadPool.empty);
}

// 退出管道就绪 - 优雅退出
else if (readySet[i].data.fd == threadPool.exitPipe[0]) {
    // 设置退出标志，通知所有工作线程
    threadPool.exitFlag = 1;
    pthread_cond_broadcast(&threadPool.empty);
}
```

## 优雅退出机制

### 退出流程设计

```
Signal (SIGUSR1/SIGINT) 
        ↓
Signal Handler 
        ↓
Write to Exit Pipe 
        ↓
Epoll Detects Pipe Event 
        ↓
Set Exit Flag 
        ↓
Broadcast to All Workers 
        ↓
Workers Check Flag & Exit 
        ↓
Main Thread Joins All Threads 
        ↓
Clean Resources & Exit
```

### 关键实现

**信号处理器：**
```c
void handler(int signum) {
    printf("Received signal %d, initiating graceful shutdown...\\n", signum);
    if (globalThreadPool) {
        globalThreadPool->exitFlag = 1;
        write(globalThreadPool->exitPipe[1], "1", 1);
    }
}
```

**工作线程退出检查：**
```c
// 双重检查确保及时响应退出信号
if (threadPool->exitFlag) {
    pthread_mutex_unlock(&threadPool->mutex);
    printf("Worker thread %lu exiting gracefully\\n", pthread_self());
    pthread_exit(NULL);
}
```

## 性能优化策略

### 1. 内存管理
- **零拷贝传输**：使用mmap减少内存复制
- **预分配缓冲区**：避免频繁的内存分配/释放
- **及时释放**：连接处理完毕立即清理资源

### 2. 并发优化
- **细粒度锁**：最小化锁的持有时间
- **条件变量**：避免忙等待，提高CPU利用率
- **线程池**：复用线程，避免创建销毁开销

### 3. 网络优化
- **Epoll模型**：高效的I/O多路复用
- **MSG_NOSIGNAL**：避免SIGPIPE信号中断
- **TCP优化**：设置SO_REUSEADDR等套接字选项

### 4. 错误处理
- **完善的错误检查**：每个系统调用都有错误处理
- **资源清理**：异常情况下的资源释放
- **日志记录**：便于问题诊断和调试

## 扩展性考虑

### 水平扩展
- 支持多服务器实例部署
- 负载均衡器分发请求
- 共享存储文件系统

### 垂直扩展
- 动态调整线程池大小
- 自适应缓冲区大小
- CPU亲和性优化

### 功能扩展
- 文件权限控制
- 传输加密支持
- 断点续传功能
- 压缩传输选项
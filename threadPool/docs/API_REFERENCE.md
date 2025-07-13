# API 文档与代码示例

## 概述

本文档详细描述了ThreadPool项目中所有公开API的使用方法、参数说明和代码示例。

## 服务器端 API

### ThreadPool 管理 API

#### ThreadPoolInit()
```c
int ThreadPoolInit(ThreadPool_t* threadPool, int workerNum);
```

**功能：** 初始化线程池结构体

**参数：**
- `threadPool` - 线程池结构体指针
- `workerNum` - 工作线程数量

**返回值：**
- `0` - 成功
- `-1` - 失败

**示例：**
```c
#include "ThreadPool.h"

int main() {
    ThreadPool_t threadPool;
    int workerNum = 4;
    
    int ret = ThreadPoolInit(&threadPool, workerNum);
    if (ret != 0) {
        fprintf(stderr, "Failed to initialize thread pool\n");
        return -1;
    }
    
    printf("Thread pool initialized with %d workers\n", workerNum);
    return 0;
}
```

#### makeWorker()
```c
int makeWorker(ThreadPool_t *threadPool);
```

**功能：** 创建工作线程

**参数：**
- `threadPool` - 已初始化的线程池指针

**返回值：**
- `0` - 成功创建所有工作线程
- `-1` - 创建失败

**示例：**
```c
ThreadPool_t threadPool;
ThreadPoolInit(&threadPool, 3);

// 创建工作线程
if (makeWorker(&threadPool) != 0) {
    fprintf(stderr, "Failed to create worker threads\n");
    return -1;
}

printf("All worker threads created successfully\n");
```

### 任务队列 API

#### taskQueueInit()
```c
int taskQueueInit(taskQueue_t* taskQueue);
```

**功能：** 初始化任务队列

**参数：**
- `taskQueue` - 任务队列结构体指针

**返回值：**
- `0` - 成功

**示例：**
```c
#include "taskQueue.h"

taskQueue_t queue;
taskQueueInit(&queue);

printf("Task queue initialized\n");
printf("Queue size: %d\n", queue.queueSize);
```

#### enQueue()
```c
int enQueue(taskQueue_t* taskQueue, int netfd);
```

**功能：** 向任务队列添加新任务

**参数：**
- `taskQueue` - 任务队列指针
- `netfd` - 客户端网络文件描述符

**返回值：**
- `0` - 成功入队

**示例：**
```c
taskQueue_t queue;
taskQueueInit(&queue);

// 模拟客户端连接
int client_fd = accept(server_fd, NULL, NULL);
if (client_fd > 0) {
    enQueue(&queue, client_fd);
    printf("Added client fd %d to queue\n", client_fd);
    printf("Current queue size: %d\n", queue.queueSize);
}
```

#### deQueue()
```c
int deQueue(taskQueue_t *taskQueue);
```

**功能：** 从任务队列移除任务

**参数：**
- `taskQueue` - 任务队列指针

**返回值：**
- `0` - 成功出队

**注意：** 此函数只负责出队操作，需要在调用前手动获取队首元素的netfd。

**示例：**
```c
taskQueue_t queue;
// ... 队列已初始化并有数据

if (queue.queueSize > 0) {
    int netfd = queue.front->netfd;  // 获取文件描述符
    deQueue(&queue);                 // 出队操作
    printf("Dequeued client fd: %d\n", netfd);
    
    // 处理客户端请求
    transFile(netfd);
}
```

### 网络通信 API

#### initTcp()
```c
int initTcp(char *ip, char* port);
```

**功能：** 初始化TCP服务器套接字

**参数：**
- `ip` - 绑定IP地址
- `port` - 监听端口

**返回值：**
- 套接字文件描述符 (成功)
- `-1` - 失败

**示例：**
```c
#include <my_header.h>

int main() {
    char *ip = "127.0.0.1";
    char *port = "8888";
    
    int server_fd = initTcp(ip, port);
    if (server_fd == -1) {
        perror("Failed to initialize TCP server");
        return -1;
    }
    
    printf("TCP server initialized on %s:%s\n", ip, port);
    printf("Server socket fd: %d\n", server_fd);
    
    // 开始监听客户端连接
    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd > 0) {
            printf("New client connected: fd=%d\n", client_fd);
            // 处理客户端...
            close(client_fd);
        }
    }
    
    close(server_fd);
    return 0;
}
```

#### epollAdd() / epollDel()
```c
int epollAdd(int epfd, int fd);
int epollDel(int epfd, int fd);
```

**功能：** 向epoll实例添加/删除文件描述符

**参数：**
- `epfd` - epoll文件描述符
- `fd` - 要添加/删除的文件描述符

**返回值：**
- `0` - 成功

**示例：**
```c
#include <sys/epoll.h>

int main() {
    // 创建epoll实例
    int epfd = epoll_create(1);
    if (epfd == -1) {
        perror("epoll_create");
        return -1;
    }
    
    // 创建服务器套接字
    int server_fd = initTcp("127.0.0.1", "8888");
    
    // 添加服务器套接字到epoll
    if (epollAdd(epfd, server_fd) != 0) {
        fprintf(stderr, "Failed to add server fd to epoll\n");
        return -1;
    }
    
    struct epoll_event events[10];
    
    while (1) {
        int nfds = epoll_wait(epfd, events, 10, -1);
        
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // 新客户端连接
                int client_fd = accept(server_fd, NULL, NULL);
                epollAdd(epfd, client_fd);
                printf("Added client fd %d to epoll\n", client_fd);
            } else {
                // 客户端数据就绪
                int client_fd = events[i].data.fd;
                printf("Data ready on client fd %d\n", client_fd);
                
                // 处理完毕后删除
                epollDel(epfd, client_fd);
                close(client_fd);
            }
        }
    }
    
    close(epfd);
    return 0;
}
```

### 文件传输 API

#### transFile()
```c
int transFile(int netfd);
```

**功能：** 处理文件传输请求

**参数：**
- `netfd` - 客户端网络连接文件描述符

**返回值：**
- `0` - 传输成功
- `-1` - 传输失败

**示例：**
```c
// 在工作线程中使用
void* worker(void* arg) {
    ThreadPool_t* threadPool = (ThreadPool_t*)arg;
    
    while (1) {
        pthread_mutex_lock(&threadPool->mutex);
        
        // 等待任务
        while (threadPool->taskQueue.queueSize == 0 && !threadPool->exitFlag) {
            pthread_cond_wait(&threadPool->empty, &threadPool->mutex);
        }
        
        if (threadPool->exitFlag) {
            pthread_mutex_unlock(&threadPool->mutex);
            break;
        }
        
        // 获取任务
        int netfd = threadPool->taskQueue.front->netfd;
        deQueue(&threadPool->taskQueue);
        pthread_mutex_unlock(&threadPool->mutex);
        
        // 处理文件传输
        printf("Worker %lu processing client fd %d\n", pthread_self(), netfd);
        
        int result = transFile(netfd);
        if (result == 0) {
            printf("File transfer completed successfully\n");
        } else {
            printf("File transfer failed\n");
        }
    }
    
    return NULL;
}
```

## 客户端 API

### 网络通信函数

#### recvn()
```c
int recvn(int sockfd, void *buf, int len);
```

**功能：** 可靠接收指定长度的数据

**参数：**
- `sockfd` - 套接字文件描述符
- `buf` - 接收缓冲区
- `len` - 要接收的字节数

**返回值：**
- 实际接收的字节数
- `<=0` - 连接关闭或错误

**示例：**
```c
#include <my_header.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 连接到服务器...
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8888);
    
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        perror("connect");
        return -1;
    }
    
    // 接收固定长度的数据
    char buffer[1024];
    int bytes_received = recvn(sockfd, buffer, sizeof(buffer));
    
    if (bytes_received > 0) {
        printf("Received %d bytes: %.*s\n", bytes_received, bytes_received, buffer);
    } else if (bytes_received == 0) {
        printf("Connection closed by server\n");
    } else {
        perror("recvn failed");
    }
    
    close(sockfd);
    return 0;
}
```

### 文件下载函数

#### sendFile()
```c
int sendFile(int netfd, char* filename);
```

**功能：** 从服务器下载指定文件

**参数：**
- `netfd` - 服务器连接文件描述符
- `filename` - 要下载的文件名

**返回值：**
- `0` - 下载成功
- `-1` - 下载失败

**示例：**
```c
#include <my_header.h>

int download_file_example(const char* server_ip, int port, const char* filename) {
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }
    
    // 连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(server_ip);
    addr.sin_port = htons(port);
    
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        perror("connect");
        close(sockfd);
        return -1;
    }
    
    printf("Connected to %s:%d\n", server_ip, port);
    
    // 下载文件
    int result = sendFile(sockfd, (char*)filename);
    
    if (result == 0) {
        printf("File '%s' downloaded successfully\n", filename);
    } else {
        printf("Failed to download file '%s'\n", filename);
    }
    
    close(sockfd);
    return result;
}

int main() {
    // 示例用法
    return download_file_example("127.0.0.1", 8888, "test.txt");
}
```

## 完整示例程序

### 简化版服务器
```c
// simple_server.c
#include "ThreadPool.h"

ThreadPool_t *globalThreadPool = NULL;

void signal_handler(int signum) {
    printf("Received signal %d, shutting down...\n", signum);
    if (globalThreadPool) {
        globalThreadPool->exitFlag = 1;
        write(globalThreadPool->exitPipe[1], "1", 1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip> <port> <workers>\n", argv[0]);
        return 1;
    }
    
    // 初始化线程池
    ThreadPool_t threadPool;
    globalThreadPool = &threadPool;
    
    ThreadPoolInit(&threadPool, atoi(argv[3]));
    pipe(threadPool.exitPipe);
    
    // 注册信号处理
    signal(SIGUSR1, signal_handler);
    signal(SIGINT, signal_handler);
    
    // 创建工作线程
    makeWorker(&threadPool);
    
    // 初始化网络
    int server_fd = initTcp(argv[1], argv[2]);
    int epfd = epoll_create(1);
    
    epollAdd(epfd, server_fd);
    epollAdd(epfd, threadPool.exitPipe[0]);
    
    printf("Server started on %s:%s with %s workers\n", 
           argv[1], argv[2], argv[3]);
    
    // 主事件循环
    struct epoll_event events[64];
    
    while (1) {
        int nfds = epoll_wait(epfd, events, 64, -1);
        
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // 新客户端连接
                int client_fd = accept(server_fd, NULL, NULL);
                printf("New client: fd=%d\n", client_fd);
                
                // 添加到任务队列
                pthread_mutex_lock(&threadPool.mutex);
                enQueue(&threadPool.taskQueue, client_fd);
                pthread_cond_signal(&threadPool.empty);
                pthread_mutex_unlock(&threadPool.mutex);
                
            } else if (events[i].data.fd == threadPool.exitPipe[0]) {
                // 退出信号
                printf("Graceful shutdown initiated\n");
                
                // 等待所有线程结束
                pthread_mutex_lock(&threadPool.mutex);
                threadPool.exitFlag = 1;
                pthread_cond_broadcast(&threadPool.empty);
                pthread_mutex_unlock(&threadPool.mutex);
                
                for (int j = 0; j < threadPool.tidArr.tidArrSize; j++) {
                    pthread_join(threadPool.tidArr.tidArr[j], NULL);
                }
                
                printf("All threads joined, exiting\n");
                goto cleanup;
            }
        }
    }
    
cleanup:
    close(server_fd);
    close(epfd);
    close(threadPool.exitPipe[0]);
    close(threadPool.exitPipe[1]);
    
    return 0;
}
```

### 简化版客户端
```c
// simple_client.c
#include <my_header.h>

typedef struct {
    int len;
    char data[1000];
} train_t;

int simple_download(const char* server_ip, int port, const char* filename) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(server_ip);
    addr.sin_port = htons(port);
    
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        perror("connect");
        return -1;
    }
    
    printf("Connected to %s:%d\n", server_ip, port);
    
    // 发送文件名请求
    train_t train;
    train.len = strlen(filename);
    send(sockfd, &train.len, sizeof(train.len), 0);
    send(sockfd, filename, train.len, 0);
    
    printf("Requesting file: %s\n", filename);
    
    // 接收文件大小
    if (recv(sockfd, &train.len, sizeof(train.len), 0) <= 0) {
        printf("Connection error\n");
        close(sockfd);
        return -1;
    }
    
    if (train.len == 0) {
        printf("File not found\n");
        close(sockfd);
        return -1;
    }
    
    off_t filesize;
    recv(sockfd, &filesize, train.len, 0);
    printf("File size: %ld bytes\n", filesize);
    
    // 创建本地文件
    char local_name[256];
    snprintf(local_name, sizeof(local_name), "downloaded_%s", filename);
    int fd = open(local_name, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    
    // 接收文件内容
    off_t received = 0;
    while (1) {
        if (recv(sockfd, &train.len, sizeof(train.len), 0) <= 0) break;
        if (train.len == 0) break;  // 传输结束
        
        recv(sockfd, train.data, train.len, 0);
        write(fd, train.data, train.len);
        received += train.len;
        
        printf("Progress: %.1f%%\r", 
               100.0 * received / filesize);
        fflush(stdout);
    }
    
    printf("\nDownload completed: %s\n", local_name);
    
    close(fd);
    close(sockfd);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server_ip> <port> <filename>\n", argv[0]);
        return 1;
    }
    
    return simple_download(argv[1], atoi(argv[2]), argv[3]);
}
```

## 编译和测试

### 编译示例程序
```bash
# 编译简化服务器
gcc simple_server.c -o simple_server -lpthread -I../server

# 编译简化客户端  
gcc simple_client.c -o simple_client
```

### 测试流程
```bash
# 1. 启动服务器
./simple_server 127.0.0.1 8888 2 &
SERVER_PID=$!

# 2. 创建测试文件
echo "Hello, World!" > test.txt

# 3. 客户端下载
./simple_client 127.0.0.1 8888 test.txt

# 4. 验证结果
cat downloaded_test.txt

# 5. 停止服务器
kill -SIGUSR1 $SERVER_PID
```

这套API提供了完整的文件传输服务器和客户端功能，可以根据具体需求进行扩展和定制。
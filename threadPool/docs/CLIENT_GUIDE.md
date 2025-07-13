# 客户端使用指南

## 概述

ThreadPool客户端是一个简单高效的文件下载工具，支持从ThreadPool服务器下载文件，并提供实时进度显示和错误处理功能。

## 功能特性

- 🚀 **高速下载** - 基于小火车协议的可靠传输
- 📊 **进度显示** - 实时显示下载进度百分比
- 🛡️ **错误处理** - 完善的网络异常和文件错误处理
- 📁 **智能重命名** - 自动添加前缀避免文件名冲突
- 🔄 **断线检测** - 网络中断时及时报告错误

## 编译安装

### 系统要求
- Linux操作系统
- GCC编译器
- C标准库

### 编译步骤
```bash
cd threadPool/client
make clean      # 清理旧文件
make           # 编译客户端
```

编译成功后会生成 `client` 可执行文件。

## 基本用法

### 命令语法
```bash
./client <服务器IP> <端口> <文件名>
```

### 参数说明
- **服务器IP**: 目标服务器的IP地址
- **端口**: 服务器监听的端口号
- **文件名**: 要下载的文件名（服务器端路径）

### 使用示例

#### 1. 下载本地服务器文件
```bash
./client 127.0.0.1 8888 test.txt
```

#### 2. 下载远程服务器文件
```bash
./client 192.168.1.100 9999 document.pdf
```

#### 3. 下载大文件
```bash
./client 10.0.0.50 8888 video.mp4
```

## 输出信息解读

### 正常下载流程
```bash
$ ./client 127.0.0.1 8888 testfile.txt
Connected to server 127.0.0.1:8888
Requesting file: testfile.txt
File size: 1048576 bytes
Progress: 100.00%
File download completed: downloaded_testfile.txt
```

**输出说明：**
1. `Connected to server` - 成功连接到服务器
2. `Requesting file` - 发送文件请求
3. `File size` - 显示文件大小
4. `Progress` - 实时进度（每1%更新一次）
5. `File download completed` - 下载完成，显示本地文件名

### 错误处理信息

#### 连接失败
```bash
$ ./client 192.168.1.999 8888 test.txt
connect: No route to host
```

#### 文件不存在
```bash
$ ./client 127.0.0.1 8888 nonexistent.txt
Connected to server 127.0.0.1:8888
Requesting file: nonexistent.txt
File not found on server
```

#### 网络中断
```bash
$ ./client 127.0.0.1 8888 largefile.bin
Connected to server 127.0.0.1:8888
Requesting file: largefile.bin
File size: 10485760 bytes
Progress: 45.23%
Connection closed or error
```

## 协议交互流程

### 1. 连接建立
```
客户端                           服务器
   |                               |
   |------ TCP连接请求 ----------->|
   |<----- TCP连接确认 ------------|
   |                               |
```

### 2. 文件请求
```
客户端                           服务器
   |                               |
   |------ [长度][文件名] --------->|
   |                               |
   |<----- [长度][文件大小] --------|
   |                               |
```

### 3. 文件传输
```
客户端                           服务器
   |                               |
   |<----- [长度][数据块1] --------|
   |<----- [长度][数据块2] --------|
   |<----- [长度][数据块N] --------|
   |<----- [0][] (结束标志) -------|
   |                               |
```

## 高级用法

### 批量下载脚本
```bash
#!/bin/bash
# batch_download.sh

SERVER_IP="127.0.0.1"
SERVER_PORT="8888"

# 文件列表
files=(
    "document1.pdf"
    "image1.jpg" 
    "video1.mp4"
    "archive1.zip"
)

for file in "${files[@]}"; do
    echo "Downloading: $file"
    ./client $SERVER_IP $SERVER_PORT "$file"
    if [ $? -eq 0 ]; then
        echo "✓ Successfully downloaded: $file"
    else
        echo "✗ Failed to download: $file"
    fi
    echo "------------------------"
done
```

### 下载监控脚本
```bash
#!/bin/bash
# monitor_download.sh

LOG_FILE="download.log"
SERVER_IP="$1"
SERVER_PORT="$2"
FILE_NAME="$3"

if [ $# -ne 3 ]; then
    echo "Usage: $0 <server_ip> <port> <filename>"
    exit 1
fi

echo "[$(date)] Starting download: $FILE_NAME" >> $LOG_FILE

./client "$SERVER_IP" "$SERVER_PORT" "$FILE_NAME" 2>&1 | tee -a $LOG_FILE

if [ ${PIPESTATUS[0]} -eq 0 ]; then
    echo "[$(date)] Download completed successfully: $FILE_NAME" >> $LOG_FILE
else
    echo "[$(date)] Download failed: $FILE_NAME" >> $LOG_FILE
fi
```

## 性能优化建议

### 网络环境优化
1. **使用有线连接** - 比WiFi更稳定
2. **关闭其他网络应用** - 减少带宽竞争
3. **调整TCP缓冲区** - 系统级别优化

### 系统配置优化
```bash
# 临时增加TCP缓冲区大小
echo 'net.core.rmem_max = 16777216' >> /etc/sysctl.conf
echo 'net.core.wmem_max = 16777216' >> /etc/sysctl.conf
sysctl -p

# 增加文件描述符限制
ulimit -n 65536
```

### 并发下载
```bash
# 使用并行下载（注意服务器负载）
for i in {1..5}; do
    ./client 127.0.0.1 8888 "file$i.txt" &
done
wait  # 等待所有下载完成
```

## 故障排除

### 常见问题及解决方案

#### 1. 编译错误
```bash
# 错误：找不到头文件
$ make
gcc client.c -o client -g -Wall
client.c:1:10: fatal error: my_header.h: No such file or directory

# 解决：检查头文件路径
$ ls /usr/include/my_header.h
$ sudo find /usr -name "my_header.h"
```

#### 2. 运行时错误
```bash
# 错误：权限不足
$ ./client 127.0.0.1 8888 test.txt
open local file: Permission denied

# 解决：检查目录权限
$ ls -la ./
$ chmod 755 .
```

#### 3. 网络问题
```bash
# 错误：连接超时
$ ./client 192.168.1.100 8888 test.txt
connect: Connection timed out

# 解决：检查网络连通性
$ ping 192.168.1.100
$ telnet 192.168.1.100 8888
```

#### 4. 文件问题
```bash
# 错误：磁盘空间不足
$ ./client 127.0.0.1 8888 largefile.bin
write: No space left on device

# 解决：清理磁盘空间
$ df -h
$ du -sh ./*
```

### 调试技巧

#### 启用详细输出
```bash
# 方法1：使用strace跟踪系统调用
strace -e trace=network ./client 127.0.0.1 8888 test.txt

# 方法2：使用gdb调试
gdb ./client
(gdb) set args 127.0.0.1 8888 test.txt
(gdb) run
```

#### 网络分析
```bash
# 抓包分析通信过程
sudo tcpdump -i lo -w client_capture.pcap port 8888 &
./client 127.0.0.1 8888 test.txt
sudo pkill tcpdump

# 使用wireshark分析
wireshark client_capture.pcap
```

## 与服务器配合使用

### 服务器准备
1. **启动服务器**
   ```bash
   cd threadPool/server
   ./server 127.0.0.1 8888 3
   ```

2. **准备测试文件**
   ```bash
   # 创建不同大小的测试文件
   echo "Small file content" > small.txt
   dd if=/dev/urandom of=medium.bin bs=1M count=10
   dd if=/dev/urandom of=large.bin bs=1M count=100
   ```

### 测试流程
1. **基本功能测试**
   ```bash
   ./client 127.0.0.1 8888 small.txt
   ```

2. **大文件传输测试**
   ```bash
   ./client 127.0.0.1 8888 large.bin
   ```

3. **错误处理测试**
   ```bash
   ./client 127.0.0.1 8888 nonexistent.txt
   ```

4. **并发测试**
   ```bash
   for i in {1..10}; do
       ./client 127.0.0.1 8888 medium.bin &
   done
   ```

## 最佳实践

### 1. 生产环境使用
- 使用配置文件管理服务器信息
- 实现重试机制处理临时网络问题
- 添加日志记录功能便于问题追踪
- 设置合理的超时时间

### 2. 安全考虑
- 验证下载文件的完整性（MD5/SHA256）
- 限制下载文件大小避免磁盘占满
- 实现用户认证机制
- 使用HTTPS加密传输（扩展功能）

### 3. 性能优化
- 根据网络带宽调整缓冲区大小
- 实现多线程并发下载
- 添加断点续传功能
- 使用压缩减少传输数据量

## 扩展开发

### 功能扩展建议
1. **配置文件支持**
   ```c
   // config.h
   typedef struct {
       char server_ip[16];
       int server_port;
       int timeout_seconds;
       char download_dir[256];
   } client_config_t;
   ```

2. **进度回调函数**
   ```c
   typedef void (*progress_callback_t)(int percentage, size_t bytes_downloaded);
   int download_file_with_callback(const char* filename, progress_callback_t callback);
   ```

3. **异步下载接口**
   ```c
   typedef struct {
       int download_id;
       char filename[256];
       int status;  // 0: pending, 1: downloading, 2: completed, -1: error
   } download_task_t;
   
   int start_async_download(const char* filename);
   download_task_t* get_download_status(int download_id);
   ```

客户端设计简洁而功能完善，为用户提供了可靠的文件下载体验。
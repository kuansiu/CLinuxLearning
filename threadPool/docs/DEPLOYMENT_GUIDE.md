# 部署和使用指南

## 快速开始

### 1. 克隆和编译
```bash
# 进入项目目录
cd threadPool

# 编译服务器
cd server
make clean && make
cd ..

# 编译客户端
cd client  
make clean && make
cd ..
```

### 2. 基本测试
```bash
# 启动服务器（3个工作线程）
cd server
./server 127.0.0.1 8888 3 &

# 创建测试文件
echo "Hello ThreadPool!" > hello.txt

# 客户端下载测试
cd ../client
./client 127.0.0.1 8888 hello.txt

# 查看下载结果
cat downloaded_hello.txt
```

### 3. 优雅停止
```bash
# 获取服务器进程ID
ps aux | grep server

# 发送停止信号
kill -SIGUSR1 <PID>
```

## 生产环境部署

### 系统要求
- Linux Server (Ubuntu 18.04+ 或 CentOS 7+)
- 最小2GB RAM，推荐4GB+
- 至少1GB可用磁盘空间
- GCC 7.0+ 编译器

### 安装步骤

#### Ubuntu/Debian
```bash
# 更新系统
sudo apt update && sudo apt upgrade -y

# 安装依赖
sudo apt install build-essential git

# 编译项目
cd threadPool
make -C server clean && make -C server
make -C client clean && make -C client
```

#### CentOS/RHEL
```bash
# 安装依赖
sudo yum groupinstall "Development Tools"
sudo yum install git

# 编译项目  
cd threadPool
make -C server clean && make -C server
make -C client clean && make -C client
```

### 性能调优

#### 系统参数优化
```bash
# 增加文件描述符限制
echo "* soft nofile 65536" >> /etc/security/limits.conf
echo "* hard nofile 65536" >> /etc/security/limits.conf

# 优化TCP参数
echo "net.core.somaxconn = 1024" >> /etc/sysctl.conf
echo "net.core.netdev_max_backlog = 5000" >> /etc/sysctl.conf
echo "net.ipv4.tcp_max_syn_backlog = 1024" >> /etc/sysctl.conf
sysctl -p

# 重启生效
reboot
```

#### 线程数配置建议
```bash
# 查看CPU核心数
nproc

# 建议线程数配置：
# - CPU密集型：核心数 
# - I/O密集型：核心数 × 2
# - 混合负载：核心数 × 1.5

# 4核CPU示例
./server 0.0.0.0 8888 6
```

### 监控脚本

#### 服务器状态监控
```bash
#!/bin/bash
# monitor_server.sh

SERVER_NAME="threadpool_server"
LOG_FILE="/var/log/threadpool.log"

check_server() {
    PID=$(pgrep -f "$SERVER_NAME")
    if [ -z "$PID" ]; then
        echo "[$(date)] Server is not running" >> $LOG_FILE
        return 1
    else
        echo "[$(date)] Server is running (PID: $PID)" >> $LOG_FILE
        
        # 检查内存使用
        MEM=$(ps -p $PID -o %mem --no-headers)
        echo "[$(date)] Memory usage: ${MEM}%" >> $LOG_FILE
        
        # 检查CPU使用
        CPU=$(ps -p $PID -o %cpu --no-headers)
        echo "[$(date)] CPU usage: ${CPU}%" >> $LOG_FILE
        
        return 0
    fi
}

# 每分钟检查一次
while true; do
    check_server
    sleep 60
done
```

#### 自动重启脚本
```bash
#!/bin/bash
# auto_restart.sh

SERVER_DIR="/path/to/threadPool/server"
SERVER_CMD="./server 0.0.0.0 8888 4"
MAX_RESTARTS=5
RESTART_DELAY=10

restart_count=0

while [ $restart_count -lt $MAX_RESTARTS ]; do
    echo "[$(date)] Starting server (attempt $((restart_count + 1)))"
    
    cd $SERVER_DIR
    $SERVER_CMD &
    SERVER_PID=$!
    
    # 等待服务器进程结束
    wait $SERVER_PID
    EXIT_CODE=$?
    
    echo "[$(date)] Server exited with code $EXIT_CODE"
    
    if [ $EXIT_CODE -eq 0 ]; then
        echo "[$(date)] Server exited normally"
        break
    else
        restart_count=$((restart_count + 1))
        echo "[$(date)] Restarting in $RESTART_DELAY seconds..."
        sleep $RESTART_DELAY
    fi
done

if [ $restart_count -eq $MAX_RESTARTS ]; then
    echo "[$(date)] Max restart attempts reached. Giving up."
    exit 1
fi
```

### 服务化部署 (systemd)

#### 创建服务文件
```bash
# /etc/systemd/system/threadpool.service
[Unit]
Description=ThreadPool File Transfer Server
After=network.target

[Service]
Type=simple
User=threadpool
Group=threadpool
WorkingDirectory=/opt/threadpool/server
ExecStart=/opt/threadpool/server/server 0.0.0.0 8888 4
ExecStop=/bin/kill -SIGUSR1 $MAINPID
Restart=always
RestartSec=3
StandardOutput=journal
StandardError=journal

[Install]
WantedBy=multi-user.target
```

#### 部署和启动
```bash
# 创建用户
sudo useradd -r -s /bin/false threadpool

# 创建目录并复制文件
sudo mkdir -p /opt/threadpool
sudo cp -r threadPool/* /opt/threadpool/
sudo chown -R threadpool:threadpool /opt/threadpool

# 启用服务
sudo systemctl daemon-reload
sudo systemctl enable threadpool.service
sudo systemctl start threadpool.service

# 检查状态
sudo systemctl status threadpool.service
```

### 日志管理

#### 日志轮转配置
```bash
# /etc/logrotate.d/threadpool
/var/log/threadpool/*.log {
    daily
    missingok
    rotate 30
    compress
    delaycompress
    notifempty
    copytruncate
    postrotate
        systemctl reload threadpool.service > /dev/null 2>&1 || true
    endscript
}
```

### 安全配置

#### 防火墙设置
```bash
# Ubuntu (ufw)
sudo ufw allow 8888/tcp
sudo ufw enable

# CentOS (firewalld)
sudo firewall-cmd --permanent --add-port=8888/tcp
sudo firewall-cmd --reload
```

#### 限制访问
```bash
# 只允许特定IP访问
sudo iptables -A INPUT -p tcp --dport 8888 -s 192.168.1.0/24 -j ACCEPT
sudo iptables -A INPUT -p tcp --dport 8888 -j DROP

# 保存规则
sudo iptables-save > /etc/iptables/rules.v4
```

### 性能测试

#### 并发测试脚本
```bash
#!/bin/bash
# stress_test.sh

SERVER_IP="127.0.0.1"
SERVER_PORT="8888"
CLIENTS=10
TEST_FILE="test.txt"

# 创建测试文件
dd if=/dev/urandom of=$TEST_FILE bs=1M count=10

echo "Starting stress test with $CLIENTS concurrent clients"

# 启动多个客户端
for i in $(seq 1 $CLIENTS); do
    echo "Starting client $i"
    (
        cd client
        time ./client $SERVER_IP $SERVER_PORT $TEST_FILE > client_$i.log 2>&1
        echo "Client $i completed"
    ) &
done

# 等待所有客户端完成
wait

echo "Stress test completed"

# 统计结果
echo "Results:"
for i in $(seq 1 $CLIENTS); do
    if [ -f "client/downloaded_$TEST_FILE" ]; then
        echo "Client $i: SUCCESS"
    else
        echo "Client $i: FAILED"
    fi
done
```

#### 性能监控
```bash
#!/bin/bash
# performance_monitor.sh

PID=$(pgrep -f "server")
if [ -z "$PID" ]; then
    echo "Server not running"
    exit 1
fi

echo "Monitoring server performance (PID: $PID)"
echo "Press Ctrl+C to stop"

while true; do
    CPU=$(ps -p $PID -o %cpu --no-headers | tr -d ' ')
    MEM=$(ps -p $PID -o %mem --no-headers | tr -d ' ')
    RSS=$(ps -p $PID -o rss --no-headers | tr -d ' ')
    
    CONNECTIONS=$(ss -t state established | grep :8888 | wc -l)
    
    echo "[$(date '+%H:%M:%S')] CPU: ${CPU}% | MEM: ${MEM}% | RSS: ${RSS}KB | Connections: $CONNECTIONS"
    
    sleep 1
done
```

### 故障排除

#### 常见问题检查清单

1. **服务器无法启动**
```bash
# 检查端口占用
sudo netstat -tlnp | grep 8888

# 检查权限
ls -la server/
file server/server

# 检查库依赖
ldd server/server
```

2. **客户端连接失败**
```bash
# 测试网络连通性
telnet <server_ip> 8888
nc -zv <server_ip> 8888

# 检查防火墙
sudo iptables -L | grep 8888
sudo ufw status
```

3. **性能问题**
```bash
# 检查系统负载
top
htop
iostat 1

# 检查网络状况
iftop
netstat -i
```

4. **内存泄漏检查**
```bash
# 使用valgrind检测
valgrind --tool=memcheck --leak-check=full ./server 127.0.0.1 8888 2

# 长时间监控内存使用
watch -n 5 'ps -p $(pgrep server) -o pid,ppid,cmd,%mem,rss'
```

### 备份和恢复

#### 数据备份脚本
```bash
#!/bin/bash
# backup.sh

BACKUP_DIR="/backup/threadpool"
DATA_DIR="/opt/threadpool"
DATE=$(date +%Y%m%d_%H%M%S)

mkdir -p $BACKUP_DIR

# 备份程序文件
tar -czf $BACKUP_DIR/threadpool_$DATE.tar.gz -C $DATA_DIR .

# 备份配置
cp /etc/systemd/system/threadpool.service $BACKUP_DIR/

echo "Backup completed: $BACKUP_DIR/threadpool_$DATE.tar.gz"

# 清理旧备份（保留7天）
find $BACKUP_DIR -name "threadpool_*.tar.gz" -mtime +7 -delete
```

#### 恢复步骤
```bash
# 停止服务
sudo systemctl stop threadpool.service

# 恢复文件
cd /opt/threadpool
sudo tar -xzf /backup/threadpool/threadpool_YYYYMMDD_HHMMSS.tar.gz

# 修复权限
sudo chown -R threadpool:threadpool /opt/threadpool

# 重启服务
sudo systemctl start threadpool.service
sudo systemctl status threadpool.service
```

这套部署指南涵盖了从开发环境到生产环境的完整部署流程，确保项目能够稳定可靠地运行。
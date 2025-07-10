#include <my_header.h>
#define PIPE_A_TO_B "pipe_a_to_b"  // 用户A发送给用户B的管道
#define PIPE_B_TO_A "pipe_b_to_a"  // 用户B发送给用户A的管道
#define BUFFER_SIZE 1024

int main() {
    int write_fd, read_fd;
    char send_message[BUFFER_SIZE];
    char recv_message[BUFFER_SIZE];
    int bytes_read;
    
    printf("=== 用户B聊天程序 ===\n");
    printf("轮流聊天模式：等待用户A发送消息，然后你回复\n");
    printf("输入 'quit' 退出聊天\n\n");
    
    // 等待管道文件存在
    printf("[系统]: 正在连接到用户A...\n");
    while (access(PIPE_A_TO_B, F_OK) != 0 || access(PIPE_B_TO_A, F_OK) != 0) {
        printf("[系统]: 等待用户A创建管道，请确保用户A程序已启动...\n");
        sleep(2);
    }
    
    printf("[系统]: 连接成功！等待用户A发送消息...\n\n");
    
    // 开始通信循环
    while (1) {
        // 1. 接收用户A的消息
        read_fd = open(PIPE_A_TO_B, O_RDONLY);
        if (read_fd == -1) {
            perror("打开读管道失败");
            break;
        }
        
        bytes_read = read(read_fd, recv_message, BUFFER_SIZE - 1);
        if (bytes_read > 0) {
            recv_message[bytes_read] = '\0';
            printf("[用户A说]: %s", recv_message);
            
            // 检查用户A是否退出
            if (strncmp(recv_message, "quit", 4) == 0) {
                printf("[系统]: 用户A已退出聊天\n");
                close(read_fd);
                break;
            }
        } else {
            printf("[系统]: 接收消息失败或用户A已断开连接\n");
            close(read_fd);
            break;
        }
        
        close(read_fd);
        
        // 2. 发送回复给用户A
        printf("[用户B说]: ");
        fflush(stdout);
        
        if (fgets(send_message, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        
        // 检查是否退出
        if (strncmp(send_message, "quit", 4) == 0) {
            // 先发送quit消息通知用户A
            write_fd = open(PIPE_B_TO_A, O_WRONLY);
            if (write_fd != -1) {
                write(write_fd, send_message, strlen(send_message));
                close(write_fd);
            }
            printf("[系统]: 用户B退出聊天\n");
            break;
        }
        
        // 打开写管道，发送回复
        write_fd = open(PIPE_B_TO_A, O_WRONLY);
        if (write_fd == -1) {
            perror("打开写管道失败");
            break;
        }
        
        if (write(write_fd, send_message, strlen(send_message)) == -1) {
            perror("发送消息失败");
            close(write_fd);
            break;
        }
        
        close(write_fd);
        printf("[系统]: 回复已发送，等待用户A的下一条消息...\n\n");
    }
    
    printf("[系统]: 聊天结束\n");
    return 0;
}

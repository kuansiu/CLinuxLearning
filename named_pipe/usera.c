#include <my_header.h>

#define PIPE_A_TO_B "pipe_a_to_b"  // 用户A发送给用户B的管道
#define PIPE_B_TO_A "pipe_b_to_a"  // 用户B发送给用户A的管道
#define BUFFER_SIZE 1024

int main() {
    int write_fd, read_fd;
    char send_message[BUFFER_SIZE];
    char recv_message[BUFFER_SIZE];
    int bytes_read;

    printf("=== 用户A聊天程序 ===\n");
    printf("轮流聊天模式：你发送一条消息，然后等待用户B回复\n");
    printf("输入 'quit' 退出聊天\n\n");

    // 创建有名管道
    printf("[系统]: 正在创建通信管道...\n");

    // 删除可能存在的旧管道
    unlink(PIPE_A_TO_B);
    unlink(PIPE_B_TO_A);

    // 创建新管道
    if (mkfifo(PIPE_A_TO_B, 0666) == -1) {
        perror("创建管道 PIPE_A_TO_B 失败");
        exit(1);
    }

    if (mkfifo(PIPE_B_TO_A, 0666) == -1) {
        perror("创建管道 PIPE_B_TO_A 失败");
        unlink(PIPE_A_TO_B);
        exit(1);
    }

    printf("[系统]: 管道创建成功，等待用户B连接...\n");
    printf("[系统]: 请在另一个终端启动用户B程序\n\n");

    // 开始通信循环
    while (1) {
        // 1. 发送消息给用户B
        printf("[用户A说]: ");
        fflush(stdout);

        if (fgets(send_message, BUFFER_SIZE, stdin) == NULL) {
            break;
        }

        // 检查是否退出
        if (strncmp(send_message, "quit", 4) == 0) {
            printf("[系统]: 用户A退出聊天\n");
            break;
        }

        // 打开写管道，发送消息
        write_fd = open(PIPE_A_TO_B, O_WRONLY);
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
        printf("[系统]: 消息已发送，等待用户B回复...\n");

        // 2. 接收用户B的回复
        read_fd = open(PIPE_B_TO_A, O_RDONLY);
        if (read_fd == -1) {
            perror("打开读管道失败");
            break;
        }

        bytes_read = read(read_fd, recv_message, BUFFER_SIZE - 1);
        if (bytes_read > 0) {
            recv_message[bytes_read] = '\0';
            printf("[用户B回复]: %s", recv_message);

            // 检查用户B是否退出
            if (strncmp(recv_message, "quit", 4) == 0) {
                printf("[系统]: 用户B已退出聊天\n");
                close(read_fd);
                break;
            }
        } else {
            printf("[系统]: 接收消息失败或用户B已断开连接\n");
            close(read_fd);
            break;
        }

        close(read_fd);
        printf("\n");
    }

    // 清理：删除管道文件
    unlink(PIPE_A_TO_B);
    unlink(PIPE_B_TO_A);
    printf("[系统]: 聊天结束，管道已清理\n");

    return 0;
}

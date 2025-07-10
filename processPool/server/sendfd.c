#include "head.h"

int sendfd(int sockfd, int fdtosend){
    // sockfd是传输介质 fdtosend是待发送的文件对象的fd
    struct msghdr hdr; //消息结构
    bzero(&hdr,sizeof(hdr)); // name 填NULL和0
    //消息的正文
    char buf[] = "hello";
    // 准备一个iovec数组
    struct iovec iov[1];
    iov[0].iov_base = buf;
    iov[0].iov_len = 5;
    hdr.msg_iov = iov;
    hdr.msg_iovlen = 1;
    // 消息的控制字段 data要放一个fd，是一个int
    struct cmsghdr *pcmsg = (struct cmsghdr *)malloc(CMSG_LEN(sizeof(int)));
    pcmsg->cmsg_len = CMSG_LEN(sizeof(int)); //总长度
    pcmsg->cmsg_level = SOL_SOCKET;
    pcmsg->cmsg_type = SCM_RIGHTS; // 传文件对象
    *(int *)CMSG_DATA(pcmsg) = fdtosend; //求data首地址-->转int*-->解引用赋值
    hdr.msg_control = pcmsg;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    int ret = sendmsg(sockfd,&hdr,0);
    ERROR_CHECK(ret,-1,"sendmsg");
    return 0;
}
int recvfd(int sockfd, int *pfdtorecv){
    // sockfd是传输介质 fdtosend是待发送的文件对象的fd
    struct msghdr hdr; //消息结构
    bzero(&hdr,sizeof(hdr)); // name 填NULL和0
    char buf[6] = {0};
    struct iovec iov[1];
    iov[0].iov_base = buf;
    iov[0].iov_len = 5;
    hdr.msg_iov = iov;
    hdr.msg_iovlen = 1;
    struct cmsghdr *pcmsg = (struct cmsghdr *)malloc(CMSG_LEN(sizeof(int)));
    pcmsg->cmsg_len = CMSG_LEN(sizeof(int)); //总长度
    pcmsg->cmsg_level = SOL_SOCKET;
    pcmsg->cmsg_type = SCM_RIGHTS; // 传文件对象
    hdr.msg_control = pcmsg;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    int ret = recvmsg(sockfd,&hdr,0);
    ERROR_CHECK(ret,-1,"sendmsg");
    printf("buf = %s, fdtorecv = %d\n",buf, *(int *)CMSG_DATA(pcmsg));
    *pfdtorecv = *(int *)CMSG_DATA(pcmsg);
    return 0;
}

#include "head.h"


int sendfd(int sockfd, int fdtosend)
{
    struct msghdr hdr;
    //name 填NULL和0
    bzero(&hdr,sizeof(hdr));
    char buf[] = "Linux";

    struct iovec iov[1];
    iov[0].iov_base = buf;
    iov[0].iov_len = 5;
    hdr.msg_iov = iov;
    hdr.msg_iovlen = 1;

    struct cmsghdr *pcmsg = (struct cmsghdr*)malloc(CMSG_LEN(sizeof(int)));
    pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
    pcmsg->cmsg_level = SOL_SOCKET;
    pcmsg->cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA(pcmsg) = fdtosend;

    hdr.msg_control = pcmsg;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    int ret = sendmsg(sockfd,&hdr,0);
    ERROR_CHECK(ret,-1,"sendmsg");

    return 0;
}

int recvfd(int sockfd, int *pfdtorecv){
    struct msghdr hdr;
    bzero(&hdr,sizeof(hdr));
    char buf[6]={0};
    struct iovec iov[1];
    iov[0].iov_base = buf;
    iov[0].iov_len = 5;

    hdr.msg_iov = iov;
    hdr.msg_iovlen = 1;
    struct cmsghdr *pcmsg = (struct cmsghdr*)malloc(CMSG_LEN(sizeof(int)));
    pcmsg->cmsg_type = SCM_RIGHTS;
    pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
    pcmsg->cmsg_level = SOL_SOCKET;

    hdr.msg_control = pcmsg;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    int ret = recvmsg(sockfd,&hdr,0);
    ERROR_CHECK(ret,-1,"recvmsg");
    printf("buf = %s,fdtorecv = %d\n",buf,*(int *)CMSG_DATA(pcmsg));
    *pfdtorecv = *(int *)CMSG_DATA(pcmsg);
    return 0;
}

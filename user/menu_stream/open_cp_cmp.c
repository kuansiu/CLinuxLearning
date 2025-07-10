#include <my_header.h>
long long current_time_millis(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts); // 获取当前时间
    return (long long)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000; // 转换为毫秒
}

void copy_file(const char* src_path,const char* dest_path,int len){
      int fdr =open(src_path,O_RDONLY,0666);
      ERROR_CHECK(fdr,-1,"open read  failed");
      int fdw = open(dest_path,O_WRONLY|O_CREAT|O_TRUNC,0666);
      ERROR_CHECK(fdw,-1,"open write failed");
      char *buf=(char*)malloc(len);
 
      ssize_t ret;
      while((ret = read(fdr,buf,len-1))!=0){
          write(fdw,buf,ret);
      }
      close(fdr);
      close(fdw);

}


/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc, 3);
    long long time_1 = current_time_millis();
    copy_file(argv[1],argv[2],1024);
    long long time_2 = current_time_millis();
    copy_file(argv[1],argv[2],2*1024);
    long long time_3 = current_time_millis();
    copy_file(argv[1],argv[2],4*1024);
    long long time_4 = current_time_millis();
    copy_file(argv[1],argv[2],8*1024);
    long long time_5 = current_time_millis();
    printf("size 1024 time cost %lld\n",time_2-time_1);
    printf("size 2*1024 time cost %lld\n",time_3-time_2);
    printf("size 4*1024 time cost %lld\n",time_4-time_3);
    printf("size 8*1024 time cost %lld\n",time_5-time_4);



    return 0;
}


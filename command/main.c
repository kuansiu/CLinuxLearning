#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    printf("argc = %d\n",argc);
    for(int i=0;i<argc;i++){
        printf("第%d个命令行参数是:%s\n",(i+1),argv[i]);
    }
    /* printf("Hello world\n"); */
    return 0;
}


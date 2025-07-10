#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    mode_t mode = 0777;

    int ret = chmod("1.txt",mode);

    printf("chmod ret = %d",ret);

    if(ret==1){
        perror("1.txt");
    }
    printf("\n");
    return 0;
}


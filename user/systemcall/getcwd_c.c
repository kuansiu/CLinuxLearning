#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    char *buf = (char*)malloc(100);
    char *ret = getcwd(buf,100);
    ERROR_CHECK(ret,NULL,"getcwd");
    printf("cwd = %s\n",buf);
    printf("cwd = %s\n",ret);
    printf("%d\n", (ret == buf));
    

    char path[1024]={0};
    char *p = getcwd(path,sizeof(path));
    ERROR_CHECK(ret,NULL,"getcwd2");

    printf("cwd = %s\n",path);
    printf("cwd = %s\n",p);
    printf("%d\n", (path == p));
    free(buf);
    return 0;
}


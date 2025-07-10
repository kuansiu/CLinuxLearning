#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);
   
    char *cwd = (char*)malloc(1024);
    char *ret = getcwd(cwd,1024);
    ERROR_CHECK(ret,NULL,"getcwd");
    printf("cwd = %s\n",cwd);

    int ret2 = chdir(argv[1]);
    ERROR_CHECK(ret2,-1,"chdir");

    char* ret3 = getcwd(cwd,1024);
    printf("chdir after = %s\n",cwd);


    return 0;
}


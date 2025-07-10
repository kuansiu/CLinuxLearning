#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    DIR *dirp = opendir(".");
    ERROR_CHECK(dirp,NULL,"opendir");

    struct dirent *entry;
    printf("当前目录文件列表:\n");
    while((entry = readdir(dirp))!=NULL){
       printf("%s\n",entry->d_name); 
    }
    closedir(dirp);

    return 0;
}


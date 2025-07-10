#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);
    DIR* dirp = opendir(argv[1]);
    ERROR_CHECK(dirp,NULL,"opendir");
    struct dirent *pdirent;
    long loc;
    while((pdirent=readdir(dirp))!=NULL){
        printf("inode num = %lu,reclen = %hu,type = %u,name = %s\n\n",
               pdirent->d_ino,
               pdirent->d_reclen,
               pdirent->d_type,
               pdirent->d_name);
        if(strcmp("3.txt",pdirent->d_name)==0){
            loc = telldir(dirp);
            break;
        }
    }
    printf("----------------seekdir---------------------\n");
    seekdir(dirp,loc);
    pdirent = readdir(dirp);
    printf("inode num = %lu,reclen = %hu,type = %u,name =%s\n\n", 
                     pdirent->d_ino,
                     pdirent->d_reclen,
                     pdirent->d_type,
                     pdirent->d_name); 

    printf("-------------------rewinddir------------------\n"); 
    rewinddir(dirp);
    pdirent = readdir(dirp);
    printf("inode num = %lu,reclen = %hu,type = %u,name =%s\n\n", 
                     pdirent->d_ino,
                     pdirent->d_reclen,
                     pdirent->d_type,
                     pdirent->d_name); 


    closedir(dirp);

    return 0;
}


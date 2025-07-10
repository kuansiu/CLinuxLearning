#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){
    ARGS_CHECK(argc,2);
    DIR *dirp =  opendir(argv[1]);
    ERROR_CHECK(dirp,NULL,"opendir");

    struct dirent* pdirent;

    while((pdirent = readdir(dirp))!=NULL){
        printf("%lu %d %d %s \n" ,
               pdirent->d_ino,
               pdirent->d_reclen,
               pdirent->d_type,
               pdirent->d_name);
        switch (pdirent->d_type){
             case DT_REG:
                printf("%d DT_REG  ",pdirent->d_type);
                break;
            case DT_DIR:
                printf("%d DT_DIR  ",pdirent->d_type);
                break;
            case DT_LNK:
                printf("%d DT_LNK  ",pdirent->d_type);
            default:
                break;
        }
            

    }
    closedir(dirp);
    return 0;
}


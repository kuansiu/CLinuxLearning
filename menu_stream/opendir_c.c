#include <my_header.h>
#include <dirent.h>
#include <sys/types.h>

/* DIR *opendir(const char *name); */

/* int closedir(DIR *dirp); */

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);
    DIR *dirp = opendir(argv[1]);
    ERROR_CHECK(dirp,NULL,"opendir");

    closedir(dirp);


    /* printf("Hello orld\n"); */
    return 0;
}


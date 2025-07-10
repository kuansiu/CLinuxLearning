#include <my_header.h>

void my_tree(char* path,int level){
    DIR* dirp = opendir(path);
    ERROR_CHECK(dirp,NULL,"opendir");

    struct dirent *de;
    struct stat sb;

    while((de = readdir(dirp))!=NULL){
        if(strcmp(de->d_name,".")==0||strcmp(de->d_name,"..")==0){
            continue;
        }

        for(int i =0;i<level;i++){
            printf("│   ");
        }
        printf("├──%s\n",de->d_name);

        char fullpath[1024] ={0};
        snprintf(fullpath,sizeof(fullpath),"%s/%s",path,de->d_name);

        if(stat(fullpath,&sb)==0&&S_ISDIR(sb.st_mode)){
            my_tree(fullpath,level+1);
        }
    }
    closedir(dirp);


}





/* Usage:  */
int main(int argc, char *argv[]){                                  
    const char *start_dir = ".";
    if(argc>=2){
        start_dir = argv[1];
    }

    printf("%s\n",start_dir);
    my_tree(start_dir,0);

    return 0;
}


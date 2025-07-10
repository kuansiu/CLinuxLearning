#include <my_header.h>
int compare(const void* a,const void* b);
void set_type_and_mode(mode_t mode,char str[20]){
    
    switch (mode & S_IFMT) {
    case S_IFBLK:   str[0] = 'b';        break;
    case S_IFCHR:   str[0] = 'c';        break;
    case S_IFDIR:   str[0] = 'd';        break;
    case S_IFIFO:   str[0] = 'p';        break;
    case S_IFLNK:   str[0] = 'l';        break;
    case S_IFREG:   str[0] = '-';        break;
    case S_IFSOCK:  str[0] = 's';        break;
    default:        str[0] = '?';        break;
    }

    str[1] =((mode & 0400)!=0)?'r':'-';
    str[2] =((mode & 0200)!=0)?'w':'-';    
    str[3] =((mode & 0100)!=0)?'x':'-';    
    str[4] =((mode & 0040)!=0)?'r':'-';    
    str[5] =((mode & 0020)!=0)?'w':'-';    
    str[6] =((mode & 0010)!=0)?'x':'-';    
    str[7] =((mode & 0004)!=0)?'r':'-';    
    str[8] =((mode & 0002)!=0)?'w':'-';    
    str[9] =((mode & 0001)!=0)?'x':'-';    
    str[10]='\0';
}

void format_time(time_t mtime,char* time_str){
    
    /* const char month_arr[][10]={ */
    /*     "1月", "2月", "3月", "4月", "5月", "6月", */
    /*     "7月", "8月", "9月", "10月", "11月", "12月" */
    /* }; */

    struct tm* st_tm = localtime(&mtime);
    
    sprintf(time_str,"%2d月 %2d %02d:%02d",
            st_tm->tm_mon+1,
            st_tm->tm_mday,
            st_tm->tm_hour,
            st_tm->tm_min);
}


/* Usage:  */
int main(int argc, char *argv[]){                                  
    char* dir_name;
    if(argc==1){
        dir_name = ".";
    }else if(argc ==2){
        dir_name=argv[1];
    }else{
        perror("argc num error");
        exit(EXIT_FAILURE);
    }

    DIR *dirp = opendir(dir_name);
    ERROR_CHECK(dirp,NULL,"opendir");

    int ret = chdir(dir_name);
    ERROR_CHECK(ret,-1,"chdir");

    struct dirent *pdirent;
    int count = 0;

    while((pdirent = readdir(dirp))!=NULL){
        count++;
    }
    
    struct dirent** dir_arr = (struct dirent**)malloc(sizeof(struct dirent*)*count);
    ERROR_CHECK(dir_arr,NULL,"malloc dlist");
    rewinddir(dirp);

    int idx =0;
    while((pdirent = readdir(dirp))!=NULL){
        dir_arr[idx]=pdirent;
        idx++;
    }

    qsort(dir_arr,count,sizeof(struct dirent *),compare);



    for(int i=0;i<count;i++){

        struct stat stat_buf;
        
        int ret = stat(dir_arr[i]->d_name,&stat_buf);
        ERROR_CHECK(ret,-1,"stat");

        char mode_str[1024]={0};
        set_type_and_mode(stat_buf.st_mode,mode_str);

        char time_str[1024]={0};
        format_time(stat_buf.st_mtim.tv_sec,time_str);
        printf("%-10s %3lu %-8s %-8s %10lu %s %-20s\n",
                mode_str,                    // %-10s: 左对齐，10字符宽度
                stat_buf.st_nlink,          // %3lu:  右对齐，3字符宽度
                getpwuid(stat_buf.st_uid)->pw_name,  // %-8s:  左对齐，8字符宽度
                getgrgid(stat_buf.st_gid)->gr_name,  // %-8s:  左对齐，8字符宽度
                stat_buf.st_size,           // %8lu:  右对齐，8字符宽度
                time_str,                   // 时间字符串
                dir_arr[i]->d_name             // 文件名
               );
    }
    free(dir_arr);
    closedir(dirp);
    return 0;
}

int compare(const void* a,const void* b){
    struct dirent* dir_a = *(struct dirent**)a;
    struct dirent* dir_b = *(struct dirent**)b;

    char file_name[256];
    char file_name1[256];

    strncpy(file_name,dir_a->d_name,sizeof(file_name)-1);
    strncpy(file_name1,dir_b->d_name,sizeof(file_name1)-1);

    file_name[sizeof(file_name)-1]='\0';
    file_name1[sizeof(file_name1)-1]='\0';

    return strcasecmp(file_name,file_name1);
}





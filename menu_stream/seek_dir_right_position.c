#include <my_header.h>
/* 方法2：记录123.txt的位置编号，然后重新遍历到该位置 */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);
    DIR* dirp = opendir(argv[1]);
    ERROR_CHECK(dirp,NULL,"opendir");
    struct dirent *pdirent;
    int target_position = -1;
    int current_count = 0;
    
    // 第一次遍历：显示所有文件并记录123.txt的位置编号
    while((pdirent=readdir(dirp))!=NULL){
        printf("inode num = %lu,reclen = %hu,type = %u,name = %s\n",
               pdirent->d_ino,
               pdirent->d_reclen,
               pdirent->d_type,
               pdirent->d_name);
               
        if(strcmp("3.txt",pdirent->d_name)==0){
            target_position = current_count; // 记录123.txt是第几个文件
        }
        current_count++;
    }
    
    printf("--------------------------\n");
    
    // 重新遍历到目标位置
    if(target_position != -1) {
        rewinddir(dirp);
        current_count = 0;
        
        while((pdirent=readdir(dirp))!=NULL && current_count <= target_position){
            if(current_count == target_position) {
                // 找到了123.txt
                printf("inode num = %lu,reclen = %hu,type = %u,name =%s\n",
                       pdirent->d_ino,
                       pdirent->d_reclen,
                       pdirent->d_type,
                       pdirent->d_name);
                break;
            }
            current_count++;
        }
    }
    
    printf("---------------------------------------\n");
    rewinddir(dirp);
    pdirent = readdir(dirp);
    printf("inode num = %lu,reclen = %hu,type = %u,name =%s\n",
           pdirent->d_ino,
           pdirent->d_reclen,
           pdirent->d_type,
           pdirent->d_name);
    
    closedir(dirp);
    return 0;
}

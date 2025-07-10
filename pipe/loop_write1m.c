#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){
    int num;
    sscanf(argv[1],"%d",&num);
    for(int i = 0;i<num;i++){
        system("./write1m");
        /* printf("run %d",i); */
    }
    return 0;
}


#include <my_header.h>


int main()
{
    int x = 0x12345678;
    printf("x = %x\n",x);
    char* p = (char*)&x;
    printf("*p = %x\n",*p);
    return 0;
}


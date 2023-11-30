
#include<unistd.h>
#include<stdio.h>
int main(){
    
    int z = 100;

    if(fork()) z+= 10;

    z-=10;
    
    printf("%d\n", z);

    return 0;
}

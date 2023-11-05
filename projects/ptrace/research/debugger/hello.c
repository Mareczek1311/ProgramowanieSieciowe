#include <stdio.h>

int f3(int i){
    int j = 0x44444444;
    return i+j;
}

int f2(int i, int j){
    int k;
    k = f3(i+j);
    return k;
}

int f1(int i ){
    return f2(i, 0x22222222);
}

int main(){
    
    int h = f1(0x11111111);


    printf("HELLO! f1 returned: %X \n", h);

    return 0;
}



#include <stdio.h>


int main(void) {
    int ret = 2000;

    printf("myfunc(32) is %d\n", ret);
    ret += 100;
    printf("myfunc(32) is %d\n", ret);

    return 0;
}

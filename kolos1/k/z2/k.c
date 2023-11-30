#include <signal.h>
#include <stdio.h>
int main(){
    int n;
    scanf("%d", &n);
    kill(SIGUSR1, n);
}

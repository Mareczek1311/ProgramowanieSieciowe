#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int variable = 3;
int main(){
    variable = 0;
    printf("\nCHILD: Variable = %d\n", variable);


    printf("\nCHILD: Variable = %d\n", variable);

    return 0;
}

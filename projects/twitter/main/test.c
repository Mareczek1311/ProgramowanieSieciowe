#include "data.h"

int main(){
    struct database* db;
    db = (struct database*)malloc(sizeof(struct database));

    printf("%lu\n",  sizeof(struct database));

    return 0;
}
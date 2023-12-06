#include "data.h"

int main(){
    struct database* db;
    db = (struct database*)malloc(sizeof(struct database));
    db->posts = (struct post**) malloc(10 * sizeof(struct post*));

    printf("%lu\n",  sizeof(struct database));

    return 0;
}
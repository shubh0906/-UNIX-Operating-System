#include "../include/unistd.h"
#include "../include/sys/defs.h"
#include "../include/string.h"

struct mallocinfo{
    uint64_t size;
    uint64_t address;
    int alloc1dealloc0;
    struct mallocinfo *next;
};

struct mallocinfo* start=NULL;

void free(uint64_t add){
    struct mallocinfo *temp = start;
    while (temp) {
        if (add == temp->address){
            temp->alloc1dealloc0=0;
            memset((void*)temp->address,temp->size);
            break;
        }
        temp = temp->next;
    }
}

uint64_t malloc(uint64_t size){
    if (start==NULL){
        uint64_t mallocadd = (uint64_t)sb_malloc(size);
        start = (struct mallocinfo*)sb_malloc(sizeof(struct mallocinfo));
        start->size = size;
        start->address=mallocadd;
        start->alloc1dealloc0=1;
        start->next=NULL;
        return mallocadd;
    }
    else {
        uint64_t min=10000;
        //uint64_t minadd=0;
        struct mallocinfo* mintemp=NULL;
        int flag=0;
        struct mallocinfo *temp = start;
        while (temp->next) {
            if (temp->alloc1dealloc0==0){
                if (temp->size>size) {
                    if (min > temp->size) {
                        min = size;
                        // minadd=temp->address;
                        mintemp = temp;
                        flag = 1;
                    }
                }
            }
            temp = temp->next;
        }
        if (flag==1){
           // mintemp->address = minadd;
            //mintemp->size=min;
            mintemp->alloc1dealloc0=1;
            return mintemp->address;
        }
        else {
            uint64_t mallocadd = (uint64_t)sb_malloc(size);
            struct mallocinfo* temp1  = sb_malloc(sizeof(struct mallocinfo));
            temp1->size = size;
            temp1->address = mallocadd;
            temp1->alloc1dealloc0 = 1;
            temp1->next = '\0';
            temp->next=temp1;
            return mallocadd;
        }
    }

}
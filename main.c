#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

void *mmalloc(size_t size);

int main(void) {
    int* arr = (int*)mmalloc(sizeof(int)*10);
    for (int i = 0; i < 10; ++i) {
        arr[i] = i;
    }
    printf("%d\n", arr);
    printf("%d\n", arr[5]);
    printf("%d\n", &arr[5]);
    return 0;
}


void *mmalloc(size_t size) {
    void *newBlock;
    newBlock = sbrk(size);
    if (newBlock == (void*)-1) {
        return NULL;
    }
    return newBlock;
}
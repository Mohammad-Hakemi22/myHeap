#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("hello world!\n");
    printf("%x", sbrk(0));
    return 0;
}
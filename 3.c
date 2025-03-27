#include <stdio.h>
#include <stdlib.h>

int g = 1;

void func(){
    printf("Func called\n");
}

int main(){
    int *p = (int *)malloc(sizeof(int));
    *p = 1;

    printf("Text segment, aka main(), is at: %p\n", (void*)main);
    printf("Global int (data segment) is at: %p\n", &g);
    printf("Heap is at: %p\n", p);

    int i;
    printf("The stack top is near: %p\n", &i);
    func();
    int arr[1000];
    char charr[1000];
    int i2;
    printf("Now stack top is near: %p\n", &i2);

    free(p);
}
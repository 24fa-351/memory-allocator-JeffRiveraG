#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>

void test_malloc_free()
{
    printf("Running test_malloc_free...\n");

    void *ptr1 = my_malloc(100);
    if (!ptr1)
    {
        printf("FAIL: malloc returned NULL\n");
        return;
    }

    my_free(ptr1);
    printf("PASS: malloc and free\n");
}

void test_realloc()
{
    printf("Running test_realloc...\n");

    void *ptr = my_malloc(50);
    if (!ptr)
    {
        printf("FAIL: malloc returned NULL\n");
        return;
    }

    void *new_ptr = my_realloc(ptr, 100);
    if (!new_ptr)
    {
        printf("FAIL: realloc returned NULL\n");
        return;
    }

    my_free(new_ptr);
    printf("PASS: realloc\n");
}

int main(int argc, char **argv)
{
    memory_manager_init(1024 * 1024);

    test_malloc_free();
    test_realloc();

    memory_manager_destroy();
    return 0;
}

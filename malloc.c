#include "malloc.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define HEAP_SIZE 1024 * 1024

typedef struct Block
{
    size_t size;
    int free;
    struct Block *next;
} Block;

static Block *freeList = NULL;
static void *heap = NULL;

void *get_me_blocks(ssize_t how_much)
{
    void *ptr = sbrk(0);
    if (sbrk(how_much) == (void *)-1)
    {
        return NULL;
    }
    return ptr;
}

void initialize()
{
    heap = get_me_blocks(HEAP_SIZE);
    if (heap == NULL)
    {
        fprintf(stderr, "Failed to initialize heap\n");
        return;
    }
    freeList = (Block *)heap;
    freeList->size = HEAP_SIZE - sizeof(Block);
    freeList->free = 1;
    freeList->next = NULL;
}

void memory_manager_init(size_t total_memory)
{
    heap = get_me_blocks(total_memory);
    if (heap == NULL)
    {
        fprintf(stderr, "Failed to initialize memory manager with %zu bytes\n", total_memory);
        return;
    }
    freeList = (Block *)heap;
    freeList->size = total_memory - sizeof(Block);
    freeList->free = 1;
    freeList->next = NULL;
    fprintf(stderr, "Memory manager initialized with %zu bytes\n", total_memory);
}

void memory_manager_destroy()
{
    heap = NULL;
    freeList = NULL;
    fprintf(stderr, "Memory manager destroyed\n");
}

void split(Block *fitting_slot, size_t size)
{
    Block *newBlock = (void *)((void *)fitting_slot + size + sizeof(Block));
    newBlock->size = (fitting_slot->size) - size - sizeof(Block);
    newBlock->free = 1;
    newBlock->next = fitting_slot->next;
    fitting_slot->size = size;
    fitting_slot->free = 0;
    fitting_slot->next = newBlock;
}

void *my_malloc(size_t size)
{
    Block *curr;
    if (!(freeList->size))
    {
        initialize();
        if (heap == NULL)
        {
            return NULL;
        }
    }
    curr = freeList;
    while ((((curr->size) < size) || ((curr->free) == 0)) && (curr->next != NULL))
    {
        curr = curr->next;
    }
    if ((curr->size) == size)
    {
        curr->free = 0;
        return (void *)(++curr);
    }
    else if ((curr->size) > (size + sizeof(Block)))
    {
        split(curr, size);
        return (void *)(++curr);
    }
    else
    {
        return NULL;
    }
}

void merge()
{
    Block *curr;
    curr = freeList;
    while ((curr->next) != NULL)
    {
        if ((curr->free) && (curr->next->free))
        {
            curr->size += (curr->next->size) + sizeof(Block);
            curr->next = curr->next->next;
        }
        curr = curr->next;
    }
}

void my_free(void *ptr)
{
    if (((void *)heap <= ptr) && (ptr <= (void *)(heap + HEAP_SIZE)))
    {
        Block *curr = ptr;
        --curr;
        curr->free = 1;
        merge();
    }
}

void *my_realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
    {
        return my_malloc(size);
    }
    Block *curr = (Block *)ptr - 1;
    if (curr->size >= size)
    {
        return ptr;
    }
    void *new_ptr = my_malloc(size);
    if (new_ptr)
    {
        memcpy(new_ptr, ptr, curr->size);
        my_free(ptr);
    }
    return new_ptr;
}
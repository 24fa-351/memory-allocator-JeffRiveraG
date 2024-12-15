#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>
#include <unistd.h>

void *my_malloc(size_t size);
void my_free(void *ptr);
void *my_realloc(void *ptr, size_t size);
void memory_manager_init(size_t total_memory);
void memory_manager_destroy();
void *get_me_blocks(ssize_t how_much);

#endif

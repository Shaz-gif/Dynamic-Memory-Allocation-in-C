#include "my_realloc.h"
#include "my_malloc.h"
#include "my_free.h"
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

#define META_BLOCK_SIZE sizeof(struct meta_block)
#define MMAP_THRESHOLD (128 * 1024)

typedef struct meta_block *meta_ptr;

struct meta_block {
    int free;
    size_t size;
    meta_ptr next;
    meta_ptr prev;
    void *ptr;
    char data[1];
};

static void *base = NULL;

static meta_ptr get_block_addr(void *p) {
    return (meta_ptr)((char *)p - META_BLOCK_SIZE);
}

static int is_addr_valid(void *p) {
    if (base && p) {
        void *heap_end = sbrk(0);
        if (p >= (void*)base && p < heap_end) {
            meta_ptr block = get_block_addr(p);
            return (p == block->ptr);
        }
    }
    return 0;
}

void *my_realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return my_malloc(size);
    }

    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    if (!is_addr_valid(ptr)) {
        return NULL;
    }

    meta_ptr block = get_block_addr(ptr);
    void *new_ptr;

    if (block->size >= size) {
        // If the current block is large enough, we can just use it
        return ptr;
    } else {
        // If the current block is too small, we need to allocate a new block
        new_ptr = my_malloc(size);
        if (new_ptr == NULL) {
            return NULL;
        }

        // Copy the old data to the new block
        memcpy(new_ptr, ptr, block->size);

        // Free the old block
        my_free(ptr);

        return new_ptr;
    }
}

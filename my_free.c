#include "my_free.h"
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

static meta_ptr merge_blocks(meta_ptr block) {
    if (block->next && block->next->free) {
        block->size += META_BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }
    return block;
}

void my_free(void *ptr) {
    if (!ptr) return;

    if (is_addr_valid(ptr)) {
        meta_ptr block = get_block_addr(ptr);
        if (block->free) {
            fprintf(stderr, "Error: Double free detected.\n");
            return;
        }
        block->free = 1;

        if (block->prev && block->prev->free) {
            block = merge_blocks(block->prev);
        }

        if (block->next) {
            block = merge_blocks(block);
        } else {
            if (block->prev) {
                block->prev->next = NULL;
            } else {
                base = NULL;
            }
            if (block->size >= MMAP_THRESHOLD) {
                if (munmap(block, block->size + META_BLOCK_SIZE) == -1) {
                    fprintf(stderr, "Error: Failed to unmap memory.\n");
                }
            } else {
                brk(block);
            }
        }
    } 
}

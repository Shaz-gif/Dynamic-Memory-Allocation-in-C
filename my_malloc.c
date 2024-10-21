#include "my_malloc.h"
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define META_BLOCK_SIZE sizeof(struct meta_block)
#define align4(x) (((((x)-1) >> 2) << 2) + 4)
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

static meta_ptr find_suitable_block(meta_ptr *last, size_t size) {
    meta_ptr b = base;
    while (b && !(b->free && b->size >= size)) {
        *last = b;
        b = b->next;
    }
    return b;
}

static void split_space(meta_ptr block, size_t size) {
    meta_ptr new_block;
    if (block->size - size >= META_BLOCK_SIZE + 4) {
        new_block = (meta_ptr)(block->data + size);
        new_block->size = block->size - size - META_BLOCK_SIZE;
        new_block->next = block->next;
        new_block->free = 1;
        new_block->ptr = new_block->data;
        new_block->prev = block;
        block->next = new_block;
        block->size = size;

        if (new_block->next) {
            new_block->next->prev = new_block;
        }
    }
}

static meta_ptr extend_heap(meta_ptr last, size_t size) {
    meta_ptr new_block;
    if (size >= MMAP_THRESHOLD) {
        new_block = mmap(NULL, size + META_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (new_block == MAP_FAILED) {
            errno = ENOMEM;
            return NULL;
        }
    } else {
        new_block = (meta_ptr)sbrk(META_BLOCK_SIZE + size);
        if (new_block == (void *)-1) {
            errno = ENOMEM;
            return NULL;
        }
    }
    new_block->size = size;
    new_block->free = 0;
    new_block->next = NULL;
    new_block->ptr = new_block->data;

    if (last) {
        last->next = new_block;
        new_block->prev = last;
    } else {
        new_block->prev = NULL;
    }

    return new_block;
}

void *my_malloc(size_t size) {
    if (size == 0) return NULL;

    meta_ptr block, last = base;
    size_t s = align4(size);

    if (base) {
        block = find_suitable_block(&last, s);
        if (block) {
            if (block->size - s >= (META_BLOCK_SIZE + 4)) {
                split_space(block, s);
            }
            block->free = 0;
        } else {
            block = extend_heap(last, s);
            if (!block) return NULL;
        }
    } else {
        block = extend_heap(NULL, s);
        if (!block) return NULL;
        base = block;
    }

    return block->data;
}

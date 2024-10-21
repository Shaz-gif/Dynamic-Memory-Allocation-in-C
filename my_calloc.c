#include "my_calloc.h"
#include "my_malloc.h"
#include <string.h>
#include <errno.h>

void *my_calloc(size_t number, size_t size) {
    if (number == 0 || size == 0) return NULL;

    size_t total_size;
    if (__builtin_mul_overflow(number, size, &total_size)) {
        errno = ENOMEM;
        return NULL;
    }

    void *ptr = my_malloc(total_size);
    if (ptr) {
        memset(ptr, 0, total_size);
    }
    return ptr;
}

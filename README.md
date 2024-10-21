# Dynamic Memory Allocation using C

## Overview
This project demonstrates the implementation of dynamic memory allocation functions in C, including `malloc()`, `free()`, `calloc()`, and `realloc()`. Each function is implemented in separate files, ensuring modularity and ease of understanding.

The memory is allocated as different blocks based on the requests. Each block contains two sections:
1. **Meta Information (meta_block)**: Contains details about the memory block.
2. **Data Section**: Used for storing the actual data.

The meta information helps when freeing allocated memory. Blocks are maintained in the order of their allocation, and they form a doubly linked list. This structure aids in managing memory fragmentation and keeping track of both free and used memory spaces.

**Alignment**: The requested size is aligned to the nearest multiple of 4 for simplified address handling.

**System Calls**: The `sbrk()` and `brk()` system calls are used to manage the heap's break.

---

## malloc.c
- Contains the `meta_block` structure which stores details about each memory block.
- **meta_block** stores:
  - Size of the corresponding memory block.
  - Whether the memory block is free or not.
  - Pointers to the previous and next nodes in the doubly linked list.
  - Pointer to the start of the data section.
- A character array helps in determining where data can be stored within the block.

### Functions:
- **find_suitable_block()**: Finds a free memory block with at least the required space.
- **split_space()**: Splits a large block if there's extra space after allocation.
- **extend_heap()**: Extends the heap and adds a new block to the list.

### malloc() workflow:
1. If the linked list is empty (`base` is NULL), a new block is created using `extend_heap()`.
2. If a suitable block exists, it is returned; otherwise, the heap is extended.
3. If the block found has extra space, `split_space()` is invoked.

---

## free.c
- **free()** first checks if the pointer passed is valid using `is_addr_valid()`.
- The **meta_block** of the memory location is found using `get_block_addr()`.
- The block is merged with adjacent blocks if any are free using `merge_block()`.
- If the block to be freed is at the end of the list, it is removed, and the heap break is adjusted using `brk()`.

---

## calloc.c
- Uses `malloc()` to allocate memory and sets all bytes to 0 by iterating through the allocated space.

---

## realloc.c
- If the old address pointer is `NULL`, `malloc()` is used to allocate memory.
- If the old memory block is large enough, it is reused.
- If not, the next block is checked to see if it can be merged.
- If merging is possible, the block is split if there's extra space.
- If merging is not possible, a new block is allocated, and the data is copied using `copy_data()`.

---

## main.c
This file includes testing code to verify all the implemented functions.
- Two variables are created and allocated space using `malloc()`.
- The first variable is freed, and `malloc()` is called again to show space reuse.
- `calloc()` is then used to allocate an integer array of size 5 (values initialized to 0).
- The array is resized to size 10 using `realloc()`.

---

## References
- [Linux Manual page - sbrk()](https://man7.org/linux/man-pages/man2/sbrk.2.html)
- [Linux Manual page - brk()](https://man7.org/linux/man-pages/man2/brk.2.html)
- [GeeksforGeeks: Dynamic Memory Allocation in C](https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc

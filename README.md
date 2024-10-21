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
# Custom Memory Management Library

This project implements custom versions of malloc, calloc, realloc, and free functions.

## Files

- `my_malloc.c` and `my_malloc.h`: Implementation of custom malloc
- `my_calloc.c` and `my_calloc.h`: Implementation of custom calloc
- `my_realloc.c` and `my_realloc.h`: Implementation of custom realloc
- `my_free.c` and `my_free.h`: Implementation of custom free
- `Makefile`: Build script for compiling the library

## Compilation Steps

1. Ensure you have `gcc` and `make` installed on your system.

2. Clone or download this repository to your local machine.

3. Open a terminal and navigate to the project directory.

4. Run the make command to compile the library:

   ```
   make
   ```

   This will create object files for each .c file and then combine them into a static library named `libmymemory.a`.

5. To clean up the compiled files, you can use:

   ```
   make clean
   ```

## Using the Library

To use this custom memory management library in your project:

1. Include the necessary headers in your C file:

   ```c
   #include "my_malloc.h"
   #include "my_calloc.h"
   #include "my_realloc.h"
   #include "my_free.h"
   ```

2. Compile your program with the library. For example, if your main file is named `main.c`:

   ```
   gcc -o my_program main.c -L. -lmymemory
   ```

   This assumes that `libmymemory.a` is in the current directory. The `-L.` flag tells the compiler to look for libraries in the current directory, and `-lmymemory` links against `libmymemory.a`.

3. Run your program:

   ```
   ./my_program
   ```

## Note

This implementation is for educational purposes and may not be suitable for production use. It doesn't include all the optimizations and safety checks that a production-quality allocator would have.




## References
- [Linux Manual page - sbrk()](https://man7.org/linux/man-pages/man2/sbrk.2.html)
- [Linux Manual page - brk()](https://man7.org/linux/man-pages/man2/brk.2.html)
- [GeeksforGeeks: Dynamic Memory Allocation in C](https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc

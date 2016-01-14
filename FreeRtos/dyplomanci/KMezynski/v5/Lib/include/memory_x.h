#ifndef MEMORY_X_H
#define MEMORY_X_H

#include <stdlib.h>

/**
 * Allocate memory
 * @param size - amount of memory to allocate
 */
void  *xmalloc(size_t size);

/**
 * Amount of free space on heap
 * @return number of free bytes on heap
 */
size_t xmallocAvailable(void);

#endif
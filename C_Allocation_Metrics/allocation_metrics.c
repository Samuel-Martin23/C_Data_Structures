#include "allocation_metrics.h"

allocation_metrics_t mem_usage = {0, 0};

void print_total_mem()
{
    printf("Total Memory: %d Bytes\n", mem_usage.allocated - mem_usage.freed);
}

void print_allocated_mem()
{
    printf("Memory Allocated: %d Bytes\n", mem_usage.allocated);
}

void print_freed_mem()
{
    printf("Memory Freed: %d Bytes\n", mem_usage.freed);
}

void *new_mem(size_t bytes)
{
    mem_usage.allocated += (u_int32_t)bytes;
    return malloc(bytes);
}

void free_mem(void *mem, size_t bytes)
{
    mem_usage.freed += (u_int32_t)bytes;
    free(mem);
}

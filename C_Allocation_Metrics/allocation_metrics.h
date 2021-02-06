
#ifndef ALLOCATION_METRICS_H
#define ALLOCATION_METRICS_H

#include <stdio.h> 
#include <stdlib.h>

typedef struct allocation_metrics
{
    uint32_t allocated;
    uint32_t freed;

} allocation_metrics_t;

extern allocation_metrics_t mem_usage;

void print_total_mem();
void print_allocated_mem();
void print_freed_mem();

#endif /* ALLOCATION_METRICS_H */

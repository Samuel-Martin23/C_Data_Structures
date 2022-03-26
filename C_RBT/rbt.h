#ifndef RBT_H
#define RBT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#define RBT_CAPACITY 50

typedef struct rbt
{
    size_t size;
    size_t capacity;
    size_t longest_path;
    int **data;
} rbt_t;

void right_rotate(int **arr, size_t rotate_index);
void left_rotate(int **arr, size_t rotate_index);
int *alloc_int(int x);

rbt_t *rbt_init(void);
rbt_t *rbt_init_args(size_t size, ...);
void rbt_print(rbt_t *tree);
void rbt_free(rbt_t **tree);

#endif /* RBT_H */

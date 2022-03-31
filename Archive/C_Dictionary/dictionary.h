#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdio.h>
#include <stdlib.h>
#include "../C_Template/template.h"
#include "../C_Allocation_Metrics/allocation_metrics.h"
#include <stdarg.h>

typedef struct node_dict
{
    void *key;
    void *value;
    struct node_dict *left;
    struct node_dict *right;
} node_dict_t;

typedef struct dictionary
{
    int size;
    template_t T_key;
    template_t T_value;
    node_dict_t *root;
} dictionary_t;

dictionary_t dict_init(template_t T_key, template_t T_value);
void dict_insert(dictionary_t *dict, ...);
void dict_remove_key(dictionary_t *dict, ...);
void *dict_lookup(dictionary_t *dict, ...);
void dict_copy(dictionary_t *dict_dest, dictionary_t *dict_src);
void dict_free(dictionary_t *dict);
void dict_print(dictionary_t *dict);

#endif /* DICTIONARY_H */

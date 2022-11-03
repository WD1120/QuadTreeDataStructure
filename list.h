/*----------------------------------------------------
..Project: dict1 dict2
  list.h :
         = the interface of module list of the project
----------------------------------------------------*/

#ifndef _LIST_H_
#define _LIST_H_
#include "data.h"

typedef struct node node_t;
struct node {
    data_t data;
    node_t *next;
};

typedef struct list list_t;

// Function prototypes
list_t *make_empty_list(void);

int is_empty_list(list_t *list);

void free_list(list_t *list, void (*dataFreeFunc)(void *));

list_t *insert_at_head(list_t *list, data_t value);

list_t *insert_at_foot(list_t *list, data_t value);

node_t *get_head(list_t *list);

list_t *get_tail(list_t *list);

#endif
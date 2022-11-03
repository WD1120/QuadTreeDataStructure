/*----------------------------------------------------
..Project: dict1 dict2 dict3 dict4
  list.c :
         = the implementation of module list of the project

These functions are imported from COMP10002 linked list 
implementation classes
----------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"
#include "data.h"

/* Structure definitiion */
struct list {
    node_t *head;
    node_t *foot;
};

/* Create an empty list */
list_t *make_empty_list(void) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}

/* Checks if the list is empty */
int is_empty_list(list_t *list) {
	assert(list!=NULL);
	return list->head==NULL;
}

/* Frees the list */
void free_list(list_t *list, void (*dataFreeFunc)(void *)) {
	node_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		dataFreeFunc(&prev->data);
		free(prev);
	}
	free(list);
}

/* Inseart an element into the beginning of the list */
list_t *insert_at_head(list_t *list, data_t value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = list->head;
	list->head = new;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->foot = new;
	}
	return list;
}

/* Inseart an element at the end of the list */
list_t *insert_at_foot(list_t *list, data_t value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = NULL;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}

/* Get the head node of the list */
node_t *get_head(list_t *list) {
	assert(list!=NULL && list->head!=NULL);
	return list->head;
}

/* Get the tail node of the list */
list_t *get_tail(list_t *list) {
	node_t *oldhead;
	assert(list!=NULL && list->head!=NULL);
	oldhead = list->head;
	list->head = list->head->next;
	if (list->head==NULL) {
		/* the only list node just got deleted */
		list->foot = NULL;
	}
	free(oldhead);
	return list;
}


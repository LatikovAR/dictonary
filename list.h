#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

struct sl_list;

void add_list(struct sl_list** list_arr, int hash, int position);
int read_list(struct sl_list* list);
void free_list(struct sl_list* list);

#endif // LIST_H

#include "list.h"
#include "hash_string.h"

struct sl_list {
    struct sl_list* next;
    int data;
};

void add_list(struct sl_list** list_arr, int hash, int position) {
    struct sl_list* last_list;
    assert(list_arr != nullptr);

    if(list_arr[hash] == nullptr) {
        list_arr[hash] = (sl_list*) calloc(1, sizeof (struct sl_list));
        assert(list_arr[hash] != nullptr);
        list_arr[hash]->data = position;
        list_arr[hash]->next = nullptr;
    }
    else {
        last_list = list_arr[hash];
        while(last_list->next != nullptr) {
            last_list = last_list->next;
        }
        last_list->next = (sl_list*) calloc(1, sizeof (struct sl_list));
        last_list = last_list->next;
        last_list->data = position;
        last_list->next = nullptr;
    }
}

int read_list(struct sl_list* list) {
    static struct sl_list* prev_list;
    int data;
    if(list != nullptr) {
        prev_list = list;
    }
    if(prev_list != nullptr) {
        data = prev_list->data;
        prev_list = prev_list->next;
        return data;
    }
    return -1;
}

void free_list(struct sl_list* list) {
    struct sl_list* next;
    while(list != nullptr) {
        next = list->next;
        free(list);
        list = next;
    }
}

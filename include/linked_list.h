#ifndef ND_LINKED_LIST
#define ND_LINKED_LIST

typedef int (*compare_data_cb)(const void* elem_1, const void* elem_2);
typedef int (*clean_data_cb)(void* elem_1);

typedef struct linked_list_sample{
  void* data;
  int value_size;
  struct linked_list_sample* next;
} linked_list_sample;

typedef struct{
    linked_list_sample* head;
    int size;
    compare_data_cb compare_samples;
    clean_data_cb clean_sample;
} linked_list;

linked_list* linked_list_init(compare_data_cb compare_function, clean_data_cb clean_sample);
int linked_list_cleanup(linked_list** list);
int linked_list_insert(linked_list *list, const void* value, int value_size);
int linked_list_remove(linked_list *list, const void* value);
void *linked_list_find(linked_list *list, const void *value);
int linked_list_size(linked_list *list);
void* linked_list_at(linked_list *list, unsigned int index);

#endif

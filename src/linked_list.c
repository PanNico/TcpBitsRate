#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int linked_list_size(linked_list *list) {
  if (!list)
    return -1;

  return list->size;
}

int default_clean_sample(void* elem_1){
  free(elem_1);
  return 0;
}

linked_list *linked_list_init(compare_data_cb compare_function, clean_data_cb clean_sample) {
  if(!compare_function)
    return NULL;
    
  linked_list *to_ret = malloc(sizeof(linked_list));
  if (!to_ret)
    return NULL;

  to_ret->head = NULL;
  to_ret->size = 0;
  to_ret->compare_samples = compare_function;
  to_ret->clean_sample = clean_sample ? clean_sample : default_clean_sample;
  
  return to_ret;
}

static linked_list_sample *create_sample(const void *value, int value_size) {
  linked_list_sample *tmp = malloc(sizeof(linked_list_sample));

  if (!tmp)
    return NULL;

  tmp->data = malloc(value_size);

  if (!tmp->data) {
    free(tmp);
    return NULL;
  }

  memcpy(tmp->data, value, value_size);
  tmp->value_size = value_size;
  tmp->next = NULL;
  return tmp;
}

int linked_list_insert(linked_list *list, const void *value, int value_size) {
  if (!list || !value || !value_size)
    return -2;

  linked_list_sample *tmp = create_sample(value, value_size);

  if (!tmp) {
    return -1;
  }

  struct linked_list_sample *old_head = list->head;

  list->head = tmp;
  list->head->next = old_head;
  list->size++;

  return 0;
}

static int _find(linked_list *list, const void *value,
                 linked_list_sample **sample_found,
                 linked_list_sample **parent) {
  int status = -1;
  linked_list_sample *previous = NULL;
  linked_list_sample *current = list->head;

  for (int i = 0; i < list->size; i++) {
    if (!list->compare_samples(current->data, value)) {
      *sample_found = current;
      if (parent)
        *parent = previous;
      status = 0;
      break;
    }

    previous = current;
    current = current->next;
  }

  return status;
}

void *linked_list_find(linked_list *list, const void *value) {
  if (!list || !value)
    return NULL;

  linked_list_sample *to_find;
  if (!_find(list, value, &to_find, NULL))
    return to_find->data;

  return NULL;
}

int linked_list_remove(linked_list *list, const void *value) {
  if (!list || !value)
    return -2;

  linked_list_sample *to_find, *parent;
  if (_find(list, value, &to_find, &parent))
    return -1;

  if (parent)
    parent->next = to_find->next;
  else
    list->head = to_find->next;

  int status=list->clean_sample(to_find->data);
  free(to_find);
  list->size--;

  return status;
}

void *linked_list_at(linked_list *list, unsigned int index) {

  if (!list || !list->head)
    return NULL;

  linked_list_sample *to_ret = list->head;

  for (int i = 0; i < index; i++) {
    to_ret = to_ret->next;
    if (!to_ret)
      return NULL;
  }

  return to_ret->data;
}

int linked_list_cleanup(linked_list **list) {
  if (!list || !*list)
    return -2;

  linked_list *list_to_del = *list;

  if (!list_to_del->head)
    goto free_list;

  linked_list_sample *current_value = list_to_del->head->next;
  linked_list_sample *next = NULL;

  while (current_value) {
    next = current_value->next;
    list_to_del->clean_sample(current_value->data);
    free(current_value);
    current_value = next;
  }

  list_to_del->clean_sample(list_to_del->head->data);
  free(list_to_del->head);

free_list:
  free(*list);
  *list = NULL;
  return 0;
}

#pragma once

#include "ctypes.h"

/*
 * PlantOS LinkedList
 * Copyright 2024 by min0911Y & copi143 & zhouzhihao
 */

typedef struct list *list_t;
struct list {
    union {
        void *data;
        isize idata;
        usize udata;
    };
    list_t prev;
    list_t next;
};

list_t list_alloc(void *data);
list_t list_free(list_t list);
list_t list_free_with(list_t list, free_t free_data);
list_t list_append(list_t list, void *data);
list_t list_head(list_t list);
list_t list_tail(list_t list);
list_t list_nth(list_t list, size_t n);
list_t list_nth_last(list_t list, size_t n);
list_t list_prepend(list_t list, void *data);
bool list_search(list_t list, void *data);
list_t list_delete(list_t list, void *data);
list_t list_delete_node(list_t list, list_t node);
size_t list_length(list_t list);
void *list_pop(list_t *list_p);
void list_print(list_t list);

#define list_push(list, data) list_append(list, data)

#define list_popi(list) ((usize)list_pop(list))
#define list_popu(list) ((isize)list_pop(list))


#define list_foreach(list, node) for (list_t node = (list); node; node = node->next)

#define list_foreach_cnt(list, i, node, code)                                                      \
  ({                                                                                               \
    size_t i = 0;                                                                                  \
    for (list_t node = (list); node; (node) = (node)->next, (i)++)                                 \
      (code);                                                                                      \
  })

#define list_first_node(list, node, expr)                                                          \
  ({                                                                                               \
    list_t _match_ = NULL;                                                                         \
    for (list_t node = (list); node; node = node->next) {                                          \
      if ((expr)) {                                                                                \
        _match_ = node;                                                                            \
        break;                                                                                     \
      }                                                                                            \
    }                                                                                              \
    _match_;                                                                                       \
  })

#define list_first(list, _data_, expr)                                                             \
  ({                                                                                               \
    void *_match_ = NULL;                                                                          \
    for (list_t node = (list); node; node = node->next) {                                          \
      void *_data_ = node->data;                                                                   \
      if ((expr)) {                                                                                \
        _match_ = _data_;                                                                          \
        break;                                                                                     \
      }                                                                                            \
    }                                                                                              \
    _match_;                                                                                       \
  })
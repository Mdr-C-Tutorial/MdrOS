#include "list.h"
#include "kmalloc.h"

list_t list_alloc(void *data) {
    list_t node = kmalloc(sizeof(*node));
    if (node == NULL) return NULL;
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

list_t list_free(list_t list) {
    while (list != NULL) {
        list_t next = list->next;
        kfree(list);
        list = next;
    }
    return NULL;
}

list_t list_free_with(list_t list, void (*free_data)(void *)) {
    while (list != NULL) {
        list_t next = list->next;
        free_data(list->data);
        kfree(list);
        list = next;
    }
    return NULL;
}

list_t list_append(list_t list, void *data) {
    list_t node = list_alloc(data);
    if (node == NULL) return list;

    if (list == NULL) {
        list = node;
    } else {
        list_t current = list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
        node->prev    = current;
    }

    return list;
}

list_t list_prepend(list_t list, void *data) {
    list_t node = list_alloc(data);
    if (node == NULL) return list;

    node->next = list;
    if (list != NULL) list->prev = node;
    list = node;

    return list;
}

void *list_pop(list_t *list_p) {
    if (list_p == NULL || *list_p == NULL) return NULL;
    list_t list = list_tail(*list_p);
    if (*list_p == list) *list_p = list->prev;
    if (list->prev) list->prev->next = NULL;
    void* data = list->data;
    kfree(list);
    return data;
}

list_t list_head(list_t list) {
    if (list == NULL) return NULL;
    for (; list->prev; list = list->prev) {}
    return list;
}

list_t list_tail(list_t list) {
    if (list == NULL) return NULL;
    for (; list->next; list = list->next) {}
    return list;
}

list_t list_nth(list_t list, size_t n) {
    if (list == NULL) return NULL;
    list = list_head(list);
    for (size_t i = 0; i < n; i++) {
        list = list->next;
        if (list == NULL) return NULL;
    }
    return list;
}

list_t list_nth_last(list_t list, size_t n) {
    if (list == NULL) return NULL;
    list = list_tail(list);
    for (size_t i = 0; i < n; i++) {
        list = list->prev;
        if (list == NULL) return NULL;
    }
    return list;
}

bool list_search(list_t list, void *data) {
    list_t current = list;
    while (current != NULL) {
        if (current->data == data) return true;
        current = current->next;
    }
    return false;
}

list_t list_delete(list_t list, void *data) {
    if (list == NULL) return NULL;

    if (list->data == data) {
        list_t temp = list;
        list        = list->next;
        kfree(temp);
        return list;
    }

    for (list_t current = list->next; current; current = current->next) {
        if (current->data == data) {
            current->prev->next = current->next;
            if (current->next != NULL) current->next->prev = current->prev;
            kfree(current);
            break;
        }
    }

    return list;
}

list_t list_delete_with(list_t list, void *data, free_t callback) {
    if (list == NULL) return NULL;

    if (list->data == data) {
        list_t temp = list;
        list        = list->next;
        if (callback) callback(temp->data);
        kfree(temp);
        return list;
    }

    for (list_t current = list->next; current; current = current->next) {
        if (current->data == data) {
            current->prev->next = current->next;
            if (current->next != NULL) current->next->prev = current->prev;
            if (callback) callback(current->data);
            kfree(current);
            break;
        }
    }

    return list;
}

list_t list_delete_node(list_t list, list_t node) {
    if (list == NULL || node == NULL) return list;

    if (list == node) {
        list_t temp = list;
        list        = list->next;
        kfree(temp);
        return list;
    }

    node->prev->next = node->next;
    if (node->next != NULL) node->next->prev = node->prev;
    kfree(node);
    return list;
}

list_t list_delete_node_with(list_t list, list_t node, free_t callback) {
    if (list == NULL || node == NULL) return list;

    if (list == node) {
        list_t temp = list;
        list        = list->next;
        if (callback) callback(temp->data);
        kfree(temp);
        return list;
    }

    node->prev->next = node->next;
    if (node->next != NULL) node->next->prev = node->prev;
    if (callback) callback(node->data);
    kfree(node);
    return list;
}

size_t list_length(list_t list) {
    size_t count   = 0;
    list_t current = list;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

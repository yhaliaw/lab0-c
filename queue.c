#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *l = malloc(sizeof(struct list_head));
    if (!l)
        return NULL;
    INIT_LIST_HEAD(l);
    return l;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;

    element_t *ei, *safe;
    list_for_each_entry_safe (ei, safe, l, list) {
        q_release_element(ei);
    }

    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *e = malloc(sizeof(element_t));
    if (!e)
        return false;

    e->value = strdup(s);
    if (!e->value) {
        free(e);
        return false;
    }

    list_add(&e->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *e = malloc(sizeof(element_t));
    if (!e)
        return false;

    e->value = strdup(s);
    if (!e->value) {
        free(e);
        return false;
    }

    list_add_tail(&e->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *e = list_first_entry(head, element_t, list);
    list_del(&e->list);
    if (sp) {
        strncpy(sp, e->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return e;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *e = list_last_entry(head, element_t, list);
    list_del(&e->list);
    if (sp) {
        strncpy(sp, e->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return e;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    struct list_head *slow = head->next;
    struct list_head *fast = head->next;
    while (fast->next != head && fast->next->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    list_del(slow);
    q_release_element(list_entry(slow, element_t, list));

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head)
        return false;

    bool dup = false;
    char *v = NULL;
    struct list_head *prev;
    element_t *ei, *safe;
    list_for_each_entry_safe (ei, safe, head, list) {
        if (v && strcmp(v, ei->value) == 0) {
            list_del(&ei->list);
            q_release_element(ei);
            dup = true;
        } else {
            v = ei->value;
            if (dup) {
                prev = ei->list.prev;
                list_del(prev);
                q_release_element(list_entry(prev, element_t, list));
            }
            dup = false;
        }
    }

    if (dup) {
        prev = ei->list.prev;
        list_del(prev);
        q_release_element(list_entry(prev, element_t, list));
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    for (struct list_head *node = head->next;
         node && node != head && node->next != head; node = node->next)
        list_move(node, node->next);
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *li, *safe;
    list_for_each_safe (li, safe, head)
        list_move(li, head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

void _merge(struct list_head **l, struct list_head *l1, struct list_head *l2)
{
    for (struct list_head **node = NULL; l1 && l2; *node = (*node)->next) {
        node = strcmp(list_entry(l1, element_t, list)->value,
                      list_entry(l2, element_t, list)->value) < 0
                   ? &l1
                   : &l2;
        *l = *node;
        l = &(*l)->next;
    }
    *l = l1 ? l1 : l2;
}

void _merge_sort(struct list_head **l)
{
    if (!*l || !(*l)->next)
        return;

    struct list_head *slow = *l;
    for (struct list_head *fast = *l; fast && fast->next;
         fast = fast->next->next)
        slow = slow->next;

    slow->prev->next = NULL;

    _merge_sort(l);
    _merge_sort(&slow);

    _merge(l, *l, slow);
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    head->prev->next = NULL;

    _merge_sort(&head->next);

    head->next->prev = head;

    struct list_head *prev = head;
    for (struct list_head *node = head->next; node; node = node->next) {
        node->prev = prev;
        prev = node;
    }
    head->prev = prev;
    prev->next = head;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}

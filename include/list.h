#ifndef _LIST_H
#define _LIST_H

/** \file list.h
 * A list structure and its tool methods.
 */

/* ***** INCLUDES ***** */
#include <types.h>

/* ***** TYPES ***** */
/**
 * A type to manage circular lists.
 */
typedef struct _list_t {
    struct _list_t *next;   ///< Next element in the list.
    struct _list_t *prev;   ///< Previous element in the list.
} list_t;

/* ***** PUBLIC METHODS ***** */
/**
 * @brief Determine the offset of a member of one structure
 *                    Imported from the Linux kernel
 * @param type        The type to be used to determine the offset
 * @param member      The member to determine the offset in the structure of type #type
 */
#define offset_of(type, member) ((size_t)((type *)0)->member)

/**
 * @brief Determine the address of the container of the given ptr address
 *                    Imported from the Linux kernel
 * @param ptr         The ptr which is contained in a structure
 * @param type        The type of the structure containing #ptr
 * @param member      The member which is pointed by ptr
 */
#define container_of(ptr, type, member) ({                 \
    const typeof(((type *)0)->member)*__mptr = (ptr);      \
    (type *)((char *)__mptr - offset_of(type, member)); })

/**
 * @brief Initialize a new circular list.
 * @param l           The list to initialize
 * @return error status
 */
static inline int32_t list_init(list_t *l)
{
    if (l == NULL)
        return -1;

    l->next = l;
    l->prev = l;

    return 0;
}

/**
 * @brief Insert a new element #elt in the circular list #l
 * @param elt         The element to insert in the list #l
 * @param l           The list to be used
 * @return error status
 */
static inline int32_t list_insert(list_t *elt, list_t* l)
{
    if (l == NULL)
        return -1;
    if (elt == NULL)
        return -1;

    elt->next = l->next;
    elt->prev = l;
    l->next = elt;
    if (l->prev == l)
        l->prev = elt;

    return 0;
}

/**
 * @brief Remove an element #elt from the circular list it is belonging
 * @param elt         The element to remove from the list #l
 * @return error status
 */
static inline int32_t list_remove(list_t *elt)
{
    if (elt == NULL)
        return -1;

    if (elt->next != elt->prev) {
        elt->next->prev = elt->prev;
        elt->prev->next = elt->next;

        elt->next = elt;
        elt->prev = elt;
    }

    return 0;
}

/**
 * @brief Parse the circular list #l
 * @param pos       The current element
 * @param l         The list to parse
 */
#define list_foreach(pos, l) for ((pos) = (l) ; (pos) != (l) ; (pos) = (pos)->next)

/**
 * @brief Retrieve the element from its list pointer
 * @param elt       The current element
 * @param type      The type of the element to retrieve
 * @param member    The name of the list in the containing structure
 */
#define list_entry(elt, type, member) container_of((elt), (type), (member))

#endif /* ! _LIST_H */

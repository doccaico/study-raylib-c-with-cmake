#ifndef YAC_DYNAMIC_ARRAY_H_
#define YAC_DYNAMIC_ARRAY_H_

#ifndef YAC_DYNAMIC_ARRAY_ASSERT
#include <assert.h>
#define YAC_DYNAMIC_ARRAY_ASSERT assert
#endif // YAC_DYNAMIC_ARRAY_ASSERT

#ifndef YAC_DYNAMIC_ARRAY_REALLOC
#include <stdlib.h>
#define YAC_DYNAMIC_ARRAY_REALLOC realloc
#endif // YAC_DYNAMIC_ARRAY_REALLOC

#ifndef YAC_DYNAMIC_ARRAY_FREE
#include <stdlib.h>
#define YAC_DYNAMIC_ARRAY_FREE free
#endif // YAC_DYNAMIC_ARRAY_FREE


#include <string.h> // memcpy


// Dynamic array works on any struct with *items, len and capacity fields

// Grow factor of the dynamic array.
#ifndef YAC_DYNAMIC_ARRAY_GROW_FACTOR
#define YAC_DYNAMIC_ARRAY_GROW_FACTOR 2
#endif

// Initial capacity of the dynamic array.
#ifndef YAC_DYNAMIC_ARRAY_INIT_CAP
#define YAC_DYNAMIC_ARRAY_INIT_CAP 8
#endif


#define YacDynamicArrayReserve(da, expected_capacity)                                                     \
    do {                                                                                                  \
        if ((expected_capacity) > (da)->capacity) {                                                       \
            if ((da)->capacity == 0) {                                                                    \
                (da)->capacity = YAC_DYNAMIC_ARRAY_INIT_CAP;                                              \
            }                                                                                             \
            while ((expected_capacity) > (da)->capacity) {                                                \
                (da)->capacity *= YAC_DYNAMIC_ARRAY_GROW_FACTOR;                                          \
            }                                                                                             \
            (da)->items = YAC_DYNAMIC_ARRAY_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items));  \
            YAC_DYNAMIC_ARRAY_ASSERT((da)->items != NULL && "could not allocate memory");                 \
        }                                                                                                 \
    } while (0)

// Append an item to the dynamic array.
#define YacDynamicArrayAppend(da, item)               \
    do {                                              \
        YacDynamicArrayReserve((da), (da)->len + 1);  \
        (da)->items[(da)->len++] = (item);            \
    } while (0)

// Insert item at index i.
#define YacDynamicArrayInsert(da, i, item)                                \
    do {                                                                  \
        YAC_DYNAMIC_ARRAY_ASSERT((i) < (da)->len + 1 && "out of range");  \
        YacDynamicArrayReserve((da), (da)->len + 1);                      \
        for (size_t _i = (da)->len; _i > (i); --_i) {                     \
            (da)->items[_i] = (da)->items[_i - 1];                        \
        }                                                                 \
        (da)->items[i] = (item);                                          \
        (da)->len += 1;                                                   \
    } while (0)

// Invalidates all element pointers.
#define YacDynamicArrayClearAndFree(da)                                      \
    do {                                                                     \
        YAC_DYNAMIC_ARRAY_ASSERT((da).items != NULL && "da.items is NULL");  \
        YAC_DYNAMIC_ARRAY_FREE((da).items);                                  \
        (da).items = NULL;                                                   \
        (da).len = 0;                                                        \
        (da).capacity = 0;                                                   \
    } while (0)

// Invalidates all element pointers, but retaining capacity.
#define YacDynamicArrayClearRetainingCapacity(da) (da).len = 0

// Whether items is NULL or not.
#define YacDynamicArrayIsNull(da) ((da).items == NULL)

// Append several items to the dynamic array.
#define YacDynamicArrayAppendMany(da, new_items, new_items_len)                              \
    do {                                                                                     \
        YacDynamicArrayReserve((da), (da)->len + (new_items_len));                           \
        memcpy((da)->items + (da)->len, (new_items), (new_items_len)*sizeof(*(da)->items));  \
        (da)->len += (new_items_len);                                                        \
    } while (0)

// Returns the last element from the dynamic array. Asserts that the dynamic array is not empty.
#define YacDynamicArrayGetLast(da) \
    (da)->items[(YAC_DYNAMIC_ARRAY_ASSERT((da)->len > 0 && "da->items is empty"), (da)->len-1)]

// Returns the last element from the dynamic array, or null if dynamic array is empty.
#define YacDynamicArraygetLastOrNull(da) ((da)->len == 0) ? NULL : &(da)->items[(da)->len-1]

// Remove and return the last element from the dynamic array, or return null if dynamic array is empty.
// Invalidates element pointers to the removed element, if any.
#define YacDynamicArrayPop(da) (((da)->len == 0) ? NULL : &(da)->items[--((da)->len)])

#define YacDynamicArrayRemove(da, i)                                                                    \
    do {                                                                                                \
        YAC_DYNAMIC_ARRAY_ASSERT((i) < (da)->len && "out of range");                                    \
        memmove((da)->items + (i), (da)->items + (i) + 1, ((da)->len - (i) - 1)*sizeof(*(da)->items));  \
        (da)->len--;                                                                                    \
    } while(0)

// Also known as: swap remove
#define YacDynamicArrayRemoveUnordered(da, i)                         \
    do {                                                              \
        YAC_DYNAMIC_ARRAY_ASSERT((i) < (da)->len && "out of range");  \
        (da)->items[i] = (da)->items[--(da)->len];                    \
    } while(0)


#endif // YAC_DYNAMIC_ARRAY_H_

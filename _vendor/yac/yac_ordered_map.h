// The MIT License (MIT)
//
// Original work Copyright (C) 2016 ZongXian Shen <andy.zsshen@gmail.com>
//   :: https://github.com/ZSShen/C-Common-Data-Structures
// Modified work Copyright (C) 2025 Doccaico
//   :: https://github.com/doccaico/yac
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.


#ifndef YAC_ORDERED_MAP_H_
#define YAC_ORDERED_MAP_H_

#include <stdbool.h> // bool

#ifndef YAC_ORDERED_MAP_API
#ifdef YAC_ORDERED_MAP_STATIC
#define YAC_ORDERED_MAP_API static
#else
#define YAC_ORDERED_MAP_API extern
#endif // YAC_ORDERED_MAP_STATIC
#endif // YAC_ORDERED_MAP_API


// The key value pair for associative data structures.
typedef struct _YacOrderedMapPair {
    void* key;
    void* value;
} YacOrderedMapPair;


// YacOrderedMapData is the data type for the container private information.
typedef struct _YacOrderedMapData YacOrderedMapData;

// Compare the equality of two keys.
typedef int (*YacOrderedMapCompare) (void*, void*);

// Key cleanup function called whenever a live entry is removed.
typedef void (*YacOrderedMapCleanKey) (void*);

// Value cleanup function called whenever a live entry is removed.
typedef void (*YacOrderedMapCleanValue) (void*);


// The implementation for ordered map.
typedef struct _YacOrderedMap {
    // The container private information
    YacOrderedMapData* data;

    // Insert a key value pair into the map. @see YacOrderedMapPut.
    bool (*put) (struct _YacOrderedMap*, void*, void*);

    // Retrieve the value corresponding to the designated key. @see YacOrderedMapGet.
    void* (*get) (struct _YacOrderedMap*, void*);

    // Check if the map contains the designated key. @see YacOrderedMapFind.
    bool (*find) (struct _YacOrderedMap*, void*);

    // Delete the key value pair corresponding to the designated key. @see YacOrderedMapRemove.
    bool (*remove) (struct _YacOrderedMap*, void*);

    // Return the number of stored key value pairs. @see YacOrderedMapSize.
    unsigned (*size) (struct _YacOrderedMap*);

    // Retrieve the key value pair with the minimum order from the map. @see YacOrderedMapMinimum.
    YacOrderedMapPair* (*minimum) (struct _YacOrderedMap*);

    // Retrieve the key value pair with the maximum order from the map. @see YacOrderedMapMaximum.
    YacOrderedMapPair* (*maximum) (struct _YacOrderedMap*);

    // Retrieve the key value pair which is the predecessor of the given key. @see YacOrderedMapPredecessor.
    YacOrderedMapPair* (*predecessor) (struct _YacOrderedMap*, void*);

    // Retrieve the key value pair which is the successor of the given key. @see YacOrderedMapSuccessor.
    YacOrderedMapPair* (*successor) (struct _YacOrderedMap*, void*);

    // Initialize the map iterator. @see YacOrderedMapFirst.
    void (*first) (struct _YacOrderedMap*);

    // Get the key value pair pointed by the iterator and advance the iterator. @see YacOrderedMapNext.
    YacOrderedMapPair* (*next) (struct _YacOrderedMap*);

    // Get the key value pair pointed by the iterator and advance the iterator in the reverse order.
    // @see YacOrderedMapNext.
    YacOrderedMapPair* (*reverse_next) (struct _YacOrderedMap*);

    // Set the custom key comparison function. @see YacOrderedMapSetCompare.
    void (*set_compare) (struct _YacOrderedMap*, YacOrderedMapCompare);

    // Set the custom key cleanup function. @see YacOrderedMapSetCleanKey.
    void (*set_clean_key) (struct _YacOrderedMap*, YacOrderedMapCleanKey);

    // Set the custom value cleanup function. @see YacOrderedMapSetCleanValue.
    void (*set_clean_value) (struct _YacOrderedMap*, YacOrderedMapCleanValue);
} YacOrderedMap;


//
// Definition for the exported member operations
//

// The constructor for YacOrderedMap.
YAC_ORDERED_MAP_API YacOrderedMap* YacOrderedMapInit(void);

// The destructor for YacOrderedMap.
YAC_ORDERED_MAP_API void YacOrderedMapDeinit(YacOrderedMap* obj);

// Insert a key value pair into the map.
// This function inserts a key value pair into the map. If the designated key is
// equal to a certain one stored in the map, the existing pair will be replaced.
// Also, the cleanup functions are invoked for that replaced pair.
YAC_ORDERED_MAP_API bool YacOrderedMapPut(YacOrderedMap* self, void* key, void* value);

// Retrieve the value corresponding to the designated key.
YAC_ORDERED_MAP_API void* YacOrderedMapGet(YacOrderedMap* self, void* key);

// Check if the map contains the designated key.
YAC_ORDERED_MAP_API bool YacOrderedMapFind(YacOrderedMap* self, void* key);

// Remove the key value pair corresponding to the designated key.
// This function removes the key value pair corresponding to the designated key.
// Also, the cleanup functions are invoked for that removed pair.
YAC_ORDERED_MAP_API bool YacOrderedMapRemove(YacOrderedMap* self, void* key);

// Return the number of stored key value pairs.
YAC_ORDERED_MAP_API unsigned YacOrderedMapSize(YacOrderedMap* self);

// Retrieve the key value pair with the minimum order from the map.
YAC_ORDERED_MAP_API YacOrderedMapPair* YacOrderedMapMinimum(YacOrderedMap* self);

// Retrieve the key value pair with the maximum order from the map.
YAC_ORDERED_MAP_API YacOrderedMapPair* YacOrderedMapMaximum(YacOrderedMap* self);

// Retrieve the key value pair which is the predecessor of the given key.
YAC_ORDERED_MAP_API YacOrderedMapPair* YacOrderedMapPredecessor(YacOrderedMap* self, void* key);

// Retrieve the key value pair which is the successor of the given key.
YAC_ORDERED_MAP_API YacOrderedMapPair* YacOrderedMapSuccessor(YacOrderedMap* self, void* key);

// Initialize the map iterator.
YAC_ORDERED_MAP_API void YacOrderedMapFirst(YacOrderedMap* self);

// Get the key value pair pointed by the iterator and advance the iterator.
YAC_ORDERED_MAP_API YacOrderedMapPair* YacOrderedMapNext(YacOrderedMap* self);

// Get the key value pair pointed by the iterator and advance the iterator in the reverse order.
YAC_ORDERED_MAP_API YacOrderedMapPair* YacOrderedMapReverseNext(YacOrderedMap* self);

// Set the custom key comparison function. By default, key is treated as integer.
YAC_ORDERED_MAP_API void YacOrderedMapSetCompare(YacOrderedMap* self, YacOrderedMapCompare func);

// Set the custom key cleanup function. By default, no cleanup operation for key.
YAC_ORDERED_MAP_API void YacOrderedMapSetCleanKey(YacOrderedMap* self, YacOrderedMapCleanKey func);

// Set the custom value cleanup function. By default, no cleanup operation for value.
YAC_ORDERED_MAP_API void YacOrderedMapSetCleanValue(YacOrderedMap* self, YacOrderedMapCleanValue func);


#endif // YAC_ORDERED_MAP_H_


//
// IMPLEMENTATION
//

#ifdef YAC_ORDERED_MAP_IMPLEMENTATION


#include <stdlib.h> // malloc, free

#ifndef YAC_ORDERED_MAP_MALLOC
#define YAC_ORDERED_MAP_MALLOC malloc
#endif

#ifndef YAC_ORDERED_MAP_FREE
#define YAC_ORDERED_MAP_FREE free
#endif


typedef struct _TreeNode {
    char color_;
    YacOrderedMapPair pair_;
    struct _TreeNode* parent_;
    struct _TreeNode* left_;
    struct _TreeNode* right_;
} TreeNode;

struct _YacOrderedMapData {
    char iter_direct_;
    int size_;
    TreeNode* root_;
    TreeNode* null_;
    TreeNode* iter_node_;
    YacOrderedMapCompare func_cmp_;
    YacOrderedMapCleanKey func_clean_key_;
    YacOrderedMapCleanValue func_clean_val_;
};


//
// Definition for internal operations
//

// Traverse all the tree nodes and clean the allocated resource.
static void YacOrderedMapDeinit_(YacOrderedMapData* data);

// Return the node having the maximal order in the subtree rooted by the ted node.
// The node order is determined by its stored key.
static TreeNode* YacOrderedMapMaximal_(TreeNode* null, TreeNode* curr);

// Return the node having the minimal order in the subtree rooted by the designated node.
// The node order is determined by its stored key.
static TreeNode* YacOrderedMapMinimal_(TreeNode* null, TreeNode* curr);

// Return the immediate successor of the designated node.
static TreeNode* YacOrderedMapSuccessor_(TreeNode* null, TreeNode* curr);

// Return the immediate predecessor of the designated node.
static TreeNode* YacOrderedMapPredecessor_(TreeNode* null, TreeNode* curr);

// Make right rotation for the subtree rooted by the designated node.
// After rotation, the designated node will be the right child of its original left child.
static void YacOrderedMapRightRotate_(YacOrderedMapData* data, TreeNode* curr);

// Make left rotation for the subtree rooted by the designated node.
// After rotation, the designated node will be the left child of its original right child.
static void YacOrderedMapLeftRotate_(YacOrderedMapData* data, TreeNode* curr);

// Maintain the red black tree property after node insertion.
static void YacOrderedMapInsertFixup_(YacOrderedMapData* data, TreeNode* curr);

// Maintain the red black tree property after node deletion.
static void YacOrderedMapDeleteFixup_(YacOrderedMapData* data, TreeNode* curr);

// Get the node which stores the key having the same order with the designated one.
static TreeNode* YacOrderedMapSearch_(YacOrderedMapData* data, void* key);

// The default hash key comparison function.
static int YacOrderedMapCompare_(void* lhs, void* rhs);


#define YAC_DIRECT_LEFT 0
#define YAC_DIRECT_RIGHT 1
#define YAC_COLOR_RED 0
#define YAC_COLOR_BLACK 1
#define YAC_STOP 0
#define YAC_DOWN_LEFT 1
#define YAC_DOWN_RIGHT 2
#define YAC_UP_LEFT 3
#define YAC_UP_RIGHT 4


//
// Implementation for the exported operations
//

YAC_ORDERED_MAP_API YacOrderedMap* YacOrderedMapInit(void)
{
    YacOrderedMap* obj = YAC_ORDERED_MAP_MALLOC (sizeof(YacOrderedMap));
    if (!obj)
        return NULL;

    YacOrderedMapData* data = YAC_ORDERED_MAP_MALLOC(sizeof(YacOrderedMapData));
    if (!data) {
        YAC_ORDERED_MAP_FREE(obj);
        return NULL;
    }

    // Create the dummy node representing the NULL pointer of the tree.
    TreeNode* null = YAC_ORDERED_MAP_MALLOC(sizeof(TreeNode));
    if (!null) {
        YAC_ORDERED_MAP_FREE(data);
        YAC_ORDERED_MAP_FREE(obj);
        return NULL;
    }

    null->color_ = YAC_COLOR_BLACK;
    null->parent_ = NULL;
    null->parent_ = null;
    null->right_ = null;
    null->left_ = null;

    data->size_ = 0;
    data->null_ = null;
    data->root_ = null;
    data->func_cmp_ = YacOrderedMapCompare_;
    data->func_clean_key_ = NULL;
    data->func_clean_val_ = NULL;

    obj->data = data;
    obj->put = YacOrderedMapPut;
    obj->get = YacOrderedMapGet;
    obj->find = YacOrderedMapFind;
    obj->remove = YacOrderedMapRemove;
    obj->size = YacOrderedMapSize;
    obj->minimum = YacOrderedMapMinimum;
    obj->maximum = YacOrderedMapMaximum;
    obj->predecessor = YacOrderedMapPredecessor;
    obj->successor = YacOrderedMapSuccessor;
    obj->first = YacOrderedMapFirst;
    obj->next = YacOrderedMapNext;
    obj->reverse_next = YacOrderedMapReverseNext;
    obj->set_compare = YacOrderedMapSetCompare;
    obj->set_clean_key = YacOrderedMapSetCleanKey;
    obj->set_clean_value = YacOrderedMapSetCleanValue;

    return obj;
}

YAC_ORDERED_MAP_API void YacOrderedMapDeinit(YacOrderedMap* obj)
{
    if (!obj)
        return;

    YacOrderedMapData* data = obj->data;
    YacOrderedMapDeinit_(data);
    YAC_ORDERED_MAP_FREE(data->null_);
    YAC_ORDERED_MAP_FREE(data);
    YAC_ORDERED_MAP_FREE(obj);
    return;
}

YAC_ORDERED_MAP_API bool YacOrderedMapPut(YacOrderedMap* self, void* key, void* value)
{
    TreeNode* node = YAC_ORDERED_MAP_MALLOC(sizeof(TreeNode));
    if (!node)
        return false;

    YacOrderedMapData* data = self->data;
    TreeNode* null = data->null_;
    node->pair_.key = key;
    node->pair_.value = value;
    node->color_ = YAC_COLOR_RED;
    node->parent_ = null;
    node->left_ = null;
    node->right_ = null;

    YacOrderedMapCompare func_cmp = data->func_cmp_;
    TreeNode* parent = null;
    TreeNode* curr = data->root_;
    char direct = 0;
    while (curr != null) {
        parent = curr;
        int order = func_cmp(key, curr->pair_.key);
        if (order > 0) {
            curr = curr->right_;
            direct = YAC_DIRECT_RIGHT;
        }
        else if (order < 0) {
            curr = curr->left_;
            direct = YAC_DIRECT_LEFT;
        }
        else {
            // Conflict with the already stored key value pair.
            YAC_ORDERED_MAP_FREE(node);
            if (data->func_clean_key_)
                data->func_clean_key_(curr->pair_.key);
            if (data->func_clean_val_)
                data->func_clean_val_(curr->pair_.value);

            curr->pair_.key = key;
            curr->pair_.value = value;
            return true;
        }
    }

    // Arrive at the proper position.
    node->parent_ = parent;
    if (parent != null) {
        if (direct == YAC_DIRECT_LEFT)
            parent->left_ = node;
        else
            parent->right_ = node;
    } else
        data->root_ = node;

    data->size_++;

    // Maintain the red black tree structure.
    YacOrderedMapInsertFixup_(data, node);

    return true;
}

YAC_ORDERED_MAP_API void* YacOrderedMapGet(YacOrderedMap* self, void* key)
{
    TreeNode* node = YacOrderedMapSearch_(self->data, key);
    if (node != self->data->null_)
        return node->pair_.value;
    return NULL;
}

YAC_ORDERED_MAP_API bool YacOrderedMapFind(YacOrderedMap* self, void* key)
{
    TreeNode* node = YacOrderedMapSearch_(self->data, key);
    return (node != self->data->null_)? true : false;
}

YAC_ORDERED_MAP_API bool YacOrderedMapRemove(YacOrderedMap* self, void* key)
{
    YacOrderedMapData* data = self->data;
    TreeNode* null = data->null_;
    TreeNode* curr = YacOrderedMapSearch_(data, key);
    if (curr == null)
        return false;

    TreeNode* child;
    char color;
    // The specified node has no child.
    if ((curr->left_ == null) && (curr->right_ == null)) {
        if (curr->parent_ != null) {
            if (curr == curr->parent_->left_)
                curr->parent_->left_ = null;
            else
                curr->parent_->right_ = null;
        } else
            data->root_ = null;

        color = curr->color_;
        child = null;
        child->parent_ = curr->parent_;
        if (data->func_clean_key_)
            data->func_clean_key_(curr->pair_.key);
        if (data->func_clean_val_)
            data->func_clean_val_(curr->pair_.value);
        YAC_ORDERED_MAP_FREE(curr);
    } else {
        // The specified node has two children.
        if ((curr->left_ != null) && (curr->right_ != null)) {
            TreeNode* succ = YacOrderedMapSuccessor_(null, curr);

            child = succ->left_;
            if (child == null)
                child = succ->right_;

            child->parent_ = succ->parent_;

            if (succ == succ->parent_->left_)
                succ->parent_->left_ = child;
            else
                succ->parent_->right_ = child;

            color = succ->color_;
            if (data->func_clean_key_)
                data->func_clean_key_(curr->pair_.key);
            if (data->func_clean_val_)
                data->func_clean_val_(curr->pair_.value);
            curr->pair_.key = succ->pair_.key;
            curr->pair_.value = succ->pair_.value;
            YAC_ORDERED_MAP_FREE(succ);
        }
        // The specified node has one child.
        else {
            child = curr->left_;
            if (child == null)
                child = curr->right_;

            child->parent_ = curr->parent_;

            if (curr->parent_ != null) {
                if (curr == curr->parent_->left_)
                    curr->parent_->left_ = child;
                else
                    curr->parent_->right_ = child;
            } else
                data->root_ = child;

            color = curr->color_;
            if (data->func_clean_key_)
                data->func_clean_key_(curr->pair_.key);
            if (data->func_clean_val_)
                data->func_clean_val_(curr->pair_.value);
            YAC_ORDERED_MAP_FREE(curr);
        }
    }

    // Decrease the size.
    data->size_--;

    // Maintain the balanced tree structure.
    if (color == YAC_COLOR_BLACK)
        YacOrderedMapDeleteFixup_(data, child);

    return true;
}

YAC_ORDERED_MAP_API unsigned YacOrderedMapSize(YacOrderedMap* self)
{
    return self->data->size_;
}

YAC_ORDERED_MAP_API YacOrderedMapPair* YacOrderedMapMinimum(YacOrderedMap* self)
{
    TreeNode* node = YacOrderedMapMinimal_(self->data->null_, self->data->root_);
    if (node != self->data->null_)
        return &(node->pair_);
    return NULL;
}

YAC_ORDERED_MAP_API YacOrderedMapPair* YacOrderedMapMaximum(YacOrderedMap* self)
{
    TreeNode* node = YacOrderedMapMaximal_(self->data->null_, self->data->root_);
    if (node != self->data->null_)
        return &(node->pair_);
    return NULL;
}

YAC_ORDERED_MAP_API YacOrderedMapPair* YacOrderedMapPredecessor(YacOrderedMap* self, void* key)
{
    TreeNode* curr = YacOrderedMapSearch_(self->data, key);
    if (curr == self->data->null_)
        return NULL;

    TreeNode* node = YacOrderedMapPredecessor_(self->data->null_, curr);
    if (node != self->data->null_)
        return &(node->pair_);

    return NULL;
}

YAC_ORDERED_MAP_API YacOrderedMapPair* YacOrderedMapSuccessor(YacOrderedMap* self, void* key)
{
    TreeNode* curr = YacOrderedMapSearch_(self->data, key);
    if (curr == self->data->null_)
        return NULL;

    TreeNode* node = YacOrderedMapSuccessor_(self->data->null_, curr);
    if (node != self->data->null_)
        return &(node->pair_);

    return NULL;
}

YAC_ORDERED_MAP_API void YacOrderedMapFirst(YacOrderedMap* self)
{
    self->data->iter_direct_ = YAC_DOWN_LEFT;
    self->data->iter_node_ = self->data->root_;
}

YAC_ORDERED_MAP_API YacOrderedMapPair* YacOrderedMapNext(YacOrderedMap* self)
{
    char direct = self->data->iter_direct_;
    TreeNode* null = self->data->null_;
    TreeNode* curr = self->data->iter_node_;

    while (curr != null) {
        if (direct == YAC_DOWN_LEFT || direct == YAC_DOWN_RIGHT) {
            if (curr->left_ != null) {
                curr = curr->left_;
                direct = YAC_DOWN_LEFT;
                continue;
            }

            YacOrderedMapPair* pair = &(curr->pair_);

            if (curr->right_ != null) {
                self->data->iter_node_ = curr->right_;
                self->data->iter_direct_ = YAC_DOWN_RIGHT;
            } else {
                TreeNode* temp = curr;
                curr = curr->parent_;
                self->data->iter_node_ = curr;
                if (curr != null) {
                    if (temp == curr->left_)
                        self->data->iter_direct_ = YAC_UP_LEFT;
                    else
                        self->data->iter_direct_ = YAC_UP_RIGHT;
                }
            }
            return pair;
        }

        if (direct == YAC_UP_LEFT) {
            YacOrderedMapPair* pair = &(curr->pair_);

            if (curr->right_ != null) {
                self->data->iter_node_ = curr->right_;
                self->data->iter_direct_ = YAC_DOWN_RIGHT;
            } else {
                TreeNode* temp = curr;
                curr = curr->parent_;
                self->data->iter_node_ = curr;
                if (curr != null) {
                    if (temp == curr->left_)
                        self->data->iter_direct_ = YAC_UP_LEFT;
                    else
                        self->data->iter_direct_ = YAC_UP_RIGHT;
                }
            }
            return pair;
        }

        TreeNode* temp = curr;
        curr = curr->parent_;
        if (curr != null) {
            if (temp == curr->left_)
                direct = YAC_UP_LEFT;
            else
                direct = YAC_UP_RIGHT;
        }
    }

    self->data->iter_node_ = null;
    return NULL;
}

YAC_ORDERED_MAP_API YacOrderedMapPair* YacOrderedMapReverseNext(YacOrderedMap* self)
{
    char direct = self->data->iter_direct_;
    TreeNode* null = self->data->null_;
    TreeNode* curr = self->data->iter_node_;

    while (curr != null) {
        if (direct == YAC_DOWN_LEFT || direct == YAC_DOWN_RIGHT) {
            if (curr->right_ != null) {
                curr = curr->right_;
                direct = YAC_DOWN_RIGHT;
                continue;
            }

            YacOrderedMapPair* pair = &(curr->pair_);

            if (curr->left_ != null) {
                self->data->iter_node_ = curr->left_;
                self->data->iter_direct_ = YAC_DOWN_LEFT;
            } else {
                TreeNode* temp = curr;
                curr = curr->parent_;
                self->data->iter_node_ = curr;
                if (curr != null) {
                    if (temp == curr->left_)
                        self->data->iter_direct_ = YAC_UP_LEFT;
                    else
                        self->data->iter_direct_ = YAC_UP_RIGHT;
                }
            }
            return pair;
        }

        if (direct == YAC_UP_RIGHT) {
            YacOrderedMapPair* pair = &(curr->pair_);

            if (curr->left_ != null) {
                self->data->iter_node_ = curr->left_;
                self->data->iter_direct_ = YAC_DOWN_LEFT;
            } else {
                TreeNode* temp = curr;
                curr = curr->parent_;
                self->data->iter_node_ = curr;
                if (curr != null) {
                    if (temp == curr->left_)
                        self->data->iter_direct_ = YAC_UP_LEFT;
                    else
                        self->data->iter_direct_ = YAC_UP_RIGHT;
                }
            }
            return pair;
        }

        TreeNode* temp = curr;
        curr = curr->parent_;
        if (curr != null) {
            if (temp == curr->left_)
                direct = YAC_UP_LEFT;
            else
                direct = YAC_UP_RIGHT;
        }
    }

    self->data->iter_node_ = null;
    return NULL;
}

YAC_ORDERED_MAP_API void YacOrderedMapSetCompare(YacOrderedMap* self, YacOrderedMapCompare func)
{
    self->data->func_cmp_ = func;
}

YAC_ORDERED_MAP_API void YacOrderedMapSetCleanKey(YacOrderedMap* self, YacOrderedMapCleanKey func)
{
    self->data->func_clean_key_ = func;
}

YAC_ORDERED_MAP_API void YacOrderedMapSetCleanValue(YacOrderedMap* self, YacOrderedMapCleanValue func)
{
    self->data->func_clean_val_ = func;
}


//
// Implementation for internal operations
//

static void YacOrderedMapDeinit_(YacOrderedMapData* data)
{
    TreeNode* null = data->null_;
    if (data->root_ == null)
        return;

    YacOrderedMapCleanKey func_clean_key = data->func_clean_key_;
    YacOrderedMapCleanValue func_clean_val = data->func_clean_val_;

    char direct = YAC_DOWN_LEFT;
    TreeNode* curr = data->root_;
    while (direct != YAC_STOP) {
        if (direct == YAC_DOWN_LEFT || direct == YAC_DOWN_RIGHT) {
            if (curr->left_ != null) {
                curr = curr->left_;
                direct = YAC_DOWN_LEFT;
                continue;
            }
            if (curr->right_ != null) {
                curr = curr->right_;
                direct = YAC_DOWN_RIGHT;
                continue;
            }

            TreeNode* temp = curr;
            curr = curr->parent_;
            if (curr == null)
                direct = YAC_STOP;
            else
                direct = (temp == curr->left_)? YAC_UP_LEFT : YAC_UP_RIGHT;

            if (func_clean_key)
                func_clean_key(temp->pair_.key);
            if (func_clean_val)
                func_clean_val(temp->pair_.value);
            YAC_ORDERED_MAP_FREE(temp);
            continue;
        }

        if (direct == YAC_UP_LEFT) {
            if (curr->right_ != null) {
                curr = curr->right_;
                direct = YAC_DOWN_RIGHT;
                continue;
            }

            TreeNode* temp = curr;
            curr = curr->parent_;
            if (curr == null)
                direct = YAC_STOP;
            else
                direct = (temp == curr->left_)? YAC_UP_LEFT : YAC_UP_RIGHT;

            if (func_clean_key)
                func_clean_key(temp->pair_.key);
            if (func_clean_val)
                func_clean_val(temp->pair_.value);
            YAC_ORDERED_MAP_FREE(temp);
            continue;
        }

        TreeNode* temp = curr;
        curr = curr->parent_;
        if (curr == null)
            direct = YAC_STOP;
        else
            direct = (temp == curr->left_)? YAC_UP_LEFT : YAC_UP_RIGHT;

        if (func_clean_key)
            func_clean_key(temp->pair_.key);
        if (func_clean_val)
            func_clean_val(temp->pair_.value);
        YAC_ORDERED_MAP_FREE(temp);
    }

    return;
}

static TreeNode* YacOrderedMapMaximal_(TreeNode* null, TreeNode* curr)
{
    TreeNode* parent = null;
    while (curr != null) {
        parent = curr;
        curr = curr->right_;
    }
    return parent;
}

static TreeNode* YacOrderedMapMinimal_(TreeNode* null, TreeNode* curr)
{
    TreeNode* parent = null;
    while (curr != null) {
        parent = curr;
        curr = curr->left_;
    }
    return parent;
}

static TreeNode* YacOrderedMapSuccessor_(TreeNode* null, TreeNode* curr)
{
    if (curr != null) {
        // Case 1: The minimal node in the non-null right subtree.
        if (curr->right_ != null)
            curr = YacOrderedMapMinimal_(null, curr->right_);

        // Case 2: The ancestor which considers the designated node as the maximal node of its left subtree.
        else {
            while((curr->parent_ != null) && (curr == curr->parent_->right_))
                curr = curr->parent_;
            curr = curr->parent_;
        }
    }
    return curr;
}

static TreeNode* YacOrderedMapPredecessor_(TreeNode* null, TreeNode* curr)
{
    if (curr != null) {
        // Case 1: The maximal node in the non-null left subtree.
        if (curr->left_ != null)
            curr = YacOrderedMapMaximal_(null, curr->left_);

        // Case 2: The ancestor which considers the designated node as the minimal node of its right subtree.
        else {
            while((curr->parent_ != null) && (curr == curr->parent_->left_))
                curr = curr->parent_;
            curr = curr->parent_;
        }
    }
    return curr;
}

static void YacOrderedMapRightRotate_(YacOrderedMapData* data, TreeNode* curr)
{
    TreeNode* null = data->null_;
    TreeNode* child = curr->left_;
    //
    //  Right rotation for the current node denoted as y
    //     y          x
    //    / \        / \
    //   x   c  =>  a   y
    //  / \            / \
    // a   b          b   c
    //
    //
    // Let y link b as its left child.
    // If b is not dummy node, let b link y as its parent.
    curr->left_ = child->right_;
    if (child->right_ != null)
        child->right_->parent_ = curr;

    // Let x link y's parent as its parent.
    // If y's parent is not dummy node, let it link x as its child.
    child->parent_ = curr->parent_;
    if (curr->parent_ != null) {
        if (curr == curr->parent_->left_)
            curr->parent_->left_ = child;
        else
            curr->parent_->right_ = child;
    } else
        data->root_ = child;

    // Let y link x as its parent. And let x link y as its right child.
    curr->parent_ = child;
    child->right_ = curr;

    return;
}

static void YacOrderedMapLeftRotate_(YacOrderedMapData* data, TreeNode* curr)
{
    TreeNode* null = data->null_;
    TreeNode* child = curr->right_;
    //
    //  Left rotation for the current node denoted as x
    //     x          y
    //    / \        / \
    //   a   y  =>  x   c
    //      / \    / \
    //     b   c  a   b
    //
    //
    // Let x link b as its right child.
    // If b is not dummy node, let b link x as its parent.
    curr->right_ = child->left_;
    if (child->left_ != null)
        child->left_->parent_ = curr;

    // Let y link x's parent as its parent.
    // If x's parent is not dummy node, let it link y as its child.
    child->parent_ = curr->parent_;
    if (curr->parent_ != null) {
        if (curr == curr->parent_->left_)
            curr->parent_->left_ = child;
        else
            curr->parent_->right_ = child;
    } else
        data->root_ = child;

    // Let x link y as its parent. And let y link x as its left child.
    curr->parent_ = child;
    child->left_ = curr;

    return;
}

static void YacOrderedMapInsertFixup_(YacOrderedMapData* data, TreeNode* curr)
{
    TreeNode* uncle;

    // Denote the current node as x.
    while (curr->parent_->color_ == YAC_COLOR_RED) {
        // x's parent is its grandparent's left child.
        if (curr->parent_ == curr->parent_->parent_->left_) {
            uncle = curr->parent_->parent_->right_;
            //
            // Case 1: The color of x's uncle is also red.
            // Set the colors of x's parent and x's uncle to black.
            // Set the color of x's grandparent to red.
            //
            //       g(B)               g(R)
            //      / \                / \
            //     /   \              /   \
            // (R)p     u(R)      (B)p     u(B)
            //   / \   / \    =>    / \   / \
            //  A   \ D   E        A   \ D   E
            //    (R)x               (R)x
            //      / \                / \
            //     B   C              B   C
            //
            if (uncle->color_ == YAC_COLOR_RED) {
                curr->parent_->color_ = YAC_COLOR_BLACK;
                uncle->color_ = YAC_COLOR_BLACK;
                curr->parent_->parent_->color_ = YAC_COLOR_RED;
                curr = curr->parent_->parent_;
            } else {
                //
                // Case 2: The color of x's uncle is black, and x is its parent's
                //         right child.
                // Make a left rotation for x's parent.
                //
                //        g(B)                g(B)
                //       / \                 / \
                //      /   \               /   \
                //  (R)p     u(B)       (R)x     u(B)
                //    / \   / \    =>     / \   / \
                //   A   \ D   E         /   C D   E
                //     (R)x          (R)p
                //       / \           / \
                //      B   C         A   B
                //
                if (curr == curr->parent_->right_) {
                    curr = curr->parent_;
                    YacOrderedMapLeftRotate_(data, curr);
                }
                //
                // Case 3: The color of x's uncle is black, and x is its parent's
                //         left child.
                // Set the color of x's parent to black.
                // Set the color of x's grandparent to red.
                // Make a right rotation for x's grandparent.
                //
                //          g(B)               p(B)
                //         / \                / \
                //        /   \              /   \
                //    (R)p     u(B)      (R)x     g(R)
                //      / \   / \          / \   / \
                //     /   C D   E   =>   A   B /   \
                // (R)x                        C     u(B)
                //   / \                            / \
                //  A   B                          D   E
                //
                curr->parent_->color_ = YAC_COLOR_BLACK;
                curr->parent_->parent_->color_ = YAC_COLOR_RED;
                YacOrderedMapRightRotate_(data, curr->parent_->parent_);
            }
        }
        // x's parent is its grandparent's right child.
        else {
            uncle = curr->parent_->parent_->left_;

            // Case 1: The color of x's uncle is also red.
            if (uncle->color_ == YAC_COLOR_RED) {
                curr->parent_->color_ = YAC_COLOR_BLACK;
                uncle->color_ = YAC_COLOR_BLACK;
                curr->parent_->parent_->color_ = YAC_COLOR_RED;
                curr = curr->parent_->parent_;
            } else {
                // Case 2: The color of x's uncle is black, and x is its parent's left child.
                if (curr == curr->parent_->left_) {
                    curr = curr->parent_;
                    YacOrderedMapRightRotate_(data, curr);
                }
                // Case 3: The color of x's uncle is black, and x is its parent's right child.
                curr->parent_->color_ = YAC_COLOR_BLACK;
                curr->parent_->parent_->color_ = YAC_COLOR_RED;
                YacOrderedMapLeftRotate_(data, curr->parent_->parent_);
            }
        }
    }

    data->root_->color_ = YAC_COLOR_BLACK;
    return;
}

static void YacOrderedMapDeleteFixup_(YacOrderedMapData* data, TreeNode* curr)
{
    TreeNode* brother;

    // Denote the current node as x.
    while ((curr != data->root_) && (curr->color_ == YAC_COLOR_BLACK)) {
        // x is its parent's left child.
        if (curr == curr->parent_->left_) {
            brother = curr->parent_->right_;
            //
            // Case 1: The color of x's brother is red.
            // Set the color of x's brother to black.
            // Set the color of x's parent to red.
            // Make a left rotation for x's parent.
            //
            //        p(B)                b(B)
            //      /   \               /   \
            //     x     b(R)  =>   (R)p     D
            //    / \   / \           / \
            //   A   B C   D         x   C
            //                      / \
            //                     A  B
            //
            if (brother->color_ == YAC_COLOR_RED) {
                brother->color_ = YAC_COLOR_BLACK;
                curr->parent_->color_ = YAC_COLOR_RED;
                YacOrderedMapLeftRotate_(data, curr->parent_);
                brother = curr->parent_->right_;
            }
            //
            // Case 2: The color of x's brother is black, and both of its
            //         children are also black.
            // Set the color of x's brother to red.
            //
            //         p                  p
            //       /   \              /   \
            //      /     \            /     \
            //     x       b(B)  =>   x       b(R)
            //    / \     / \        / \     / \
            //   A   B (B)y z(B)    A   B (B)y z(B)
            //
            if ((brother->left_->color_ == YAC_COLOR_BLACK) &&
                    (brother->right_->color_ == YAC_COLOR_BLACK)) {
                brother->color_ = YAC_COLOR_RED;
                curr = curr->parent_;
            } else {
                //
                // Case 3: The color of x's brother is black, and the colors of
                //         its left and right child are red and black respectively.
                // Set the color of x's brother to red.
                // Set the color of that brother's left child to black.
                // Make a right rotation for x's brother.
                //
                //         p                     p
                //       /   \                 /   \
                //      /     \               /     \
                //     x       b(B)          x       y(B)
                //    / \     / \     =>    / \     / \
                //   A   B (R)y z(B)       A   B   C   b(R)
                //           / \                      / \
                //          C   D                    D   z(B)
                //
                if (brother->right_->color_ == YAC_COLOR_BLACK) {
                    brother->left_->color_ = YAC_COLOR_BLACK;
                    brother->color_ = YAC_COLOR_RED;
                    YacOrderedMapRightRotate_(data, brother);
                    brother = curr->parent_->right_;
                }
                //
                // Case 4: The color of x's brother is black, and its right child
                //         is red.
                // Set the color of x's brother to the one of x's parent.
                // Set the color of x's parent to black.
                // Set the color of that brother's right child to black.
                // Make a left rotation for x's parent.
                //
                //       p(C')              b(C')
                //      /   \              /   \
                //     /     \            /     \
                //    x      b(B)   =>   p(B)   z(B)
                //   / \    / \         / \
                //  A   B  y   z(R)    x   y
                //                    / \
                //                   A   B
                //
                brother->color_ = curr->parent_->color_;
                curr->parent_->color_ = YAC_COLOR_BLACK;
                brother->right_->color_ = YAC_COLOR_BLACK;
                YacOrderedMapLeftRotate_(data, curr->parent_);
                curr = data->root_;
            }
        }
        // x is its parent's right child.
        else {
            brother = curr->parent_->left_;
            // Case 1: The color of x's brother is red.
            if (brother->color_ == YAC_COLOR_RED) {
                brother->color_ = YAC_COLOR_BLACK;
                curr->parent_->color_ = YAC_COLOR_RED;
                YacOrderedMapRightRotate_(data, curr->parent_);
                brother = curr->parent_->left_;
            }
            // Case 2: The color of x's brother is black, and both of its children are also black.
            if ((brother->left_->color_ == YAC_COLOR_BLACK) &&
                    (brother->right_->color_ == YAC_COLOR_BLACK)) {
                brother->color_ = YAC_COLOR_RED;
                curr = curr->parent_;
            } else {
                // Case 3: The color of x's brother is black and the colors of its
                // right and left child are red and black respectively.
                if (brother->left_->color_ == YAC_COLOR_BLACK) {
                    brother->right_->color_ = YAC_COLOR_BLACK;
                    brother->color_ = YAC_COLOR_RED;
                    YacOrderedMapLeftRotate_(data, brother);
                    brother = curr->parent_->left_;
                }
                // Case 4: The color of x's brother is black, and its left child is red.
                brother->color_ = curr->parent_->color_;
                curr->parent_->color_ = YAC_COLOR_BLACK;
                brother->left_->color_ = YAC_COLOR_BLACK;
                YacOrderedMapRightRotate_(data, curr->parent_);
                curr = data->root_;
            }
        }
    }

    curr->color_ = YAC_COLOR_BLACK;
    return;
}

static TreeNode* YacOrderedMapSearch_(YacOrderedMapData* data, void* key)
{
    YacOrderedMapCompare func_cmp = data->func_cmp_;
    TreeNode* null = data->null_;
    TreeNode* curr = data->root_;
    while(curr != null) {
        int order = func_cmp(key, curr->pair_.key);
        if (order == 0)
            break;
        else {
            if (order > 0)
                curr = curr->right_;
            else
                curr = curr->left_;
        }
    }
    return curr;
}

static int YacOrderedMapCompare_(void* lhs, void* rhs)
{
    if ((intptr_t)lhs == (intptr_t)rhs)
        return 0;
    return ((intptr_t)lhs >= (intptr_t)rhs)? 1 : (-1);
}


#endif // YAC_ORDERED_MAP_IMPLEMENTATION

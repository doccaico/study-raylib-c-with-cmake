// The MIT License (MIT)
// Original work Copyright (C) 2016 ZongXian Shen <andy.zsshen@gmail.com>
// Modified work Copyright (C) 2025 Doccaico
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

// Original work
// https://github.com/ZSShen/C-Common-Data-Structures

#ifndef YACC_MAP_H_
#define YACC_MAP_H_

#include <stdlib.h>
#include <stdbool.h>

#ifndef YACC_MAP_API
#ifdef YACC_MAP_STATIC
#define YACC_MAP_API static
#else
#define YACC_MAP_API extern
#endif // YACC_MAP_STATIC
#endif // YACC_MAP_API


// DECLARATIONS

// TreeMapData is the data type for the container private information.
typedef struct _TreeMapData TreeMapData;

// Compare the equality of two keys.
typedef int (*TreeMapCompare) (void*, void*);

// Key cleanup function called whenever a live entry is removed.
typedef void (*TreeMapCleanKey) (void*);

// Value cleanup function called whenever a live entry is removed.
typedef void (*TreeMapCleanValue) (void*);


// The key value pair for associative data structures.
typedef struct _Pair {
    void* key;
    void* value;
} Pair;


// The implementation for ordered map.
typedef struct _TreeMap {
    // The container private information
    TreeMapData *data;

    // Insert a key value pair into the map. @see TreeMapPut.
    bool (*put) (struct _TreeMap*, void*, void*);

    // Retrieve the value corresponding to the designated key. @see TreeMapGet.
    void* (*get) (struct _TreeMap*, void*);

    // Check if the map contains the designated key. @see TreeMapFind.
    bool (*find) (struct _TreeMap*, void*);

    // Delete the key value pair corresponding to the designated key. @see TreeMapRemove.
    bool (*remove) (struct _TreeMap*, void*);

    // Return the number of stored key value pairs. @see TreeMapSize.
    unsigned (*size) (struct _TreeMap*);

    // Retrieve the key value pair with the minimum order from the map. @see TreeMapMinimum.
    Pair* (*minimum) (struct _TreeMap*);

    // Retrieve the key value pair with the maximum order from the map. @see TreeMapMaximum.
    Pair* (*maximum) (struct _TreeMap*);

    // Retrieve the key value pair which is the predecessor of the given key. @see TreeMapPredecessor.
    Pair* (*predecessor) (struct _TreeMap*, void*);

    // Retrieve the key value pair which is the successor of the given key. @see TreeMapSuccessor.
    Pair* (*successor) (struct _TreeMap*, void*);

    // Initialize the map iterator. @see TreeMapFirst.
    void (*first) (struct _TreeMap*);

    // Get the key value pair pointed by the iterator and advance the iterator. @see TreeMapNext.
    Pair* (*next) (struct _TreeMap*);

    // Get the key value pair pointed by the iterator and advance the iterator in the reverse order.
    // @see TreeMapNext.
    Pair* (*reverse_next) (struct _TreeMap*);

    // Set the custom key comparison function. @see TreeMapSetCompare.
    void (*set_compare) (struct _TreeMap*, TreeMapCompare);

    // Set the custom key cleanup function. @see TreeMapSetCleanKey.
    void (*set_clean_key) (struct _TreeMap*, TreeMapCleanKey);

    // Set the custom value cleanup function. @see TreeMapSetCleanValue.
    void (*set_clean_value) (struct _TreeMap*, TreeMapCleanValue);
} TreeMap;

// The constructor for TreeMap.
YACC_MAP_API TreeMap* TreeMapInit(void);

// The destructor for TreeMap.
YACC_MAP_API void TreeMapDeinit(TreeMap* obj);

// Insert a key value pair into the map.
// This function inserts a key value pair into the map. If the designated key is
// equal to a certain one stored in the map, the existing pair will be replaced.
// Also, the cleanup functions are invoked for that replaced pair.
YACC_MAP_API bool TreeMapPut(TreeMap* self, void* key, void* value);

// Retrieve the value corresponding to the designated key.
YACC_MAP_API void* TreeMapGet(TreeMap* self, void* key);

// Check if the map contains the designated key.
YACC_MAP_API bool TreeMapFind(TreeMap* self, void* key);

// Remove the key value pair corresponding to the designated key.
// This function removes the key value pair corresponding to the designated key.
// Also, the cleanup functions are invoked for that removed pair.
YACC_MAP_API bool TreeMapRemove(TreeMap* self, void* key);

// Return the number of stored key value pairs.
YACC_MAP_API unsigned TreeMapSize(TreeMap* self);

// Retrieve the key value pair with the minimum order from the map.
YACC_MAP_API Pair* TreeMapMinimum(TreeMap* self);

// Retrieve the key value pair with the maximum order from the map.
YACC_MAP_API Pair* TreeMapMaximum(TreeMap* self);

// Retrieve the key value pair which is the predecessor of the given key.
YACC_MAP_API Pair* TreeMapPredecessor(TreeMap* self, void* key);

// Retrieve the key value pair which is the successor of the given key.
YACC_MAP_API Pair* TreeMapSuccessor(TreeMap* self, void* key);

// Initialize the map iterator.
YACC_MAP_API void TreeMapFirst(TreeMap* self);

// Get the key value pair pointed by the iterator and advance the iterator.
YACC_MAP_API Pair* TreeMapNext(TreeMap* self);

// Get the key value pair pointed by the iterator and advance the iterator in the reverse order.
YACC_MAP_API Pair* TreeMapReverseNext(TreeMap* self);

// Set the custom key comparison function. By default, key is treated as integer.
YACC_MAP_API void TreeMapSetCompare(TreeMap* self, TreeMapCompare func);

// Set the custom key cleanup function. By default, no cleanup operation for key.
YACC_MAP_API void TreeMapSetCleanKey(TreeMap* self, TreeMapCleanKey func);

// Set the custom value cleanup function. By default, no cleanup operation for value.
YACC_MAP_API void TreeMapSetCleanValue(TreeMap* self, TreeMapCleanValue func);


#endif // YACC_MAP_H_


// IMPLEMENTATION

#ifdef YACC_MAP_IMPLEMENTATION


typedef struct _TreeNode {
    char color_;
    Pair pair_;
    struct _TreeNode* parent_;
    struct _TreeNode* left_;
    struct _TreeNode* right_;
} TreeNode;

struct _TreeMapData {
    char iter_direct_;
    int size_;
    TreeNode* root_;
    TreeNode* null_;
    TreeNode* iter_node_;
    TreeMapCompare func_cmp_;
    TreeMapCleanKey func_clean_key_;
    TreeMapCleanValue func_clean_val_;
};


// Traverse all the tree nodes and clean the allocated resource.
void _TreeMapDeinit(TreeMapData* data);

// Return the node having the maximal order in the subtree rooted by the ted node.
// The node order is determined by its stored key.
TreeNode* _TreeMapMaximal(TreeNode* null, TreeNode* curr);

// Return the node having the minimal order in the subtree rooted by the designated node.
// The node order is determined by its stored key.
TreeNode* _TreeMapMinimal(TreeNode* null, TreeNode* curr);

// Return the immediate successor of the designated node.
TreeNode* _TreeMapSuccessor(TreeNode* null, TreeNode* curr);

// Return the immediate predecessor of the designated node.
TreeNode* _TreeMapPredecessor(TreeNode* null, TreeNode* curr);

// Make right rotation for the subtree rooted by the designated node.
// After rotation, the designated node will be the right child of its original left child.
void _TreeMapRightRotate(TreeMapData* data, TreeNode* curr);

// Make left rotation for the subtree rooted by the designated node.
// After rotation, the designated node will be the left child of its original right child.
void _TreeMapLeftRotate(TreeMapData* data, TreeNode* curr);

// Maintain the red black tree property after node insertion.
void _TreeMapInsertFixup(TreeMapData* data, TreeNode* curr);

// Maintain the red black tree property after node deletion.
void _TreeMapDeleteFixup(TreeMapData* data, TreeNode* curr);

// Get the node which stores the key having the same order with the designated one.
TreeNode* _TreeMapSearch(TreeMapData* data, void* key);

// The default hash key comparison function.
int _TreeMapCompare(void* lhs, void* rhs);


#define YACC_DIRECT_LEFT 0
#define YACC_DIRECT_RIGHT 1
#define YACC_COLOR_RED 0
#define YACC_COLOR_BLACK 1
#define YACC_STOP 0
#define YACC_DOWN_LEFT 1
#define YACC_DOWN_RIGHT 2
#define YACC_UP_LEFT 3
#define YACC_UP_RIGHT 4


YACC_MAP_API TreeMap* TreeMapInit(void)
{
    TreeMap* obj = (TreeMap*)malloc(sizeof(TreeMap));
    if (!obj)
        return NULL;

    TreeMapData* data = (TreeMapData*)malloc(sizeof(TreeMapData));
    if (!data) {
        free(obj);
        return NULL;
    }

    // Create the dummy node representing the NULL pointer of the tree.
    TreeNode* null = (TreeNode*)malloc(sizeof(TreeNode));
    if (!null) {
        free(data);
        free(obj);
        return NULL;
    }

    null->color_ = YACC_COLOR_BLACK;
    null->parent_ = NULL;
    null->parent_ = null;
    null->right_ = null;
    null->left_ = null;

    data->size_ = 0;
    data->null_ = null;
    data->root_ = null;
    data->func_cmp_ = _TreeMapCompare;
    data->func_clean_key_ = NULL;
    data->func_clean_val_ = NULL;

    obj->data = data;
    obj->put = TreeMapPut;
    obj->get = TreeMapGet;
    obj->find = TreeMapFind;
    obj->remove = TreeMapRemove;
    obj->size = TreeMapSize;
    obj->minimum = TreeMapMinimum;
    obj->maximum = TreeMapMaximum;
    obj->predecessor = TreeMapPredecessor;
    obj->successor = TreeMapSuccessor;
    obj->first = TreeMapFirst;
    obj->next = TreeMapNext;
    obj->reverse_next = TreeMapReverseNext;
    obj->set_compare = TreeMapSetCompare;
    obj->set_clean_key = TreeMapSetCleanKey;
    obj->set_clean_value = TreeMapSetCleanValue;

    return obj;
}

YACC_MAP_API void TreeMapDeinit(TreeMap* obj)
{
    if (!obj)
        return;

    TreeMapData* data = obj->data;
    _TreeMapDeinit(data);
    free(data->null_);
    free(data);
    free(obj);
    return;
}

YACC_MAP_API bool TreeMapPut(TreeMap* self, void* key, void* value)
{
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node)
        return false;

    TreeMapData* data = self->data;
    TreeNode* null = data->null_;
    node->pair_.key = key;
    node->pair_.value = value;
    node->color_ = YACC_COLOR_RED;
    node->parent_ = null;
    node->left_ = null;
    node->right_ = null;

    TreeMapCompare func_cmp = data->func_cmp_;
    TreeNode* parent = null;
    TreeNode* curr = data->root_;
    char direct = 0;
    while (curr != null) {
        parent = curr;
        int order = func_cmp(key, curr->pair_.key);
        if (order > 0) {
            curr = curr->right_;
            direct = YACC_DIRECT_RIGHT;
        }
        else if (order < 0) {
            curr = curr->left_;
            direct = YACC_DIRECT_LEFT;
        }
        else {
            // Conflict with the already stored key value pair.
            free(node);
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
        if (direct == YACC_DIRECT_LEFT)
            parent->left_ = node;
        else
            parent->right_ = node;
    } else
        data->root_ = node;

    data->size_++;

    // Maintain the red black tree structure.
    _TreeMapInsertFixup(data, node);

    return true;
}

YACC_MAP_API void* TreeMapGet(TreeMap* self, void* key)
{
    TreeNode* node = _TreeMapSearch(self->data, key);
    if (node != self->data->null_)
        return node->pair_.value;
    return NULL;
}

YACC_MAP_API bool TreeMapFind(TreeMap* self, void* key)
{
    TreeNode* node = _TreeMapSearch(self->data, key);
    return (node != self->data->null_)? true : false;
}

YACC_MAP_API bool TreeMapRemove(TreeMap* self, void* key)
{
    TreeMapData* data = self->data;
    TreeNode* null = data->null_;
    TreeNode* curr = _TreeMapSearch(data, key);
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
        free(curr);
    } else {
        // The specified node has two children.
        if ((curr->left_ != null) && (curr->right_ != null)) {
            TreeNode* succ = _TreeMapSuccessor(null, curr);

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
            free(succ);
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
            free(curr);
        }
    }

    // Decrease the size.
    data->size_--;

    // Maintain the balanced tree structure.
    if (color == YACC_COLOR_BLACK)
        _TreeMapDeleteFixup(data, child);

    return true;
}

YACC_MAP_API unsigned TreeMapSize(TreeMap* self)
{
    return self->data->size_;
}

YACC_MAP_API Pair* TreeMapMinimum(TreeMap* self)
{
    TreeNode* node = _TreeMapMinimal(self->data->null_, self->data->root_);
    if (node != self->data->null_)
        return &(node->pair_);
    return NULL;
}

YACC_MAP_API Pair* TreeMapMaximum(TreeMap* self)
{
    TreeNode* node = _TreeMapMaximal(self->data->null_, self->data->root_);
    if (node != self->data->null_)
        return &(node->pair_);
    return NULL;
}

YACC_MAP_API Pair* TreeMapPredecessor(TreeMap* self, void* key)
{
    TreeNode* curr = _TreeMapSearch(self->data, key);
    if (curr == self->data->null_)
        return NULL;

    TreeNode* node = _TreeMapPredecessor(self->data->null_, curr);
    if (node != self->data->null_)
        return &(node->pair_);

    return NULL;
}

YACC_MAP_API Pair* TreeMapSuccessor(TreeMap* self, void* key)
{
    TreeNode* curr = _TreeMapSearch(self->data, key);
    if (curr == self->data->null_)
        return NULL;

    TreeNode* node = _TreeMapSuccessor(self->data->null_, curr);
    if (node != self->data->null_)
        return &(node->pair_);

    return NULL;
}

YACC_MAP_API void TreeMapFirst(TreeMap* self)
{
    self->data->iter_direct_ = YACC_DOWN_LEFT;
    self->data->iter_node_ = self->data->root_;
}

YACC_MAP_API Pair* TreeMapNext(TreeMap* self)
{
    char direct = self->data->iter_direct_;
    TreeNode* null = self->data->null_;
    TreeNode* curr = self->data->iter_node_;

    while (curr != null) {
        if (direct == YACC_DOWN_LEFT || direct == YACC_DOWN_RIGHT) {
            if (curr->left_ != null) {
                curr = curr->left_;
                direct = YACC_DOWN_LEFT;
                continue;
            }

            Pair* pair = &(curr->pair_);

            if (curr->right_ != null) {
                self->data->iter_node_ = curr->right_;
                self->data->iter_direct_ = YACC_DOWN_RIGHT;
            } else {
                TreeNode* temp = curr;
                curr = curr->parent_;
                self->data->iter_node_ = curr;
                if (curr != null) {
                    if (temp == curr->left_)
                        self->data->iter_direct_ = YACC_UP_LEFT;
                    else
                        self->data->iter_direct_ = YACC_UP_RIGHT;
                }
            }
            return pair;
        }

        if (direct == YACC_UP_LEFT) {
            Pair* pair = &(curr->pair_);

            if (curr->right_ != null) {
                self->data->iter_node_ = curr->right_;
                self->data->iter_direct_ = YACC_DOWN_RIGHT;
            } else {
                TreeNode* temp = curr;
                curr = curr->parent_;
                self->data->iter_node_ = curr;
                if (curr != null) {
                    if (temp == curr->left_)
                        self->data->iter_direct_ = YACC_UP_LEFT;
                    else
                        self->data->iter_direct_ = YACC_UP_RIGHT;
                }
            }
            return pair;
        }

        TreeNode* temp = curr;
        curr = curr->parent_;
        if (curr != null) {
            if (temp == curr->left_)
                direct = YACC_UP_LEFT;
            else
                direct = YACC_UP_RIGHT;
        }
    }

    self->data->iter_node_ = null;
    return NULL;
}

YACC_MAP_API Pair* TreeMapReverseNext(TreeMap* self)
{
    char direct = self->data->iter_direct_;
    TreeNode* null = self->data->null_;
    TreeNode* curr = self->data->iter_node_;

    while (curr != null) {
        if (direct == YACC_DOWN_LEFT || direct == YACC_DOWN_RIGHT) {
            if (curr->right_ != null) {
                curr = curr->right_;
                direct = YACC_DOWN_RIGHT;
                continue;
            }

            Pair* pair = &(curr->pair_);

            if (curr->left_ != null) {
                self->data->iter_node_ = curr->left_;
                self->data->iter_direct_ = YACC_DOWN_LEFT;
            } else {
                TreeNode* temp = curr;
                curr = curr->parent_;
                self->data->iter_node_ = curr;
                if (curr != null) {
                    if (temp == curr->left_)
                        self->data->iter_direct_ = YACC_UP_LEFT;
                    else
                        self->data->iter_direct_ = YACC_UP_RIGHT;
                }
            }
            return pair;
        }

        if (direct == YACC_UP_RIGHT) {
            Pair* pair = &(curr->pair_);

            if (curr->left_ != null) {
                self->data->iter_node_ = curr->left_;
                self->data->iter_direct_ = YACC_DOWN_LEFT;
            } else {
                TreeNode* temp = curr;
                curr = curr->parent_;
                self->data->iter_node_ = curr;
                if (curr != null) {
                    if (temp == curr->left_)
                        self->data->iter_direct_ = YACC_UP_LEFT;
                    else
                        self->data->iter_direct_ = YACC_UP_RIGHT;
                }
            }
            return pair;
        }

        TreeNode* temp = curr;
        curr = curr->parent_;
        if (curr != null) {
            if (temp == curr->left_)
                direct = YACC_UP_LEFT;
            else
                direct = YACC_UP_RIGHT;
        }
    }

    self->data->iter_node_ = null;
    return NULL;
}

YACC_MAP_API void TreeMapSetCompare(TreeMap* self, TreeMapCompare func)
{
    self->data->func_cmp_ = func;
}

YACC_MAP_API void TreeMapSetCleanKey(TreeMap* self, TreeMapCleanKey func)
{
    self->data->func_clean_key_ = func;
}

YACC_MAP_API void TreeMapSetCleanValue(TreeMap* self, TreeMapCleanValue func)
{
    self->data->func_clean_val_ = func;
}


//
// Implementation for internal operations
//

void _TreeMapDeinit(TreeMapData* data)
{
    TreeNode* null = data->null_;
    if (data->root_ == null)
        return;

    TreeMapCleanKey func_clean_key = data->func_clean_key_;
    TreeMapCleanValue func_clean_val = data->func_clean_val_;

    char direct = YACC_DOWN_LEFT;
    TreeNode* curr = data->root_;
    while (direct != YACC_STOP) {
        if (direct == YACC_DOWN_LEFT || direct == YACC_DOWN_RIGHT) {
            if (curr->left_ != null) {
                curr = curr->left_;
                direct = YACC_DOWN_LEFT;
                continue;
            }
            if (curr->right_ != null) {
                curr = curr->right_;
                direct = YACC_DOWN_RIGHT;
                continue;
            }

            TreeNode* temp = curr;
            curr = curr->parent_;
            if (curr == null)
                direct = YACC_STOP;
            else
                direct = (temp == curr->left_)? YACC_UP_LEFT : YACC_UP_RIGHT;

            if (func_clean_key)
                func_clean_key(temp->pair_.key);
            if (func_clean_val)
                func_clean_val(temp->pair_.value);
            free(temp);
            continue;
        }

        if (direct == YACC_UP_LEFT) {
            if (curr->right_ != null) {
                curr = curr->right_;
                direct = YACC_DOWN_RIGHT;
                continue;
            }

            TreeNode* temp = curr;
            curr = curr->parent_;
            if (curr == null)
                direct = YACC_STOP;
            else
                direct = (temp == curr->left_)? YACC_UP_LEFT : YACC_UP_RIGHT;

            if (func_clean_key)
                func_clean_key(temp->pair_.key);
            if (func_clean_val)
                func_clean_val(temp->pair_.value);
            free(temp);
            continue;
        }

        TreeNode* temp = curr;
        curr = curr->parent_;
        if (curr == null)
            direct = YACC_STOP;
        else
            direct = (temp == curr->left_)? YACC_UP_LEFT : YACC_UP_RIGHT;

        if (func_clean_key)
            func_clean_key(temp->pair_.key);
        if (func_clean_val)
            func_clean_val(temp->pair_.value);
        free(temp);
    }

    return;
}

TreeNode* _TreeMapMaximal(TreeNode* null, TreeNode* curr)
{
    TreeNode* parent = null;
    while (curr != null) {
        parent = curr;
        curr = curr->right_;
    }
    return parent;
}

TreeNode* _TreeMapMinimal(TreeNode* null, TreeNode* curr)
{
    TreeNode* parent = null;
    while (curr != null) {
        parent = curr;
        curr = curr->left_;
    }
    return parent;
}

TreeNode* _TreeMapSuccessor(TreeNode* null, TreeNode* curr)
{
    if (curr != null) {
        // Case 1: The minimal node in the non-null right subtree.
        if (curr->right_ != null)
            curr = _TreeMapMinimal(null, curr->right_);

        // Case 2: The ancestor which considers the designated node as the maximal node of its left subtree.
        else {
            while((curr->parent_ != null) && (curr == curr->parent_->right_))
                curr = curr->parent_;
            curr = curr->parent_;
        }
    }
    return curr;
}

TreeNode* _TreeMapPredecessor(TreeNode* null, TreeNode* curr)
{
    if (curr != null) {
        // Case 1: The maximal node in the non-null left subtree.
        if (curr->left_ != null)
            curr = _TreeMapMaximal(null, curr->left_);

        // Case 2: The ancestor which considers the designated node as the minimal node of its right subtree.
        else {
            while((curr->parent_ != null) && (curr == curr->parent_->left_))
                curr = curr->parent_;
            curr = curr->parent_;
        }
    }
    return curr;
}

void _TreeMapRightRotate(TreeMapData* data, TreeNode* curr)
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

void _TreeMapLeftRotate(TreeMapData* data, TreeNode* curr)
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
    // If b is not dummy node, let b link x as its parent. */
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

void _TreeMapInsertFixup(TreeMapData* data, TreeNode* curr)
{
    TreeNode* uncle;

    // Denote the current node as x.
    while (curr->parent_->color_ == YACC_COLOR_RED) {
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
            if (uncle->color_ == YACC_COLOR_RED) {
                curr->parent_->color_ = YACC_COLOR_BLACK;
                uncle->color_ = YACC_COLOR_BLACK;
                curr->parent_->parent_->color_ = YACC_COLOR_RED;
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
                    _TreeMapLeftRotate(data, curr);
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
                curr->parent_->color_ = YACC_COLOR_BLACK;
                curr->parent_->parent_->color_ = YACC_COLOR_RED;
                _TreeMapRightRotate(data, curr->parent_->parent_);
            }
        }
        // x's parent is its grandparent's right child.
        else {
            uncle = curr->parent_->parent_->left_;

            // Case 1: The color of x's uncle is also red.
            if (uncle->color_ == YACC_COLOR_RED) {
                curr->parent_->color_ = YACC_COLOR_BLACK;
                uncle->color_ = YACC_COLOR_BLACK;
                curr->parent_->parent_->color_ = YACC_COLOR_RED;
                curr = curr->parent_->parent_;
            } else {
                // Case 2: The color of x's uncle is black, and x is its parent's left child.
                if (curr == curr->parent_->left_) {
                    curr = curr->parent_;
                    _TreeMapRightRotate(data, curr);
                }
                // Case 3: The color of x's uncle is black, and x is its parent's right child.
                curr->parent_->color_ = YACC_COLOR_BLACK;
                curr->parent_->parent_->color_ = YACC_COLOR_RED;
                _TreeMapLeftRotate(data, curr->parent_->parent_);
            }
        }
    }

    data->root_->color_ = YACC_COLOR_BLACK;
    return;
}

void _TreeMapDeleteFixup(TreeMapData* data, TreeNode* curr)
{
    TreeNode* brother;

    // Denote the current node as x.
    while ((curr != data->root_) && (curr->color_ == YACC_COLOR_BLACK)) {
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
            if (brother->color_ == YACC_COLOR_RED) {
                brother->color_ = YACC_COLOR_BLACK;
                curr->parent_->color_ = YACC_COLOR_RED;
                _TreeMapLeftRotate(data, curr->parent_);
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
            if ((brother->left_->color_ == YACC_COLOR_BLACK) &&
                (brother->right_->color_ == YACC_COLOR_BLACK)) {
                brother->color_ = YACC_COLOR_RED;
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
                if (brother->right_->color_ == YACC_COLOR_BLACK) {
                    brother->left_->color_ = YACC_COLOR_BLACK;
                    brother->color_ = YACC_COLOR_RED;
                    _TreeMapRightRotate(data, brother);
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
                curr->parent_->color_ = YACC_COLOR_BLACK;
                brother->right_->color_ = YACC_COLOR_BLACK;
                _TreeMapLeftRotate(data, curr->parent_);
                curr = data->root_;
            }
        }
        // x is its parent's right child.
        else {
            brother = curr->parent_->left_;
            // Case 1: The color of x's brother is red.
            if (brother->color_ == YACC_COLOR_RED) {
                brother->color_ = YACC_COLOR_BLACK;
                curr->parent_->color_ = YACC_COLOR_RED;
                _TreeMapRightRotate(data, curr->parent_);
                brother = curr->parent_->left_;
            }
            // Case 2: The color of x's brother is black, and both of its children are also black.
            if ((brother->left_->color_ == YACC_COLOR_BLACK) &&
                (brother->right_->color_ == YACC_COLOR_BLACK)) {
                brother->color_ = YACC_COLOR_RED;
                curr = curr->parent_;
            } else {
                // Case 3: The color of x's brother is black and the colors of its
                // right and left child are red and black respectively.
                if (brother->left_->color_ == YACC_COLOR_BLACK) {
                    brother->right_->color_ = YACC_COLOR_BLACK;
                    brother->color_ = YACC_COLOR_RED;
                    _TreeMapLeftRotate(data, brother);
                    brother = curr->parent_->left_;
                }
                // Case 4: The color of x's brother is black, and its left child is red.
                brother->color_ = curr->parent_->color_;
                curr->parent_->color_ = YACC_COLOR_BLACK;
                brother->left_->color_ = YACC_COLOR_BLACK;
                _TreeMapRightRotate(data, curr->parent_);
                curr = data->root_;
            }
        }
    }

    curr->color_ = YACC_COLOR_BLACK;
    return;
}

TreeNode* _TreeMapSearch(TreeMapData* data, void* key)
{
    TreeMapCompare func_cmp = data->func_cmp_;
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

int _TreeMapCompare(void* lhs, void* rhs)
{
    if ((intptr_t)lhs == (intptr_t)rhs)
        return 0;
    return ((intptr_t)lhs >= (intptr_t)rhs)? 1 : (-1);
}


#undef YACC_DIRECT_LEFT
#undef YACC_DIRECT_RIGHT
#undef YACC_COLOR_RED
#undef YACC_COLOR_BLACK
#undef YACC_STOP
#undef YACC_DOWN_LEFT
#undef YACC_DOWN_RIGHT
#undef YACC_UP_LEFT
#undef YACC_UP_RIGHT


#endif // YACC_MAP_IMPLEMENTATION

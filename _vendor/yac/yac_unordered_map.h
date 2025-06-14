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


#ifndef YAC_UNORDERED_MAP_H_
#define YAC_UNORDERED_MAP_H_

#include <stdbool.h> // bool

#ifndef YAC_UNORDERED_MAP_API
#ifdef YAC_UNORDERED_MAP_STATIC
#define YAC_UNORDERED_MAP_API static
#else
#define YAC_UNORDERED_MAP_API extern
#endif // YAC_UNORDERED_MAP_STATIC
#endif // YAC_UNORDERED_MAP_API


// The key value pair for associative data structures.
typedef struct _YacUnorderedMapPair {
    void* key;
    void* value;
} YacUnorderedMapPair;


// YacUnorderedMapData is the data type for the container private information.
typedef struct _YacUnorderedMapData YacUnorderedMapData;

// Calculate the hash of the given key.
typedef unsigned (*YacUnorderedMapHash) (void*);

// Compare the equality of two keys.
typedef int (*YacUnorderedMapCompare) (void*, void*);

// Key cleanup function called whenever a live entry is removed.
typedef void (*YacUnorderedMapCleanKey) (void*);

// Value cleanup function called whenever a live entry is removed.
typedef void (*YacUnorderedMapCleanValue) (void*);


// The implementation for unordered map.
typedef struct _YacUnorderedMap {
    // The container private information
    YacUnorderedMapData* data;

    // Insert a key value pair into the map. @see YacUnorderedMapPut.
    bool (*put) (struct _YacUnorderedMap*, void*, void*);

    // Retrieve the value corresponding to the specified key. @see YacUnorderedMapGet.
    void* (*get) (struct _YacUnorderedMap*, void*);

    // Check if the map contains the specified key. @see YacUnorderedMapContain.
    bool (*contain) (struct _YacUnorderedMap*, void*);

    // Remove the key value pair corresponding to the specified key. @see YacUnorderedMapRemove.
    bool (*remove) (struct _YacUnorderedMap*, void*);

    // Return the number of stored key value pairs. @see YacUnorderedMapSize.
    unsigned (*size) (struct _YacUnorderedMap*);

    // Initialize the map iterator. @see YacUnorderedMapFirst.
    void (*first) (struct _YacUnorderedMap*);

    // Get the key value pair pointed by the iterator and advance the iterator @see YacUnorderedMapNext.
    YacUnorderedMapPair* (*next) (struct _YacUnorderedMap*);

    // Set the custom hash function. @see YacUnorderedMapSetHash.
    void (*set_hash) (struct _YacUnorderedMap*, YacUnorderedMapHash);

    // Set the custom key comparison function. @see YacUnorderedMapSetCompare.
    void (*set_compare) (struct _YacUnorderedMap*, YacUnorderedMapCompare);

    // Set the custom key cleanup function. @see YacUnorderedMapSetCleanKey.
    void (*set_clean_key) (struct _YacUnorderedMap*, YacUnorderedMapCleanKey);

    // Set the custom value cleanup function. @see YacUnorderedMapSetCleanValue
    void (*set_clean_value) (struct _YacUnorderedMap*, YacUnorderedMapCleanValue);
} YacUnorderedMap;


//
// Definition for the exported member operations
//

// The constructor for YacUnorderedMap.
YAC_UNORDERED_MAP_API YacUnorderedMap* YacUnorderedMapInit(void);

// The destructor for YacUnorderedMap.
YAC_UNORDERED_MAP_API void YacUnorderedMapDeinit(YacUnorderedMap* obj);

// Insert a key value pair into the map.
// This function inserts a key value pair into the map. If the specified key is
// equal to a certain one stored in the map, the existing pair will be replaced.
// Also, the cleanup functions are invoked for that replaced pair.
YAC_UNORDERED_MAP_API bool YacUnorderedMapPut(YacUnorderedMap* self, void* key, void* value);

// Retrieve the value corresponding to the specified key.
YAC_UNORDERED_MAP_API void* YacUnorderedMapGet(YacUnorderedMap* self, void* key);

// Check if the map contains the specified key.
YAC_UNORDERED_MAP_API bool YacUnorderedMapContain(YacUnorderedMap* self, void* key);

// Remove the key value pair corresponding to the specified key.
// This function removes the key value pair corresponding to the specified key.
// Also, the cleanup functions are invoked for that removed pair.
YAC_UNORDERED_MAP_API bool YacUnorderedMapRemove(YacUnorderedMap* self, void* key);

// Return the number of stored key value pairs.
YAC_UNORDERED_MAP_API unsigned YacUnorderedMapSize(YacUnorderedMap* self);

// Initialize the map iterator.
YAC_UNORDERED_MAP_API void YacUnorderedMapFirst(YacUnorderedMap* self);

// Get the key value pair pointed by the iterator and advance the iterator.
YAC_UNORDERED_MAP_API YacUnorderedMapPair* YacUnorderedMapNext(YacUnorderedMap* self);

// Set the custom hash function.
// By default, the hash function is HashMurMur32.
YAC_UNORDERED_MAP_API void YacUnorderedMapSetHash(YacUnorderedMap* self, YacUnorderedMapHash func);

// Set the custom key comparison function.
// By default, key is treated as integer.
YAC_UNORDERED_MAP_API void YacUnorderedMapSetCompare(YacUnorderedMap* self, YacUnorderedMapCompare func);

// Set the custom key cleanup function.
// By default, no cleanup operation for key.
YAC_UNORDERED_MAP_API void YacUnorderedMapSetCleanKey(YacUnorderedMap* self, YacUnorderedMapCleanKey func);

// Set the custom value cleanup function.
// By default, no cleanup operation for value.
YAC_UNORDERED_MAP_API void YacUnorderedMapSetCleanValue(YacUnorderedMap* self, YacUnorderedMapCleanValue func);


// Non-cryptographic hash function

// Google MurMur hash proposed by Austin Appleby in 2008.
// This is the version 3 MurMur implementation to yield 32 bit hash value.
// https://code.google.com/p/smhasher/wiki/MurmurHash3
YAC_UNORDERED_MAP_API unsigned YacUnorderedMapHashMurMur32(void* key, size_t size);

// Frequently applied hash function for strings.
// http://www.cse.yorku.ca/~oz/hash.html
YAC_UNORDERED_MAP_API unsigned YacUnorderedMapHashDjb2(char* key);


#endif // YAC_UNORDERED_MAP_H_


//
// IMPLEMENTATION
//

#ifdef YAC_UNORDERED_MAP_IMPLEMENTATION


#include <stdint.h> // utf8_t
#include <stdlib.h> // malloc, free

#ifndef YAC_ORDERED_MAP_MALLOC
#define YAC_ORDERED_MAP_MALLOC malloc
#endif

#ifndef YAC_ORDERED_MAP_FREE
#define YAC_ORDERED_MAP_FREE free
#endif


//
// The container private data
//

// The container private data
static const unsigned yac_unordered_map_magic_primes[] = {
    769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433,
    1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319,
    201326611, 402653189, 805306457, 1610612741,
};
static const int yac_unordered_map_num_prime = sizeof(yac_unordered_map_magic_primes) / sizeof(unsigned);
static const double yac_unordered_map_load_factor = 0.75;


typedef struct _YacUnorderedMapSlotNode {
    YacUnorderedMapPair pair_;
    struct _YacUnorderedMapSlotNode* next_;
} YacUnorderedMapSlotNode;

struct _YacUnorderedMapData {
    int size_;
    int idx_prime_;
    unsigned num_slot_;
    unsigned curr_limit_;
    unsigned iter_slot_;
    YacUnorderedMapSlotNode** arr_slot_;
    YacUnorderedMapSlotNode* iter_node_;
    YacUnorderedMapHash func_hash_;
    YacUnorderedMapCompare func_cmp_;
    YacUnorderedMapCleanKey func_clean_key_;
    YacUnorderedMapCleanValue func_clean_val_;
};


//
// Definition for internal operations
//

// The default hash function.
static unsigned YacUnorderedMapHash_(void* key);

// The default hash key comparison function.
static int YacUnorderedMapCompare_(void* lhs, void* rhs);

// Extend the slot array and re-distribute the stored pairs.
static void YacUnorderedMapReHash_(YacUnorderedMapData* data);


//
// Implementation for the exported operations
//

YAC_UNORDERED_MAP_API YacUnorderedMap* YacUnorderedMapInit(void)
{
    YacUnorderedMap* obj = YAC_ORDERED_MAP_MALLOC(sizeof(YacUnorderedMap));
    if (!obj)
        return NULL;

    YacUnorderedMapData* data = YAC_ORDERED_MAP_MALLOC(sizeof(YacUnorderedMapData));
    if (!data) {
        YAC_ORDERED_MAP_FREE(obj);
        return NULL;
    }

    YacUnorderedMapSlotNode** arr_slot = YAC_ORDERED_MAP_MALLOC(sizeof(YacUnorderedMapSlotNode*) * yac_unordered_map_magic_primes[0]);
    if (!arr_slot) {
        YAC_ORDERED_MAP_FREE(data);
        YAC_ORDERED_MAP_FREE(obj);
        return NULL;
    }
    unsigned i;
    for (i = 0 ; i < yac_unordered_map_magic_primes[0] ; ++i)
        arr_slot[i] = NULL;

    data->size_ = 0;
    data->idx_prime_ = 0;
    data->num_slot_ = yac_unordered_map_magic_primes[0];
    data->curr_limit_ = (unsigned)((double)yac_unordered_map_magic_primes[0] * yac_unordered_map_load_factor);
    data->arr_slot_ = arr_slot;
    data->func_hash_ = YacUnorderedMapHash_;
    data->func_cmp_ = YacUnorderedMapCompare_;
    data->func_clean_key_ = NULL;
    data->func_clean_val_ = NULL;

    obj->data = data;
    obj->put = YacUnorderedMapPut;
    obj->get = YacUnorderedMapGet;
    obj->contain = YacUnorderedMapContain;
    obj->remove = YacUnorderedMapRemove;
    obj->size = YacUnorderedMapSize;
    obj->first = YacUnorderedMapFirst;
    obj->next = YacUnorderedMapNext;
    obj->set_hash = YacUnorderedMapSetHash;
    obj->set_compare = YacUnorderedMapSetCompare;
    obj->set_clean_key = YacUnorderedMapSetCleanKey;
    obj->set_clean_value = YacUnorderedMapSetCleanValue;

    return obj;
}

YAC_UNORDERED_MAP_API void YacUnorderedMapDeinit(YacUnorderedMap* obj)
{
    if (!obj)
        return;

    YacUnorderedMapData* data = obj->data;
    YacUnorderedMapSlotNode** arr_slot = data->arr_slot_;
    YacUnorderedMapCleanKey func_clean_key = data->func_clean_key_;
    YacUnorderedMapCleanValue func_clean_val = data->func_clean_val_;

    unsigned num_slot = data->num_slot_;
    unsigned i;
    for (i = 0 ; i < num_slot ; ++i) {
        YacUnorderedMapSlotNode* pred;
        YacUnorderedMapSlotNode* curr = arr_slot[i];
        while (curr) {
            pred = curr;
            curr = curr->next_;
            if (func_clean_key)
                func_clean_key(pred->pair_.key);
            if (func_clean_val)
                func_clean_val(pred->pair_.value);
            YAC_ORDERED_MAP_FREE(pred);
        }
    }

    YAC_ORDERED_MAP_FREE(arr_slot);
    YAC_ORDERED_MAP_FREE(data);
    YAC_ORDERED_MAP_FREE(obj);
    return;
}

YAC_UNORDERED_MAP_API bool YacUnorderedMapPut(YacUnorderedMap* self, void* key, void* value)
{
    // Check the loading factor for rehashing.
    YacUnorderedMapData* data = self->data;
    if ((unsigned)data->size_ >= data->curr_limit_)
        YacUnorderedMapReHash_(data);

    // Calculate the slot index.
    unsigned hash = data->func_hash_(key);
    hash = hash % data->num_slot_;

    // Check if the pair conflicts with a certain one stored in the map. If yes, replace that one.
    YacUnorderedMapCompare func_cmp = data->func_cmp_;
    YacUnorderedMapSlotNode** arr_slot = data->arr_slot_;
    YacUnorderedMapSlotNode* curr = arr_slot[hash];
    while (curr) {
        if (func_cmp(key, curr->pair_.key) == 0) {
            if (data->func_clean_key_)
                data->func_clean_key_(curr->pair_.key);
            if (data->func_clean_val_)
                data->func_clean_val_(curr->pair_.value);
            curr->pair_.key = key;
            curr->pair_.value = value;
            return true;
        }
        curr = curr->next_;
    }

    // Insert the new pair into the slot list.
    YacUnorderedMapSlotNode* node = YAC_ORDERED_MAP_MALLOC(sizeof(YacUnorderedMapSlotNode));
    if (!node)
        return false;

    node->pair_.key = key;
    node->pair_.value = value;
    if (!(arr_slot[hash])) {
        node->next_ = NULL;
        arr_slot[hash] = node;
    } else {
        node->next_ = arr_slot[hash];
        arr_slot[hash] = node;
    }
    ++(data->size_);

    return true;
}

YAC_UNORDERED_MAP_API void* YacUnorderedMapGet(YacUnorderedMap* self, void* key)
{
    YacUnorderedMapData* data = self->data;

    // Calculate the slot index.
    unsigned hash = data->func_hash_(key);
    hash = hash % data->num_slot_;

    // Search the slot list to check if there is a pair having the same key with the designated one.
    YacUnorderedMapCompare func_cmp = data->func_cmp_;
    YacUnorderedMapSlotNode* curr = data->arr_slot_[hash];
    while (curr) {
        if (func_cmp(key, curr->pair_.key) == 0)
            return curr->pair_.value;
        curr = curr->next_;
    }

    return NULL;
}

YAC_UNORDERED_MAP_API bool YacUnorderedMapContain(YacUnorderedMap* self, void* key)
{
    YacUnorderedMapData* data = self->data;

    // Calculate the slot index.
    unsigned hash = data->func_hash_(key);
    hash = hash % data->num_slot_;

    // Search the slot list to check if there is a pair having the same key with the designated one.
    YacUnorderedMapCompare func_cmp = data->func_cmp_;
    YacUnorderedMapSlotNode* curr = data->arr_slot_[hash];
    while (curr) {
        if (func_cmp(key, curr->pair_.key) == 0)
            return true;
        curr = curr->next_;
    }

    return false;
}

YAC_UNORDERED_MAP_API bool YacUnorderedMapRemove(YacUnorderedMap* self, void* key)
{
    YacUnorderedMapData* data = self->data;

    // Calculate the slot index.
    unsigned hash = data->func_hash_(key);
    hash = hash % data->num_slot_;

    // Search the slot list for the deletion target.
    YacUnorderedMapCompare func_cmp = data->func_cmp_;
    YacUnorderedMapSlotNode* pred = NULL;
    YacUnorderedMapSlotNode** arr_slot = data->arr_slot_;
    YacUnorderedMapSlotNode* curr = arr_slot[hash];
    while (curr) {
        if (func_cmp(key, curr->pair_.key) == 0) {
            if (data->func_clean_key_)
                data->func_clean_key_(curr->pair_.key);
            if (data->func_clean_val_)
                data->func_clean_val_(curr->pair_.value);

            if (!pred)
                arr_slot[hash] = curr->next_;
            else
                pred->next_ = curr->next_;

            YAC_ORDERED_MAP_FREE(curr);
            --(data->size_);
            return true;
        }
        pred = curr;
        curr = curr->next_;
    }

    return false;
}

YAC_UNORDERED_MAP_API unsigned YacUnorderedMapSize(YacUnorderedMap* self)
{
    return self->data->size_;
}

YAC_UNORDERED_MAP_API void YacUnorderedMapFirst(YacUnorderedMap* self)
{
    YacUnorderedMapData* data = self->data;
    data->iter_slot_ = 0;
    data->iter_node_ = data->arr_slot_[0];
    return;
}

YAC_UNORDERED_MAP_API YacUnorderedMapPair* YacUnorderedMapNext(YacUnorderedMap* self)
{
    YacUnorderedMapData* data = self->data;

    YacUnorderedMapSlotNode** arr_slot = data->arr_slot_;
    while (data->iter_slot_ < data->num_slot_) {
        if (data->iter_node_) {
            YacUnorderedMapPair* ptr_pair = &(data->iter_node_->pair_);
            data->iter_node_ = data->iter_node_->next_;
            return ptr_pair;
        }
        ++(data->iter_slot_);
        if (data->iter_slot_ == data->num_slot_)
            break;
        data->iter_node_ = arr_slot[data->iter_slot_];
    }
    return NULL;
}

YAC_UNORDERED_MAP_API void YacUnorderedMapSetHash(YacUnorderedMap* self, YacUnorderedMapHash func)
{
    self->data->func_hash_ = func;
}

YAC_UNORDERED_MAP_API void YacUnorderedMapSetCompare(YacUnorderedMap* self, YacUnorderedMapCompare func)
{
    self->data->func_cmp_ = func;
}

YAC_UNORDERED_MAP_API void YacUnorderedMapSetCleanKey(YacUnorderedMap* self, YacUnorderedMapCleanKey func)
{
    self->data->func_clean_key_ = func;
}

YAC_UNORDERED_MAP_API void YacUnorderedMapSetCleanValue(YacUnorderedMap* self, YacUnorderedMapCleanValue func)
{
    self->data->func_clean_val_ = func;
}

YAC_UNORDERED_MAP_API unsigned YacUnorderedMapHashMurMur32(void* key, size_t size)
{
    if (!key || size == 0)
        return 0;

    const unsigned c1 = 0xcc9e2d51;
    const unsigned c2 = 0x1b873593;
    const unsigned r1 = 15;
    const unsigned r2 = 13;
    const unsigned m = 5;
    const unsigned n = 0xe6546b64;

    unsigned hash = 0xdeadbeef;

    // TODO
    const int nblocks = (const int)size / 4;
    const unsigned *blocks = (const unsigned*)key;
    int i;
    for (i = 0; i < nblocks; i++) {
        unsigned k = blocks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        hash ^= k;
        hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
    }

    // TODO
    const uint8_t *tail = (const uint8_t*) ((int*)key + nblocks * 4);
    unsigned k1 = 0;

    switch (size & 3) {
        case 3:
            k1 ^= tail[2] << 16;
        case 2:
            k1 ^= tail[1] << 8;
        case 1:
            k1 ^= tail[0];

            k1 *= c1;
            k1 = (k1 << r1) | (k1 >> (32 - r1));
            k1 *= c2;
            hash ^= k1;
    }

    hash ^= size;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}

YAC_UNORDERED_MAP_API unsigned YacUnorderedMapHashDjb2(char* key)
{
    unsigned hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


//
// Implementation for internal operations
//

static unsigned YacUnorderedMapHash_(void* key)
{
    return (unsigned)(intptr_t)key;
}

static int YacUnorderedMapCompare_(void* lhs, void* rhs)
{
    if ((intptr_t)lhs == (intptr_t)rhs)
        return 0;
    return ((intptr_t)lhs > (intptr_t)rhs)? 1 : (-1);
}

static void YacUnorderedMapReHash_(YacUnorderedMapData* data)
{
    unsigned num_slot_new;

    // Consume the next prime for slot array extension.
    if (data->idx_prime_ < (yac_unordered_map_num_prime - 1)) {
        ++(data->idx_prime_);
        num_slot_new = yac_unordered_map_magic_primes[data->idx_prime_];
    }
    // If the prime list is completely consumed, we simply extend the slot array with treble capacity.
    else {
        data->idx_prime_ = yac_unordered_map_num_prime;
        num_slot_new = data->num_slot_ * 3;
    }

    // Try to allocate the new slot array. The rehashing should be canceled due to insufficient memory space.
    YacUnorderedMapSlotNode** arr_slot_new = YAC_ORDERED_MAP_MALLOC(sizeof(YacUnorderedMapSlotNode*) * num_slot_new);
    if (!arr_slot_new) {
        if (data->idx_prime_ < yac_unordered_map_num_prime)
            --(data->idx_prime_);
        return;
    }

    unsigned i;
    for (i = 0 ; i < num_slot_new ; ++i)
        arr_slot_new[i] = NULL;

    YacUnorderedMapHash func_hash = data->func_hash_;
    YacUnorderedMapSlotNode** arr_slot = data->arr_slot_;
    unsigned num_slot = data->num_slot_;
    for (i = 0 ; i < num_slot ; ++i) {
        YacUnorderedMapSlotNode* pred;
        YacUnorderedMapSlotNode* curr = arr_slot[i];
        while (curr) {
            pred = curr;
            curr = curr->next_;

            // Migrate each key value pair to the new slot.
            unsigned hash = func_hash(pred->pair_.key);
            hash = hash % num_slot_new;
            if (!arr_slot_new[hash]) {
                pred->next_ = NULL;
                arr_slot_new[hash] = pred;
            } else {
                pred->next_ = arr_slot_new[hash];
                arr_slot_new[hash] = pred;
            }
        }
    }

    YAC_ORDERED_MAP_FREE(arr_slot);
    data->arr_slot_ = arr_slot_new;
    data->num_slot_ = num_slot_new;
    data->curr_limit_ = (unsigned)((double)num_slot_new * yac_unordered_map_load_factor);
    return;
}


#endif // YCC_UNORDERED_MAP_IMPLEMENTATION

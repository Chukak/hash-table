/* 
    Instead unsigned char, is used char in all the program.
    See hashtable.h for more info about struct hash_table, hash_table_item, DELETED type. 
*/
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hashtable.h"

/* 
    static function, create new hash_table_item.
    set value by key in table.
    return pointer to item // {'key' : 'value'} 

    strdup not ANSI func. 
    strdup call malloc func, allocate memory for string, 
	copy string in this place and return pointer to her
*/
static hash_table_item *new_table_item(const char *key, const char *val) {
    hash_table_item *item = malloc(sizeof(hash_table_item));
    item->key = strdup(key); 
    item->val = strdup(val);
    return item;
}

/*
    static function, remove hash_table_item from memory.
    nothing to return.
 */
static void delete_table_item(hash_table_item *item) {
    free(item->key), free(item->val), free(item);
}

/* 
    this func djb2 hash.
    more info http://www.cse.yorku.ca/~oz/hash.html .
    return hash.
*/
static uint32_t get_hash(const char *key) {
    uint32_t hash = 5381;
    int32_t i;
    // set pointer to start of string, fix func
    // else raise segmentation fault error
    key++;
    while (i = *key) {
	hash = ((hash << 5) + hash) + i;
        key++;
    }
    return hash;
} 

/* 
    double hashing func for resolve hash collisions.
    more info https://en.wikipedia.org/wiki/Double_hashing .
    return hash.
*/
static int32_t hash(const char *key, const int32_t num, const int32_t try) {
    int32_t hash_a = get_hash(key);
    int32_t hash_b = get_hash(key);
    // try = attempts, num = size array
    // standart formula: hash_a(key) + try * hash_b(key) mod size
    // if hash_b == 0 then 1 - guarantees that hash_b never be zero 
    // if hash_b > 0 then hash_b 
    return (hash_a + try * (hash_b == 0 ? 1 : hash_b)) % num;
}

/* 
    create hash_table function, creates table in memory.
    return pointer to table.
*/
hash_table *create_hash_table(size_t size){
    hash_table *table = malloc(sizeof(hash_table));
    // size_t -> unsigned int32 
    table->size = (uint32_t)size;
    // set count
    table->count = 0;
    // return pointer to allocated memory for all items
    // 
    table->items = calloc(size, sizeof(hash_table_item));
    return table;
}

/* 
    insert hash_table_item func.
    insert value by key into a table.
    nothing to return
*/
void insert_item(hash_table *table, const char *key, const char*val) {
    hash_table_item *new_item, *current_item; 
    int32_t index, try;
    // get new hash_table_item, value by key 
    new_item = new_table_item(key, val);
    // get hash
    index = hash(new_item->key, table->size, 0);
    // get current item
    current_item = table->items[index];
    // number of attemps
    try = 1;
    while (current_item) { 
	// check if item is not deleted ( if item exists )
	// if item exists, replace value by key
	// if true, check key and item.key
	// if true, delete current value by key, set new value by key
	// return
	// if item if deleted, stop cycle 
        if (current_item != &DELETED) {
            if (strcmp(current_item->key, key) == 0) {
                delete_table_item(current_item);
                table->items[index] = new_item;
                return;
            }
        } else {
            break;
        }
	// get new hash
        index = hash(new_item->key, table->size, try);
	// get new current item
        current_item = table->items[index];
	// +1 attempts
        try++;
    }
    // set new item and count
    table->items[index] = new_item;
    table->count++;
}

/* 
    get value func.
    return pointer to value by key.
	
*/
char *get_value(hash_table *table, const char *key) {
    int32_t index, try;
    // number of attempts
    try = 1;
    // get new hash
    index = hash(key, table->size, 0);
    // get item
    hash_table_item *item = table->items[index];
    while (item) {
	// check if item is not deleted
	// if true, check key and item.key
	// if true, return value by key
        if (item != &DELETED) {
            if (strcmp(item->key, key) == 0) {
                return item->val;
            }
        }
	// if false, get new item, +1 attempts    
        index = hash(key, table->size, try);
        item = table->items[index];
        try++;
    }
    // if nothing if found
    return NULL;
}

/* 
    delete hash_table_item func.
    nothing to return.
    removes value by key
*/
void delete_item(hash_table *table, const char *key) {
    int32_t index, try;
    // number of attempts
    try = 1;
    // get new hash
    index = hash(key, table->size, 0);
    // get item
    hash_table_item *item = table->items[index];
    while (item) {
        // if key and item.key
	// remove item from memory
	// set this item is deleted to table
	// stop iteration
        if (strcmp(item->key, key) == 0) {
            delete_table_item(item);
            table->items[index] = &DELETED;
            break;
        }
	// get new item, +1 attempts
        index = hash(key, table->size, try);
		item = table->items[index];
        try++;
    }
    // set new count of items into table
    table->count--;
}

/* 
    delete hash_table func.
    nothing of return.
    remove hash_table from memory.
*/
void delete_hash_table(hash_table *table) {
    hash_table_item *item;
    // iteration over all items
    for (uint32_t index = 0; index < table->size; index++) {
	// set new item
        item = table->items[index];
	// item is not NULL and is not deleted
	// remove item from memory
	// else skip
        if (item && item != &DELETED) {
            delete_table_item(item);
        }
    }
    // remove table from memory
    free(table->items), free(table);
}   

/* 
    See hashtable.h for more info about struct hash_table, hash_table_item.
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "hashtable.h"

/* 
    static variable DELETED_ITEM, for check all deleted items in table.
    key and val must be null.
*/
static tb_hash_table_item DELETED = {NULL, NULL};

/* 
    static function, creates a new hash_table_item.
    sets value by key in table and returns pointer to item .
    Item example: {'key' : 'value'} .

    strdup not ANSI func. 
    strdup call malloc func, allocates memory for string, 
	copies string in this place and returns pointer to string.
*/
static tb_hash_table_item *tb_new_table_item(const char *key, const void *val) {
    tb_hash_table_item *item = malloc(sizeof(tb_hash_table_item));
    item->key = strdup(key); 
    item->val = strdup((char *)val);
    return item;
}

/*
    static function, removes hash_table_item from memory.
    nothing to returns.
 */
static void tb_delete_table_item(tb_hash_table_item *item) {
    free(item->key);
    free(item->val);
    free(item);
}

/* 
    funtion returns a djb2 hash.
    more information http://www.cse.yorku.ca/~oz/hash.html .
*/
static uint32_t get_hash(const char *key) {
    uint32_t hash = 5381;
    // pointer to start of string
    key++;
    while (*key) {
        hash = ((hash << 5) + hash) + *key;
        key++;
    }
    return hash;
} 

/* 
    double hashing function for resolve hash collisions.
    more information https://en.wikipedia.org/wiki/Double_hashing .
    returns hash.
*/
static int32_t hash(const char *key, const int32_t num, const int32_t try) {
    int32_t hash_a = get_hash(key);
    int32_t hash_b = get_hash(key);
    // variables: try is attempts, num is array size
    // formula: hash_a(key) + try * hash_b(key) mod size
    // if hash_b == 0 then 1. This guarantees that hash_b never be zero 
    // if hash_b > 0 then hash_b 
    return (hash_a + try * (hash_b == 0 ? 1 : hash_b)) % num;
}

/* 
    function creates a new table in memory.
    returns pointer to table.
*/
tb_hash_table *tb_create_hash_table(size_t size){
    tb_hash_table *table = malloc(sizeof(tb_hash_table));
    // size_t is unsigned int32 
    table->size = (uint32_t)size;
    table->count = 0;
    // returns pointer to allocated memory for all items
    table->items = calloc(size, sizeof(tb_hash_table_item));
    table->empty = 1;
    return table;
}

/* 
    function inserts value by key into table.
    nothing to returns
*/
void tb_insert_item(tb_hash_table *table, const char *key, const void *val) {
    tb_hash_table_item *new_item, *current_item; 
    int32_t index, try;
    // get new hash_table_item, value by key 
    new_item = tb_new_table_item(key, val);
    // get hash
    index = hash(new_item->key, table->size, 0);
    // get current item
    current_item = table->items[index];
    // number of attemps
    try = 1;
    while (current_item) { 
        // check if item is not deleted ( if item exists )
	if (current_item != &DELETED) {
            // if item exists, replace value by key
            if (strcmp(current_item->key, key) == 0) {
                // delete current value by key, set new value by key
                tb_delete_table_item(current_item);
                table->items[index] = new_item;
                return;
            }
        // if item is deleted, stop cycle     
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
    table->empty = 0;
}

/* 
    function gets value by key.
    returns pointer to value.
*/
void *tb_get_value(tb_hash_table *table, const char *key) {
    int32_t index, try;
    // number of attempts
    try = 1;
    // get new hash
    index = hash(key, table->size, 0);
    // get item
    tb_hash_table_item *item = table->items[index];
    while (item) {
	// check if item is not deleted
	if (item != &DELETED) {
            // check key and item.key 
            if (strcmp(item->key, key) == 0) {
                // return value by key
                return item->val;
            }
        }
	// get new item, +1 attempts    
        index = hash(key, table->size, try);
        item = table->items[index];
        try++;
    }
    // if nothing if found, return NULL
    return NULL;
}

/* 
    function removes value by key from table.
    nothing to returns.
*/
void tb_delete_item(tb_hash_table *table, const char *key) {
    int32_t index, try;
    // number of attempts
    try = 1;
    // get new hash
    index = hash(key, table->size, 0);
    // get item
   tb_hash_table_item *item = table->items[index];
    while (item) {
        if (strcmp(item->key, key) == 0) {
            // remove item from memory
            tb_delete_table_item(item);
            // set this item is deleted to table
            table->items[index] = &DELETED;
            // stop iteration
            break;
        }
	// get new item, +1 attempts
        index = hash(key, table->size, try);
		item = table->items[index];
        try++;
    }
    // set new count of items into table
    table->count--;
    if (table->count == 0) {
        table->empty = 1;
    }
}

/* 
    function removes table from memory.
    nothing of return.
*/
void tb_delete_hash_table(tb_hash_table *table) {
    tb_hash_table_item *item;
    // iteration over all items
    for (uint32_t index = 0; index < table->size; index++) {
	item = table->items[index];
	// check if item is not deleted or item is not NULL
        if (item && item != &DELETED) {
            // remove item from memory
            tb_delete_table_item(item);
        }
    }
    // remove table from memory
    free(table->items), free(table);
}   

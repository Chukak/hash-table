/* 
    See hashtable.h for more info about struct `tb_hash_table`, `tb_hash_table_item`.
*/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>

#include "hashtable.h"


// Raises segmentation fault.
#define SEG raise(SIGSEGV)

/* 
    A static variable DELETED, for check all deleted items in table.
    Key and val must be null.
*/
static tb_hash_table_item DELETED = {NULL, NULL};

/* 
    A static function, creates a new `tb_hash_table_item`.
    Puts a value by key in the table and returns pointer to item.
    Item example: {'key' : value} .

    `strdup` is not ANSI function. 
    `strdup` call malloc function, allocates memory for a string, 
	copies string in this place and returns a pointer to the string.
*/
static tb_hash_table_item *tb_new_table_item(const char *key, const void *val) {
    tb_hash_table_item *item = malloc(sizeof(tb_hash_table_item));
    item->key = strdup(key); 
    item->val = malloc(sizeof(void *));
    memcpy(item->val, val, sizeof(void *));
    return item;
}

/*
    A static function, removes `tb_hash_table_item` from memory.
    Nothing to returns.
 */
static void tb_delete_table_item(tb_hash_table_item *item) {
    free(item->key);
    free(item->val);
    free(item);
}

/*
    The `djb2` funciton.
    More information: http://www.cse.yorku.ca/~oz/hash.html.
 */
static uint64_t get_hash(const char *key) {
    uint64_t h = 5381;
    int c;
    while ((c = *key++)) {
        h = h * 33 + c;
    }
    return h;
}

/* 
    A static funtion, returns a new hash for strings.
 */
static uint64_t get_hash2(const char *key) {
    uint64_t h = 0;
    for (uint32_t i = 0; i < (uint32_t)strlen(key); i++) {
        h = ((h * 31) + (uint64_t)key[i]);
    }
    return h;
}

/* 
    The double hashing function for resolving hash collisions.
    More information: https://en.wikipedia.org/wiki/Double_hashing .
    Returns the hash.
*/ 
static uint32_t hash(const char *key, const uint32_t num, const uint32_t try) {
    uint64_t hash_a = get_hash(key);
    uint64_t hash_b = get_hash2(key);
    //printf("\nkey: %s hash_a: %lu hash_b: %lu", key, hash_a, hash_b);
    // variables: try is attempts, num is array size
    // formula: `hash_a(key) + try + (hash_b(key) + 1) mod size`
    return (uint32_t)(hash_a + (try + (hash_b + 1))) % num;
} 

/* 
    The function creates a new table in memory.
    Returns a pointer to the table.
*/
tb_hash_table *tb_create_hash_table(uint32_t size) {
    if (size <= 0) {
        return (void *)0;
    }
    tb_hash_table *table = (tb_hash_table *)malloc(sizeof(tb_hash_table));
    // size_t is unsigned int 
    table->size = size;
    table->count = 0;
    // returns a pointer to the allocated memory for all items
    table->items = (tb_hash_table_item **)malloc(size * sizeof(tb_hash_table_item));
    if (table->items == NULL) {
        return (NULL);
    }
    memset(table->items, 0, size * sizeof(tb_hash_table_item));
    table->empty = 1;
    return table;
}

/*
    Returns the `tb_hash_table_item` object, if an item with this key exists. 
    Otherwise returns NULL.
 */
tb_hash_table_item *tb_find_item(tb_hash_table * table, const char *key) {
    if (table->size) {
        uint32_t index, try;
        index = hash(key, table->size, 0);
        try = 1;
        tb_hash_table_item *item = table->items[index];
        while (item != NULL) {
            // check if an item is not deleted ( if an item exists )
            if (item != &DELETED) {
                if (strcmp(item->key, key) == 0) {
                    return item;
                }
            }
            // get a new hash
            index = hash(key, table->size, try);
            // get a new current item
            item = table->items[index];
            // +1 attempts
            try++;
        }
        return NULL;
    }
    return NULL;
}

/*
    Returns the item in the position. If the item is deleted returns `{NULL, NULL}`.
    Otherwise returns NULL.
 */
tb_hash_table_item *tb_item_at(tb_hash_table *table, uint32_t pos) {
    if (pos <= table->size) {
        tb_hash_table_item *item = table->items[pos];
        if (item != &DELETED) {
            return item;
        } else {
            return &DELETED;
        } 
    }
    return NULL;
}

/* 
    The function inserts a value by key into the table.
    Returns the position of the item.
*/
uint32_t tb_insert_item(tb_hash_table *table, const char *key, const void *val) {
    tb_hash_table_item *new_item, *current_item; 
    uint32_t index, try;
    // get a new item
    new_item = tb_new_table_item(key, val);
    // get hash
    index = hash(new_item->key, table->size, 0);
    // get a current item
    current_item = table->items[index];
    // number of attemps
    try = 1;
    while (current_item != NULL) {
        // check if an item is not deleted ( if an item exists )
	if (current_item != &DELETED) {
            // if an item exists, replace a value by key
            if (strcmp(current_item->key, key) == 0) {
                // delete a current value by key, set a new value by key
                tb_delete_table_item(current_item);
                table->items[index] = new_item;
                return index;
            }
            if (table->size == table->count) {
                SEG;
            }
        // if an item is deleted, stop cycle     
        } else {
            break;
        }
        // get a new hash
        index = hash(new_item->key, table->size, try);
	// get a new current item
        current_item = table->items[index];
        // +1 attempts
        try++;
    }
    // set a new item and count
    table->items[index] = new_item;
    table->count++;
    table->empty = 0;
    return index;
}

/* 
    The function gets a value by key.
    Returns the pointer to a value, if the value by key exists. 
    Otherwise returns NULL
*/
void *tb_get_value(tb_hash_table *table, const char *key) {
    if (table->empty) {
        return NULL;
    }
    int32_t index, try; 
    uint32_t ch = 0;
    // number of attempts
    try = 1;
    // get a new hash
    index = hash(key, table->size, 0);
    // get an item
    tb_hash_table_item *item = table->items[index];
    while (item != NULL) {
        // check if an item is not deleted
	if (item != &DELETED) {
            // check key and item.key 
            if (strcmp(item->key, key) == 0) {
                // return a value by key
                return item->val;
            } else {
                ch++;
            }
        }
        if (ch == table->count) { 
            break; 
        }
	// get a new item, +1 attempts    
        index = hash(key, table->size, try);
        item = table->items[index];
        try++;
    }
    // if nothing if found, return NULL
    return NULL;
}

/* 
    The function gets the item by key.
    Returns the pointer to the item, if the item with this key exists. 
    Otherwise returns NULL
*/
tb_hash_table_item *tb_get_item(tb_hash_table *table, const char *key) {
    int32_t index, try;
    uint32_t ch = 0;
    // number of attempts
    try = 1;
    // get a new hash
    index = hash(key, table->size, 0);
    // get an item
    tb_hash_table_item *item = table->items[index];
    while (item != NULL) {
        // check if an item is not deleted
	if (item != &DELETED) {
            // check key and item.key 
            if (strcmp(item->key, key) == 0) {
                // return a value by key
                return item;
            } else {
                ch++;
            }
        }
        if (ch == table->count) {
            break; 
        }
	// get a new item, +1 attempts    
        index = hash(key, table->size, try);
        item = table->items[index];
        try++;
    }
    // if nothing if found, return NULL
    return NULL;
}

/* 
    The function removes a value by key from table.
    Returns 1 if the deletion is successful, otherwise returns 0.
    Also, if the key is not in the table, returns 0.
*/
int tb_delete_item(tb_hash_table *table, const char *key) {
    if (!table->count) {
            return 0;
    }
    int32_t index, try;
    uint32_t ch = 0;
    // number of attempts
    try = 1;
    // get a new hash
    index = hash(key, table->size, 0);
    // get an item
    tb_hash_table_item *item = table->items[index];
    while (item != NULL) {
        if (item != &DELETED) {
            if (strcmp(item->key, key) == 0) {
                // remove an item from memory
                tb_delete_table_item(item);
                // set this item is deleted to table
                table->items[index] = &DELETED;
                // set a new count of items into table
                table->count--;
                if (table->count == 0) {
                    table->empty = 1;
                }
                // stop iteration
                return 1;
            } else {
                ch++;
            } 
        }
        if (ch == table->count) { 
            break; 
        }
	// get a new item, +1 attempts
        index = hash(key, table->size, try);
	item = table->items[index];
        try++;
    }
    return 0;
}

/* 
    The function removes the table from memory.
    Nothing of returns.
*/
void tb_delete_hash_table(tb_hash_table *table) {
    tb_hash_table_item *item;
    // iteration over all items
    for (uint32_t index = 0; index < table->size; index++) {
	item = table->items[index];
	// check if an item is not deleted or an item is not NULL
        if (item && item != &DELETED) {
            // remove an item from memory
            tb_delete_table_item(item);
        }
    }
    // remove the table from memory
    free(table->items), free(table);
}   

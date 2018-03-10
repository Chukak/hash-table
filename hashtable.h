/* 
    Instead unsigned char, is used char in all the program.
*/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>


/* 
    hash table item struct.
    key, val is pointers.
    key point to value from table {'key' : 'value'}.
    val is value by this key from table.
    key and value must be string or char.
*/
typedef struct {
    char *key;
    char *val;
} hash_table_item;

/* 
    hash table struct.
    size is size of table.
    count is sum elements in table.
    items if array of pointers, which point to has_table_items, which have value by key.
    size and count must be unsigned int and more that 0.
*/
typedef struct {
    uint32_t size;
    uint32_t count;
    hash_table_item **items;
} hash_table;

// functions from hastable.c
hash_table *create_hash_table(size_t size);
void insert_item(hash_table *table, const char *key, const char *val);
char *get_value(hash_table *table, const char *key);
void delete_item(hash_table *table, const char *key);
void delete_hash_table(hash_table *table);


#endif


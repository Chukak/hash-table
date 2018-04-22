#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>


/* 
    hash table item struct.
    key, val is pointers.
    val is value by this key from table.
    key and value must be string or char.
*/
typedef struct hash_table_item {
    char *key;
    void *val;
} tb_hash_table_item;

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
    tb_hash_table_item **items;
    int empty;
} tb_hash_table;

// functions from hastable.c
tb_hash_table *tb_create_hash_table(size_t size);
void tb_insert_item(tb_hash_table *table, const char *key, const void *val);
void *tb_get_value(tb_hash_table *table, const char *key);
void tb_delete_item(tb_hash_table *table, const char *key);
void tb_delete_hash_table(tb_hash_table *table);


#endif


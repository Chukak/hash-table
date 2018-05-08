#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>

/* 
    A macro to get an integer value. 
 */
#define TB_I(pointer) ({ \
    int *result = pointer; \
    *result; \
})

/* 
    A macro to get a float value. 
 */
#define TB_F(pointer) ({ \
    float *result = pointer; \
    *result; \
})    

/* 
    A macro to get a double value. 
 */
#define TB_D(pointer) ({ \
    double *result = pointer; \
    *result; \
})

/* 
    A macro to get a char. 
 */
#define TB_C(pointer) ({ \
    char *result = pointer; \
    *result; \
})

/* 
    A macro to get a string. 
 */
#define TB_STR(pointer) ({ \
    char **result = pointer; \
    *result; \
})

/* 
    A macro to get the value of random type. 
 */
#define TB_CUSTOM_TYPE(type, pointer) ({ \
    __typeof__(type*) value = pointer; \
    *value; \
})


/* 
    hash table item struct.
    key, val is pointers.
    val is value by this key from table.
    key must be string.
    value must be poiter to object. 
*/
typedef struct hash_table_item {
    char *key;
    void *val;
} tb_hash_table_item;

/* 
    hash table struct.
    size is size of table.
    count is sum elements in table.
    empty is 1 or 0. 
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


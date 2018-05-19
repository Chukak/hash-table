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
    A macro to get the value of a random type. 
 */
#define TB_CUSTOM_TYPE(type, pointer) ({ \
    __typeof__(type*) value = pointer; \
    *value; \
})


/* 
    
    The hash table item struct.
    `key`, `val` is pointers.
    `val` is value by this `key` from table.
    `key` must be a string.
    `val` must be a pointer to an object. 
*/
typedef struct {
    char *key;
    void *val;
} tb_hash_table_item;

/* 
    The hash table struct.
    `size` is the size of the table.
    `count` is the sum of the elements in the table.
    `empty` is 1 or 0.  
    `items` is an array of pointers.
    `size` and `count` must be unsigned int and greater that 0.
*/
typedef struct {
    uint32_t size;
    uint32_t count;
    tb_hash_table_item **items;
    int empty;
} tb_hash_table;

// The functions from `hastable.c`
tb_hash_table_item *tb_get_item(tb_hash_table *table, const char *key);
tb_hash_table_item *tb_item_at(tb_hash_table *table, uint32_t pos);
tb_hash_table_item *tb_find_item(tb_hash_table *table, const char* key);
tb_hash_table *tb_create_hash_table(size_t size);
uint32_t tb_insert_item(tb_hash_table *table, const char *key, const void *val);
void *tb_get_value(tb_hash_table *table, const char *key);
int tb_delete_item(tb_hash_table *table, const char *key);
void tb_delete_hash_table(tb_hash_table *table);


#endif

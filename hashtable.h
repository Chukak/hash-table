#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif    

/* 
    A macro to get an integer value. 
 */
#define GET_INT(pointer) *((int *)pointer)

/* 
    A macro to get a float value. 
 */
#define GET_FLOAT(pointer) *((float *)pointer)

/* 
    A macro to get a double value. 
 */
#define GET_DOUBLE(pointer) *((double *)pointer)

/* 
    A macro to get a char. 
 */
#define GET_CHAR(pointer) *((char *)pointer)

/* 
    A macro to get a string. 
 */
#define GET_STR(pointer) *((char **)pointer)

/* 
    A macro to get the value of a random type. 
 */
#define GET_CUSTOM_TYPE(type, pointer) ({ \
    __typeof__(type*) value = pointer; \
    *value; \
})


/*   
    The hashtable item struct.
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
    uint32_t allocated;
    uint32_t size;
    uint32_t count;
    tb_hash_table_item **items;
    int empty;
} tb_hash_table;

/*
    A variable EMPTY_ITEM, for check all EMPTY_ITEM items in table.
    Key and val must be null.
*/
extern tb_hash_table_item *EMPTY_ITEM;

// The functions from `hastable.c`
tb_hash_table_item *tb_get_item(const tb_hash_table * const table, const char* key);
tb_hash_table_item *tb_item_at(const tb_hash_table * const table, uint32_t pos);
tb_hash_table_item *tb_find_item(const tb_hash_table * const table, const char* key);
tb_hash_table *tb_create_hash_table(uint32_t size);
int64_t tb_insert_item(tb_hash_table *table, const char* key, const void* val);
void *tb_get_value(const tb_hash_table * const table, const char* key);
int tb_delete_item(tb_hash_table *table, const char* key);
void tb_delete_hash_table(tb_hash_table *table);

#ifdef __cplusplus
}
#endif

#endif

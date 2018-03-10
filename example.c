/* 
    Instead unsigned char, is used char in all the program.
    See hashtable.h for more info about struct hash_table type. 
*/
#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"

/* 
    Call this function for check work hash table.
*/
void example() {
    // create table function, size table = 50.
    // 
    hash_table *table = create_hash_table(50);
    /* 
        insert_item(*table, *key, *val);
	insert in table item {'key1' : 'val1'}. 
	key = 'key1', value = 'val1'.
	key, val is pointers. 
	table is struct hash_table type, see in hastable.h.
    */
    insert_item(table, "key1", "val1");
    insert_item(table, "key2", "val2");
    /* 
	get_value(*table, *key);
	get value by key from table.
	key is pointer.
	table is struct hash_table type.
    */
    char *d = get_value(table, "key2");
    // print value by key2
    printf(" Key2: %s\n", d);
    d = get_value(table, "key1");
    // print value by key1
    printf(" Key1: %s\n", d);
    /* 
	delete_item(*table, *key);
	delete value by key from table.
	key is pointer.
	table is hash_table type.
    */
    delete_item(table, "key1");
    /* 
	after removal, this func return NULL type.
        can check NULL - if (d) { more }.
    */    
    d = get_value(table, "key1");
    // print Key1: (null)
    printf(" Key1: %s\n", d);
    /* 
        if key is in the table, and value for this key changes, then. 
	insert_item func, rewrite value by this key in table.
	old value delete, new value available by key. 
    */
    insert_item(table, "key2", "val12");
    d = get_value(table, "key2");
    // print key2 new val: val12.
    printf(" Key2 new val: %s\n", d);
    // commented string
    //!!!
    //delete_hash_table(table);
    //!!!
    // this string will cause to segmentation fault error, after delete hash table.
    // 
    d = get_value(table, "key2");
    printf(" Key2 l: %s\n", d);
    /* 
	delete hash table, all items, pointers and exempts memory.
	this func call free() for table, items and pointers.
    */
    delete_hash_table(table);
} 

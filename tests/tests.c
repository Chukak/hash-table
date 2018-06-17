/* 
    Instead unsigned char, is used char in all the program.
    See hashtable.h for more info about struct hash_table type. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hashtable.h"

/* 
    Call this function for check work hash table.
*/

typedef struct {
    char *data;
} Example;


int test_create_table() {
    tb_hash_table *table = tb_create_hash_table(6);
    if (!table->empty) {
        return 1;
    }
    tb_delete_hash_table(table);
    return 0;
}

int test_insert_get_values() {
    tb_hash_table *table = tb_create_hash_table(6);
    int a = 15;
    tb_insert_item(table, "key1", &a);
    if (table->count != 1) {
        return 1;
    }
    int *a_value = tb_get_value(table, "key1");
    if (*a_value != a) {
        return 1;
    }
    int a_value_macro = TB_I(tb_get_value(table, "key1"));
    if (a_value_macro != a) {
        return 1;
    }
    char c = 'A';
    tb_insert_item(table, "key2", &c);
    if (table->count != 2) {
        return 1;
    }
    char *c_value = tb_get_value(table, "key2");
    if (*c_value != c) {
        return 1;
    }
    char *arr = "example";
    tb_insert_item(table, "key3", &arr);
    if (table->count != 3) {
        return 1;
    }
    char **arr_value = tb_get_value(table, "key3");
    for (int yu = 0; yu < strlen(arr); yu++) {
        if (arr[yu] != (*arr_value)[yu]) {
            return 1;
        }
    }
    char *arr_value_macro = TB_STR(tb_get_value(table, "key3"));
    for (int yu = 0; yu < strlen(arr); yu++) {
        if (arr[yu] != arr_value_macro[yu]) {
            return 1;
        }
    }
    double d = 1.5463;
    tb_insert_item(table, "key4", &d);
    if (table->count != 4) {
        return 1;
    }
    double *d_value = tb_get_value(table, "key4");
    if (d != *d_value) {
        return 1;
    }
    float f = 25.34;
    tb_insert_item(table, "key5", &f);
    if (table->count != 5) {
        return 1;
    }
    float *f_value = tb_get_value(table, "key5");
    if (f != *f_value) {
        return 1;
    }
    
    Example struct_example = {"example data"};
    tb_insert_item(table, "key6", &struct_example);
    if (table->count != 6) {
        return 1;
    }
    Example *struct_example_value = tb_get_value(table, "key6");
    if (struct_example.data != struct_example_value->data) {
        return 1;
    }
    Example struct_example_value_macro = TB_CUSTOM_TYPE(Example, tb_get_value(table, "key6"));
    if (struct_example.data != struct_example_value_macro.data) {
        return 1;
    } 
    tb_delete_hash_table(table); 
    return 0;
}

int test_delete_items() {
    tb_hash_table *table = tb_create_hash_table(10000);
    char *key = malloc(sizeof(char *));
    for (int i = 1; i < 10001; i++) {
        sprintf(key, "key_%i", i);
        uint32_t pos = tb_insert_item(table, key, &i);
        if (pos >= 0 && pos <= table->size) 
            continue;
        else
            break;
    }
    if (table->count != 10000) {
        return 1;
    }
    free(key);
    
    tb_hash_table_item *item = tb_find_item(table, "key_9999");
    int *a = item->val;
    if (*a != 9999) {
        return 1;
    }
    
    char *key_2 = malloc(sizeof(char *));
    for (int i = 1; i < 35; i++) {
        sprintf(key_2, "key_%i", i);
        int del = tb_delete_item(table, key_2);
        if (!del) {
            return 1;
        } 
    } 
    free(key_2);
    
    tb_delete_hash_table(table);
    return 0;
}

int test_extra() {
    tb_hash_table *table = tb_create_hash_table(2);
    char *key = malloc(sizeof(char *));
    for (int i = 1; i < 3; i++) {
        sprintf(key, "key_%i", i);
        uint32_t pos = tb_insert_item(table, key, &i);
        if (pos == i) {
            continue;
        }
    }
    
    tb_hash_table_item *item = tb_get_item(table, "key_67823");
    if (item) {
        return 1;
    }
    
    tb_hash_table_item *item2 = tb_item_at(table, 67823);
    if (item2) {
        return 1;
    }
    
    tb_delete_hash_table(table);
    return 0;
}

int run_tests() {
    printf("The tests are runned.\n");
    if (test_create_table()) {
        printf("Test №1 failed.\n");
        return 1;
    };
    if (test_insert_get_values()) {
        printf("Test №2 failed.\n");
        return 1;
    };
    if (test_delete_items()) {
        printf("Test №3 failed.\n");
        return 1;
    };
    if (test_extra()) {
        printf("Test №4 failed.\n");
        return 1;
    };
    printf("All the tests are success.\n");
    return 0;
}

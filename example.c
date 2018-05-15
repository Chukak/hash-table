/* 
    Instead unsigned char, is used char in all the program.
    See hashtable.h for more info about struct hash_table type. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

/* 
    Call this function for check work hash table.
*/

typedef struct {
    char *data;
} Example;


void example_create_table() {
    tb_hash_table *table = tb_create_hash_table(6);
    printf("%s", "Example of an empty table");
    printf("\n");
    if (table->empty) {
        printf("%s", "table is empty\n");
    }
    tb_delete_hash_table(table);
    printf("\n");
}

void example_insert_get_values() {
    tb_hash_table *table = tb_create_hash_table(6);
    printf("%s", "Example of the value `integer`");
    printf("\n");
    int a = 15;
    tb_insert_item(table, "key1", &a);
    int *a_value = tb_get_value(table, "key1");
    printf("%i", *a_value);
    printf("\n");
    printf("With macros: ");
    int a_value_macro = TB_I(tb_get_value(table, "key1"));
    printf("%i", a_value_macro);
    printf("\n");
    printf("%s", "Example of the value `char`");
    printf("\n");
    char c = 'A';
    tb_insert_item(table, "key2", &c);
    char *c_value = tb_get_value(table, "key2");
    printf("%c", *c_value);
    printf("\n");
    printf("%s", "Example of the value `char *`");
    printf("\n");
    char *arr = "example";
    tb_insert_item(table, "key3", &arr);
    char **arr_value = tb_get_value(table, "key3");
    printf("%s", *arr_value);
    printf("\n");
    printf("With macros: ");
    char *arr_value_macro = TB_STR(tb_get_value(table, "key3"));
    printf("%s", arr_value_macro);
    printf("\n");
    printf("%s", "Example of the value `double`");
    printf("\n");
    double d = 1.5463;
    tb_insert_item(table, "key4", &d);
    double *d_value = tb_get_value(table, "key4");
    printf("%f", *d_value);
    printf("\n");
    printf("%s", "Example of the value `float`");
    printf("\n");
    float f = 25.34;
    tb_insert_item(table, "key5", &f);
    float *f_value = tb_get_value(table, "key5");
    printf("%f", *f_value);
    printf("\n");
    printf("%s", "Example of the value `random struct` ");
    printf("\n");
    Example struct_example = {"example data"};
    tb_insert_item(table, "key6", &struct_example);
    Example *struct_example_value = tb_get_value(table, "key6");
    printf("struct data %s", struct_example_value->data);
    printf("\n");
    Example struct_example_value1 = TB_CUSTOM_TYPE(Example, tb_get_value(table, "key6"));
    printf("With macro: ");
    printf("struct data %s", struct_example_value1.data);
    printf("\n");
    printf("pointer on example struct %p", struct_example_value);
    printf("\n");
    tb_delete_hash_table(table);
    printf("\n");
}

void example_delete_items() {
    tb_hash_table *table = tb_create_hash_table(5000);
    for (int i = 1; i < 5001; i++) {
        char *key = malloc(sizeof(char *));
        sprintf(key, "key_%i", i);
        uint32_t pos = tb_insert_item(table, key, &i);
        if (pos == i) {
            continue;
        }
    }
    
    printf("size: %u\n", table->count);

    for (int i = 1; i < 5001; i++) {
        char *key = malloc(sizeof(char *));
        sprintf(key, "key_%i", i);
        int del = tb_delete_item(table, key);
        if (del) {
            continue;
        }
    }
    
    printf("size: %u", table->count);
    tb_delete_hash_table(table);
    printf("\n");
}

void example() {
    example_create_table();
    example_insert_get_values();
    example_delete_items();
}

/* 
    Instead unsigned char, is used char in all the program.
    See hashtable.h for more info about struct hash_table type. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "tests.h"
#include "hashtable.h"

enum TYPES {
    BOOL = 0,
    CHAR = 1,
    SHORT_INT = 2,
    INT = 3,
    LONG_INT = 4,
    LONG_LONG_INT = 5,
    FLOAT = 6,
    LONG_DOUBLE = 7,
    VOID_PTR = 8,
    UNSIGNED_CHAR = 9,
    SIGNED_CHAR = 10,
    UNSIGNED_SHORT_CHAR = 11,
    UNSIGNED_SHORT_INT = 12,
    UNSIGNED_INT = 13,
    UNSIGNED_LONG_INT = 14,
    UNSIGNED_LONG_LONG_INT = 15,
    DOUBLE = 16,
    CHAR_PTR = 17,
    UNKNOWN = 18
};

#define typename(x) _Generic((x), \
    _Bool: BOOL, \
    unsigned char: UNSIGNED_CHAR, \
    char: CHAR, \
    signed char: SIGNED_CHAR, \
    short int: SHORT_INT, \
    unsigned short int: UNSIGNED_SHORT_INT, \
    int: INT, \
    unsigned int: UNSIGNED_INT, \
    long int: LONG_INT, \
    unsigned long int: UNSIGNED_LONG_INT, \
    long long int: LONG_LONG_INT, \
    unsigned long long int: UNSIGNED_LONG_LONG_INT, \
    float: FLOAT, \
    double: DOUBLE, \
    long double: LONG_DOUBLE, \
    char *: CHAR_PTR, \
    void *: VOID_PTR, \
    default: UNKNOWN)


#define NORMAL "\x1B[0m"
#define RED "\x1B[31m"

#define RUN_TEST(name) __##name##__()

#define TEST(test_section)  void __##test_section##__() \

#define ACTUAL_TRUE(result) { \
    assert((result)); \
}

#define EXPECT_TRUE(result) { \
    if (!(result)) {\
        printf("%sEXPECT_TRUE failed! %s Test: %s. Line:%i \n", RED, NORMAL, __func__, __LINE__);\
    }\
}

#define EXPECT_FALSE(result) { \
    if ((result)) {\
        printf("%sEXPECT_FALSE failed! %s Test: %s. Line:%i \n", RED, NORMAL, __func__, __LINE__);\
    }\
}

#define EXPECT_EQ(t1, t2) { \
    if (typename(t1) != CHAR_PTR && typename(t2) != CHAR_PTR) { \
        if (t1 != t2) {\
            printf("%sEXPECT_EQ failed! %s Test: %s. Line:%i \n", RED, NORMAL, __func__, __LINE__);\
        } \
    } \
}

#define EXPECT_STRINGS_EQ(s1, s2) { \
    if (strcmp(s1, s2) != 0) { \
       printf("%sEXPECT_STRINGS_EQ failed! %s Test: %s. Line:%i \n", RED, NORMAL, __func__, __LINE__);\
    } \
}

TEST(test_insert_table) {
    clock_t begin = clock();
    tb_hash_table *table = tb_create_hash_table(100000);
    EXPECT_TRUE(table->empty);
    EXPECT_EQ(table->size, 100000);
    EXPECT_EQ(table->count, 0);
    char *key = malloc(sizeof(char *));
    uint32_t saved_count = 0;
    ++saved_count;
    for (int i = 0; i < 100000; ++i, ++saved_count) {
        sprintf(key, "key_%i", i);
        tb_insert_item(table, key, &i);
        EXPECT_EQ(table->count, saved_count);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Insertion perfomance of table - 100000 items: %f ms \n", time_spent);
    EXPECT_FALSE(table->empty);
    EXPECT_EQ(table->count, 100000);
    tb_delete_hash_table(table);
}

TEST(test_get_value_from_table) {
    tb_hash_table *table = tb_create_hash_table(100000);
    EXPECT_TRUE(table->empty);
    EXPECT_EQ(table->size, 100000);
    EXPECT_EQ(table->count, 0);
    char *key = malloc(sizeof(char *));
    uint32_t saved_count = 0;
    ++saved_count;
    for (int i = 0; i < 100000; ++i, ++saved_count) {
        sprintf(key, "key_%i", i);
        tb_insert_item(table, key, &i);
        EXPECT_EQ(table->count, saved_count);
    }
    EXPECT_FALSE(table->empty);
    EXPECT_EQ(table->count, 100000);
    clock_t begin = clock();
    for (int i = 0; i < 100000; ++i) {
        sprintf(key, "key_%i", i);
        tb_hash_table_item *item = tb_get_item(table, key);
        EXPECT_STRINGS_EQ(item->key, key);
        EXPECT_TRUE(GET_INT(item->val) == i);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("'tb_get_item' function perfomance of table - 100000 items: %f ms \n", time_spent);

    clock_t begin2 = clock();
    for (int i = 0; i < 100000; ++i) {
        sprintf(key, "key_%i", i);
        void * value = tb_get_value(table, key);
        EXPECT_TRUE(GET_INT(value) == i);
    }
    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    printf("'tb_get_value' function perfomance of table - 100000 items: %f ms \n", time_spent2);
    tb_delete_hash_table(table);
}

TEST(test_delete_value_from_table) {
    tb_hash_table *table = tb_create_hash_table(100000);
    EXPECT_TRUE(table->empty);
    EXPECT_EQ(table->size, 100000);
    EXPECT_EQ(table->count, 0);
    char *key = malloc(sizeof(char *));
    uint32_t saved_count = 0;
    ++saved_count;
    for (int i = 0; i < 100000; ++i, ++saved_count) {
        sprintf(key, "key_%i", i);
        tb_insert_item(table, key, &i);
        EXPECT_EQ(table->count, saved_count);
    }
    EXPECT_FALSE(table->empty);
    EXPECT_EQ(table->count, 100000);
    uint32_t saved_count2 = table->count;
    --saved_count2;
    clock_t begin = clock();
    for (int i = 0; i < 100000; ++i, --saved_count2) {
        sprintf(key, "key_%i", i);
        tb_hash_table_item *item = tb_get_item(table, key);
        EXPECT_STRINGS_EQ(item->key, key);
        EXPECT_TRUE(GET_INT(item->val) == i);
        void * value = tb_get_value(table, key);
        EXPECT_TRUE(GET_INT(value) == i);
        int del = tb_delete_item(table, key);
        ACTUAL_TRUE(del);
        item = tb_get_item(table, key);
        EXPECT_TRUE(item == NULL);
        value = tb_get_value(table, key);
        EXPECT_TRUE(value == NULL);
        EXPECT_EQ(table->count, saved_count2);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Deletion perfomance of table - 100000 items: %f ms \n", time_spent);
    tb_delete_hash_table(table);
}


void run_tests() {
    RUN_TEST(test_insert_table);
    RUN_TEST(test_get_value_from_table);
    RUN_TEST(test_delete_value_from_table);
}

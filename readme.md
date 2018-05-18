# hash-table

## Install
Clone this repository 
```bash
git clone https://github.com/Chukak/hash-table.git
```
Run make
```bash
make -f Makefile
```
If you need the `.so` library:
```bash
make -f LibraryMakefile
```

## How to use
### Create table
Create a table with a static size. Call `tb_create_hash_table` function. Pass the size as the first parameter.
Returns a pointer on the table or NULL.
```c
tb_hash_table *table = tb_create_hash_table(size);
```
If the size is zero, returns NULL.
```c
tb_hash_table *table = tb_create_hash_table(0);
if (!table)
    ...
```

### Insert items
Create the variable, that you want to put to the table. 
Call `tb_insert_item` function with table, key and value. 
Pass the table as the first parameter, the name of the key as the second, and the address on your variable as the third.
Returns the position of the item. The position is a `unsigned int` or `uint32_t` from `stdint.h`.
```c
int a = 1;
unsigned int pos = tb_insert_item(table, "key1", &a);
char *string = "example";
unsigned int pos = tb_insert_item(table, "key2", &string);
```

### Change value
Call `tb_insert_item` function with the name of the key, if you want to change the value by key.
```c
int *value = tb_get_value(table, "key1");
printf("%i", *value); // print 1
int b = 2;
tb_insert_item(table, "key1", &b);
int *new_value = tb_get_value(table, "key1");
printf("%i", *new_value); // print 2
```

### Get value by key
To get the value call `tb_get_value` function with table and key. 
Pass the table as the first parameter and the name of the key as the second.
Returns a pointer on the value by key. If an item does not exists in the table, returns NULL;
```c
int *value = tb_get_value(table, "key1");
printf("%i", *value); // print 1
// function returns pointer on `char *`
char *example = "example";
tb_insert_item(table, "key2", &example);
char **string = tb_get_value(table, "key2");
printf("%i", *string); // print "example"
```

### Delete items
To delete the value by key call `tb_delete_item` funtion. 
Pass the table as the first parameter and the name of the key as the second.
Returns 1 if the item is deleted, otherwise returns 0.
```c
int check = tb_delete_item(table, "key1");

type *value = tb_get_value(table, "key1");
printf("%p", value); // print (nil)
```

### Get an item 
To get the item call `tb_get_item` function. Pass the table as the first parameter, the key as the second.
Returns the pointer to an item or NULL, if an item does not exists. 
```c
tb_hash_table_item *item = tb_get_item(table, "key");
printf("%s", item->key);
```

### Get an item from the position
To get the item from the position call `tb_item_at` function. 
Pass the table as the first parameter, the position as the second.
Returns the pointer to an item or NULL, if an item does not exists. If an item is deleted, returns pointer to the structure of `{NULL, NULL}`.
```c
tb_hash_table_item *item = tb_item_at(table, 1);
```

### Find an item
To find the item call `tb_find_item` function. 
Pass the table as the first parameter, the key as the second.
Returns the pointer to an item or NULL, if item does not exists. 
```c
tb_hash_table_item *item = tb_find_item(table, "key");
```

### For loop
In the `for` loop, you must check the pointer to the item. 
```c
for (unsigned int i = 0; i < table->size; i++) {
    tb_hash_table_item *item = table.items[i];
    if (item != NULL) {
      ...
    }
}
```
If the index is out of range `0` to `table->size`:
```c
for (unsigned int i = 0; i < table->size + 10; i++) {
    unsigned int pos = tb_insert_item(table, "key"); // raises seg fault
}
```

```c
for (unsigned int i = 0; i < table->size + 10; i++) {
    void *p = tb_get_value(table, "key"); // returns NULL 
}
```

```c
for (unsigned int i = 0; i < table->size + 10; i++) {
    int delete = tb_delete_item(table, "key"); // returns 0 
}
```

```c
tb_hash_table_item *item = tb_item_at(table, index); returns NULL;
```

### Delete table
To delete table call `tb_delete_hash_table` function. Pass the table as the first parameter.
```c
tb_delete_hash_table(table);
```

## Python
### How to compile
This library can be compiled as a python module. For example, in Linux:
```bash
cd python
python setup.py built_ext --inplace
```
In python:
```python
from hashtable import Table
```

### How to use
#### Create table
Create a table with a static size. Create the `Table` class. Pass the size as the first parameter.
```python
table = Table(size);
```

#### Insert items
Call the `insert` method from the `Table` class. Pass the name of the key as the first parameter, the value as the second.
Returns the position of the item.
```python
pos = table.insert("key1", 45)
```

#### Change value
Call `insert` method from the `Table` class with the name of the key, if you want to change the value by key.
Pass the name of the key as the first parameter, the value as the second. 
```python
table.insert("key2", 45)
print(table.get("key2")) # 45
table.insert("key2", 64)
print(table.get("key2")) #64
```
#### Get value by key
To get the value call `get` method fron the `Table` class with table and key. Returns the value.
Pass the name of the key as the first parameter. Returns the value by key. If an item does not exists in the table, returns None;

```python
a = table.get("key2")
print(a) 
```

#### Delete items
To delete value by key call `delete` method from the `Table` class. Returns `True` if the deletion is successful otherwise `False`. Pass the name of the key as the first parameter.
```python
deletion = table.delete("key1");
if deletion: 
    ...
a = table.get("key1")
print(a) # None
```

#### Get an item from the position
To get the item from the position call `at` method from the `Table` class. 
Pass the the position as the first parameter.
Returns the item or ("", None), if an item does not exists. If an item is deleted, returns the tuple of `(None, None)`.
```python
key, value = table.at(1);
```

### Find an item
To find the item call `find` method from the `Table` class. 
Pass the key as the first parameter.
Returns the item or ("", None), if item does not exists. 
```python
key, value = table.find("key");
```

#### Delete table
To delete a table call `del`. 
```python
del table
```

#### Tests
For testing check this [page](https://github.com/Chukak/hash-table/blob/master/python/readme.md).

### Macros (Only C/C++)
You can be use macros to get value from `void *`. For example:
```c
int a = TB_I(tb_get_value(table, "key"));
printf("%i", a);
```
#### Macros
* `TB_I` - returns an integer value from the pointer (`int`).
* `TB_F` - returns a float value from the pointer(`float`).
* `TB_D` - returns a double value from the pointer(`double`).
* `TB_C` - returns a char from pointer(`char`).
* `TB_STR` - returns string(`char *`).
* `TB_CUSTOM_TYPE` - returns a custom type from the pointer. 

A custom type example:
```c
Type t;
tb_insert_item(table, "key", &t);
Type t = TB_CUSTOM_TYPE(tb_get_value(table, "key"));
```



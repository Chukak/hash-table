# hash-table

## Install
Clone this repository 
```bash
git clone https://github.com/Chukak/hash-table.git
```
Create the `.so` library:
```bash
cmake ..
```

## How to use
### Table structure
#### `tb_hash_table` structure:
* `table->size` - table size, type: `unsigned int`.
* `table->count` - number of elements in the table, type: `unsigned int`.
* `table.allocated` - number of bytes which was allocatef for the table. (size * 1.25)
* `table->empty` - the table is empty or not, type: `int`. Can be `1` or `0`.
* `table->items` - the pointer on a chain of pointers. type `tb_hash_table_item **`.

#### `tb_hash_table_item` structure:
* `item->key` - the name of the key, type: `chat *`
* `item->val` - the pointer to a value by key, type: `void *`

### Create table
Create a table with a static size. Call the `tb_create_hash_table` function. Pass the size as the first parameter.
Returns a pointer on the table or `NULL`.
```c
tb_hash_table *table = tb_create_hash_table(size);
```
If the size is zero, returns `NULL`.
```c
tb_hash_table *table = tb_create_hash_table(0);
if (!table)
    ...
```

### Insert items
Create the variable, which you want to put to the table. 
Call the `tb_insert_item` function. 
Pass the table as the first parameter, the name of the key as the second, and the address on your variable as the third.
If the table is full, write a warning in the console and nothing to do.
```c
int a = 1;
tb_insert_item(table, "key1", &a);
char *string = "example";
tb_insert_item(table, "key2", &string);
```

### Change value
Call the `tb_insert_item` function with the name of the key, if you want to change the value by key.
```c
int *value = tb_get_value(table, "key1");
printf("%i", *value); // print 1
int b = 2;
tb_insert_item(table, "key1", &b);
int *new_value = tb_get_value(table, "key1");
printf("%i", *new_value); // print 2
```

### Get value by key
To get the value call the `tb_get_value` function. 
Pass the table as the first parameter and the name of the key as the second.
Returns a pointer on the value by key. If an item does not exists in the table, returns `NULL`;
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
To delete the value by key call the `tb_delete_item` funtion. 
Pass the table as the first parameter and the name of the key as the second.
Returns `1` if the item is deleted, otherwise returns `0`.
```c
int check = tb_delete_item(table, "key1");

type *value = tb_get_value(table, "key1");
printf("%p", value); // print (nil)
```

### Get items 
To get the item call the `tb_get_item` function. Pass the table as the first parameter, the key as the second.
Returns the pointer to an item or `NULL`, if an item does not exists. 
```c
tb_hash_table_item *item = tb_get_item(table, "key");
printf("%s", item->key);
```

### Find items
To find the item call the `tb_find_item` function. 
Pass the table as the first parameter, the key as the second.
Returns the pointer to an item or `NULL`, if item does not exists. 
```c
tb_hash_table_item *item = tb_find_item(table, "key");
```

### For loop
In the `for` loop, you must check the pointer to the item. 
```c
for (unsigned int i = 0; i < table->allocated; i++) {
    tb_hash_table_item *item = table.items[i];
    if (item != EMPTY_ITEM) {
      ...
    }
}
```
If the index is out of range `0` to `table->size`:
```c
for (unsigned int i = 0; i < table->allocated + 10; i++) {
    tb_insert_item(table, "key");  // warning in the console
}
```

```c
for (unsigned int i = 0; i < table->allocated + 10; i++) {
    void *p = tb_get_value(table, "key"); // returns NULL 
}
```

```c
for (unsigned int i = 0; i < table->allocated + 10; i++) {
    int delete = tb_delete_item(table, "key"); // returns 0 
}
```


### Delete table
To delete table call the `tb_delete_hash_table` function. Pass the table as the first parameter.
```c
tb_delete_hash_table(table);
```

## Python
### Table class
#### Members:
* `table.size` - table size
* `table.count` - number of elements in the table
* `table.empty` - the table is empty or not. Can be `True` or `False`.

#### Methods:
* `table.items()` - the iterator on elements.
* `table.get()` - function, returns a value by key.
* `table.insert()` - function, inserts a value by key. Returns position.
* `table.delete()` - function, removes a value by key. Returns `True` if the deletion is successful, otherwise `False`.
* `table.find()` - function, searches an item by key. Returns a tuple in the format `("key", value)` if the key is exists. If the key does is not exists, returns a tuple in the format `("", None)`.

### How to compile
This library can be compiled as a python module. For example, in Linux:
```bash
cd python
python setup.py built_ext --inplace
```
If you need a special path for the `Python.h` and `structmember.h` headers:
```bash
python setup.py build_ext --inplace --include-paths /usr/include/python3.6
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
table.insert("key1", 45)
```

#### Change value
Call the `insert` method from the `Table` class with the name of the key, if you want to change the value by key.
Pass the name of the key as the first parameter, the value as the second. 
```python
table.insert("key2", 45)
print(table.get("key2")) # 45
table.insert("key2", 64)
print(table.get("key2")) #64
```
#### Get value by key
To get the value call the `get` method fron the `Table` class with table and key. Returns the value.
Pass the name of the key as the first parameter. Returns a value by key. If an item does not exists in the table, returns `None`;

```python
a = table.get("key2")
print(a) 
```

#### Delete items
To delete a value by key call the `delete` method from the `Table` class. Returns `True` if the deletion is successful otherwise `False`. Pass the name of the key as the first parameter.
```python
deletion = table.delete("key1");
if deletion: 
    ...
a = table.get("key1")
print(a) # None
```

### Find items
To find the item call the `find` method from the `Table` class. 
Pass the key as the first parameter.
Returns the item or `("", None)`, if item does not exists. 
```python
key, value = table.find("key");
```

### For loop
In the `for` loop use the `items()` method from the `Table` class. Returns an iterator with tuples. 
Tuple has a key and value.
```python
for key, value in table.items(): 
    ...
```

#### Delete table
To delete the table call `del`. This is an optional action.
```python
del table
```

#### Tests
For testing check this [page](https://github.com/Chukak/hash-table/blob/master/python/readme.md).

### Macros (Only C/C++)
You can be use macros to get value from `void *`. For example:
```c
int a = GET_INT(tb_get_value(table, "key"));
printf("%i", a);
```
#### Macros
* `GET_INT` - returns an integer value from the pointer (`int`).
* `GET_FLOAT` - returns a float value from the pointer(`float`).
* `GET_DOUBLE` - returns a double value from the pointer(`double`).
* `GET_CHAR` - returns a character from pointer(`char`).
* `GET_STRING` - returns string(`char *`).
* `GET_CUSTOM_TYPE` - returns a custom type from the pointer (Using this macro can lead to undefined behavior).
A macros example:
```c
int a = GET_INT(tb_get_value(table, "key"));
```
A custom type example:
```c
Type t;
tb_insert_item(table, "key", &t);
Type t = GET_CUSTOM_TYPE(tb_get_value(table, "key"));
```



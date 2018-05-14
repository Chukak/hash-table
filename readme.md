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
make -f Library
```

## How to use
### Create table
Create a table with a static size. Call `tb_create_hash_table` function. Pass the size as first parameter.
```c
tb_hash_table *table = tb_create_hash_table(size);
```

### Insert items
Create the variable, that you want to pass to the table. 
Call `tb_insert_item` function with table, key and value. 
Pass the table as first parameter, the name of the key as second, and the address on your variable as third.
```c
int a = 1;
tb_insert_item(table, "key1", &a);
char *string = "example";
tb_insert_item(table, "key2", &string);
```

### Change value
Call `tb_insert_item` function with the name of the key, if you want to change the value by key.
```c
int *int_value = tb_get_value(table, "key1");
printf("%i", *int_value); // print 1
int b = 2;
tb_insert_item(table, "key1", &b);
int *int_other_value = tb_get_value(table, "key1");
printf("%i", *int_other_value); // print 2
```
### Get value by key
To get the value call `tb_get_value` function with table and key. Funciton returns apointer to a value.
Pass the table as first parameter and the name of the key as second.

```c
int *int_value = tb_get_value(table, "key1");
printf("%i", *int_value); // print 1
// function returns pointer on `char *`
char **string_value = tb_get_value(table, "key2");
printf("%i", *string_value); // print example
```

### Delete items
To delete value by key call `tb_delete_item` funtion. 
Pass the table as first parameter and the name of the key as second.
```c
tb_delete_item(table, "key1");
// segmentation fault error
int *int_value = tb_get_value(table, "key1");
```

### Delete table
To delete table call `tb_delete_hash_table` function. Pass the table as first parameter.
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
Create a table with a static size. Create the `Table` class. Pass the size as first parameter.
```python
table = Table(size);
```

#### Insert items
Call the `insert` method from the `Table` class. Pass the name of the key as first parameter, the value as second. 
```python
table.insert("key1", 45)
```

#### Change value
Call `get` method from the `Table` class with the name of the key, if you want to change the value by key.
```python
table.insert("key2", 45)
print(table.get("key2")) # 45
table.insert("key2", 64)
print(table.get("key2")) #64
```
#### Get value by key
To get the value call `get` method fron the `Table` class with table and key. Returns the value.
Pass the name of the key as first.

```python
a = table.get("key2")
print(a) 
print(table.get("key2")) #a
```

#### Delete items
To delete value by key call `delete` method from the `Table` class. Returns `True` if the deletion is successful otherwise `False`. Pass the name of the key as first.
```python
result = table.delete("key1");
print(result) #True
a = table.get("key1")
print(a) # None
```

#### Delete table
To delete table call `del`. 
```python
del table
```


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



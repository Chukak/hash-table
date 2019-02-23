# Hash table

## Installation 
### C/C++
Clone this repository and create `so` library:
```bash
git clone https://github.com/Chukak/hash-table.git
cmake ..
```

Remember to copy `hashtable.h` for use in your projects.

### Python 2/3
This library can be compiled as a python module. But you must have `Python.h` and `structmember.h` on your system. In linux (Ubuntu) you can install using this command `apt-get install python-dev python3-dev`.
After it, go to the `python` directory and run command:
```bash
python setup.py built_ext --inplace
```
If you need a special path for `Python.h` and `structmember.h`:
```bash
python setup.py build_ext --inplace --include-paths /usr/include/python3.5
```

After installation, you can import hashtable:
```python
from hashtable import Table
```

## How to use

Check [Documentation](https://github.com/Chukak/hash-table/wiki/Hash-table)

## Testing

### C/C++
Go to `tests` directory and run command:
```bash
cmake .
./tests_hashtable
```

Source code: [tests.c](https://github.com/Chukak/hash-table/blob/master/tests/tests.c)

### Python 2/3

Go to `python` directory. Build python module and call `run.py` to run all the tests.
```python
python run.py
```

Source code: [run.py](https://github.com/Chukak/hash-table/blob/master/python/run.py)

## Additional

Check [Additional](https://github.com/Chukak/hash-table/wiki/Additional)


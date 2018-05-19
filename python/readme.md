# Tests 

## Compile module
This library can be compiled as a python module. For example, in Linux:
```bash
python setup.py built_ext --inplace
```
If you need a special path:
```bash
python setup.py build_ext --inplace --include-paths /usr/include/python3.5
```

In python:
```python
from hashtable import Table
```

## Run tests
Call `run.py` to run all the tests.

```python
python run.py
```

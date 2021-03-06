import sys
import os
try:
    from setuptools import setup
    from setuptools import Extension
except ImportError:
    from distutils.core import setup
    from distutils.extension import Extension

if __name__ == "__main__":
    paths = []
    if "--include-paths" not in sys.argv:
        one, two = sys.version_info[0], sys.version_info[1]
        paths = [
            "/usr/include/python" + str(one) + "." + str(two),
            "/usr/local/include/python" + str(one) + "." + str(two),
            "/usr/lib/python" + str(one) + "." + str(two),
            "/usr/local/lib/python" + str(one) + "." + str(two),

        ]
    else:
        index = sys.argv.index("--include-paths")
        sys.argv.pop(index)
        paths = [sys.argv.pop(index)]

    if not os.path.exists(paths[0]):
        print("Error: " + paths[0] + " does not exists.")
    else:
        setup(
            name="hashtable",
            version="1.0",
            description="The hash table, writen in C.",
            url="https://github.com/Chukak/hash-table",
            ext_modules=[
                Extension(
                    "hashtable",
                    ["python.c", "../src/hashtable.c"],
                    include_dirs=paths,
                    extra_compile_args=[
                        "-g", "-std=c11", "-Werror", "-Wall", "-D_DEFAULT_SOURCE"
                    ])
            ],
            py_modules=["hashtable"],
        )

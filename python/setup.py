from distutils.core import setup, Extension

setup(
    name="hashtable",
    version="1.0",
    ext_modules=[
        Extension(
            "hashtable",
            ["python.c", "../hashtable.c"],
            extra_compile_args=[
                "-g", "-std=c11", "-Werror", "-Wall", "-D_DEFAULT_SOURCE"
            ])
    ],
    py_modules=["hashtable"],
)

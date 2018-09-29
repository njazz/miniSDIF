from distutils.core import setup, Extension
import os

os.environ["CC"] = "gcc"

module = Extension("minisdifpy", [\

"../src/mSDIFFile.cpp", \
"../src/mSDIFFrame.cpp", \
"../src/mSDIFMatrix.cpp", \
"../src/mSDIFTypes.cpp", \

"mini-sdif-py.c", \
])

# module.extra_compile_args = ['-std=gnu++11'];
# module.language

setup(
    ext_modules=[module],
    # include_dirs=["../src/"]
)

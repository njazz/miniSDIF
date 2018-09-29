#!/bin/sh

echo "***"
python3 gen_bind.py > mini-sdif-py.c
pwd

# python build_lib.py build_ext --inplace

rm -r build
mkdir build
cd build
cmake ..
make

cd ..
cp build/minisdifpy.dylib ./minisdifpy.so
rm -r build

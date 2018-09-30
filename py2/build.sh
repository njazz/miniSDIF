#!/bin/sh

echo "***"
pwd

# python build_lib.py build_ext --inplace

# rm -r build
mkdir build
cd build
cmake ..
make

cd ..
cp build/minisdifpy.dylib ./minisdifpy`python3-config --extension-suffix`
rm -r build

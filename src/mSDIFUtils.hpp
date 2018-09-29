//
//  mSDIFUtils.h

/* Copyright 2018 Alex Nadzharov

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//  Created on 28/01/2018.

// http://sdif.sourceforge.net/standard/sdif-standard.html

#ifndef mSDIFUtils_h
#define mSDIFUtils_h

#include "stdint.h"

#include <vector>

#include <iostream>
#include <fstream>

#include <array>

enum mFileError {
    meOK = 0,
    meNoObject,
    meFileError,
    meSDIFHeaderError,
    meBadFrameHeaderSize,
    meBadMatrixDataSize

};

#define MAX_FRAME_DATA_SIZE 1103200256 // 1 GB

template <class T>
inline void swapEndianness(T& x)
{
    std::array<char, sizeof(T)> p;
    memcpy(&p[0], &x, sizeof(T));
    std::reverse(p.begin(), p.end());
    memcpy(&x, &p[0], sizeof(T));
}

//

enum MMatrixDataTypesMSB {
    mMFloat = 0,
    mMSignedInteger = 1,
    mMUnsignedInteger = 2,
    mMText = 3,
    mMBlob = 4
};

enum MMatrixDataTypesV3 {

    mTText = 0x0301,
    mTChar = 0x0301,
    mTFloat4 = 0x0004,
    mTFloat8 = 0x0008,
    mTInt1 = 0x0101,
    mTInt2 = 0x0102,
    mTInt4 = 0x0104,
    mTInt8 = 0x0108,
    mTUInt1 = 0x0201,
    mTUInt2 = 0x0202,
    mTUInt4 = 0x0204,
    mTUInt8 = 0x0208
};

enum MMatrixDataTypesV2 {
    mTFloat4a = 0x0001, // = 1
    mTFloat4b = 0x0020, // = 32
    mTFloat8a = 0x0002, // = 2
    mTFloat8b = 0x0040, // = 64
};

#endif /* mSDIFUtils_h */

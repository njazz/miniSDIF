

//
//  mSDIFStructures.h
//  
//
//  Created by Alex on 27/01/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

// http://sdif.sourceforge.net/standard/sdif-standard.html

#ifndef mSDIFStructures_h
#define mSDIFStructures_h

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

#endif /* mSDIFStructures_h */

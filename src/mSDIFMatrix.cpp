
//
//  mSDIFMatrix.cpp
//
//
//  Created by Alex on 28/01/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#include "mSDIFMatrix.hpp"

void MSDIFMatrixHeader::operator=(const MSDIFMatrixHeader& h)
{
    for (int i = 0; i < 4; i++)
        signature[i] = h.signature[i];

    dataType = h.dataType;
    rows = h.rows;
    columns = h.columns;
};

mFileError MSDIFMatrixHeader::fromFile(std::ifstream& file)
{
    MSDIFMatrixHeader* newHeader = new MSDIFMatrixHeader;

    size_t hdr_size = 16;
    file.read((char*)newHeader, hdr_size);

    swapEndianness(newHeader->dataType);
    swapEndianness(newHeader->rows);
    swapEndianness(newHeader->columns);

    for (int i = 0; i < 4; i++)
        signature[i] = newHeader->signature[i];
    dataType = newHeader->dataType;
    rows = newHeader->rows;
    columns = newHeader->columns;

    delete newHeader;

    return meOK;
}

mFileError MSDIFMatrixHeader::toFile(std::ofstream& file)
{
    MSDIFMatrixHeader* newHeader = new MSDIFMatrixHeader;

    *newHeader = *this;

    swapEndianness(newHeader->dataType);
    swapEndianness(newHeader->rows);
    swapEndianness(newHeader->columns);

    size_t hdr_size = 16;
    file.write((char*)newHeader, hdr_size);

    return meOK;
}

int MSDIFMatrixHeader::byteSize()
{
    // V3
    int ret = dataType % 256;

    // V2
    if (dataType == mTFloat4a)
        ret = 4;
    if (dataType == mTFloat4b)
        ret = 4;
    if (dataType == mTFloat8a)
        ret = 8;
    if (dataType == mTFloat8b)
        ret = 8;

    return ret;
}

void MSDIFMatrixHeader::setSignature(std::string s)
{
    if (s.length() < 4)
        return;

    for (int i = 0; i < 4; i++)
        signature[i] = s[i];
}

//

mFileError MSDIFMatrix::fromFile(std::ifstream& file)
{
    header.fromFile(file);

    //        if (byteSize == 0)
    //            return meBadMatrixDataSize;

    //size_t matrix_data_size = header.rows * header.columns * header.byteSize();

    data = new char[matrixDataSize()];
    file.read((char*)data, matrixDataSize());

    if (header.dataType == mTFloat4)
        for (int i = 0; i < header.rows * header.columns; i++) {
            swapEndianness(values<float>()[i]);
        }

    int padding_size = (matrixDataSize() % 8) ? (8 - (matrixDataSize() % 8)) : 0;
    if ((matrixDataSize() % 8) && (header.byteSize() == 4))
        padding_size = 4;

    char* padding = new char[padding_size];
    file.read((char*)padding, padding_size);

    return meOK;
}

inline int MSDIFMatrix::paddingSize()
{
    int padding_size = (matrixDataSize() % 8) ? (8 - (matrixDataSize() % 8)) : 0;
    if ((matrixDataSize() % 8) && (header.byteSize() == 4))
        padding_size = 4;

    return padding_size;
}

mFileError MSDIFMatrix::toFile(std::ofstream& file)
{
    printf("write matrix\n");

    printf("write: %s", info().c_str());

    header.toFile(file);

    //        if (byteSize == 0)
    //            return meBadMatrixDataSize;

    printf("write: %s", info().c_str());

    file.write((char*)data, matrixDataSize());

    printf("write matrix size %i\n", matrixDataSize());

    char* padding = new char[paddingSize()];
    file.write((char*)padding, paddingSize());

    return meOK;
}

std::string MSDIFMatrix::info()
{
    std::string ret;

    char s[5];
    for (int i = 0; i < 4; i++)
        s[i] = header.signature[i];
    s[4] = '\0';

    ret += "* Matrix type: " + std::string(s);
    ret += "*** Matrix Byte size: " + std::to_string(header.byteSize());
    ret += " Rows: " + std::to_string(header.rows);
    ret += " Columns: " + std::to_string(header.columns);
    ret += "\n";

    return ret;
}

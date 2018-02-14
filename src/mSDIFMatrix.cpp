//
//  mSDIFMatrix.cpp

/* Copyright 2018 Alex Nadzharov
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//  Created on 28/01/2018.

#include "mSDIFMatrix.hpp"

#include "mSDIFTypes.hpp"

#include <stdio.h>

void MSDIFMatrixHeader::operator&=(const MSDIFMatrixHeader& h)
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

MSDIFMatrix::MSDIFMatrix()
{
}

MSDIFMatrix::MSDIFMatrix(std::string signature, uint32_t rows, uint32_t columns, uint32_t type)
{
    _header.setSignature(signature);
    _header.columns = columns;
    _header.rows = rows;
    _header.dataType = type;

    _data = 0;

    MSDIFType* t = MSDIFType::fromSignature(signature);
    if (t) {
        _header.dataType = t->dataType();
        _header.columns = (uint32_t)t->columnNames().size();
    }

    newSize(_header.rows, _header.columns);

    _hasIndexColumn = MSDIFType::hasIndexColumn(signature);
    _gainColumnIndex = MSDIFType::columnIndex(signature, "Amplitude");
}

MSDIFMatrix::MSDIFMatrix(const MSDIFMatrix& m)
{
    _header = m._header;
    _header.setSignature(m._header.signature);

    MSDIFType* t = MSDIFType::fromSignature(m._header.signature);
    if (t) {
        _header.dataType = t->dataType();
        _header.columns = (uint32_t)t->columnNames().size();
    }
    _header.rows = m._header.rows;

    newSize(_header.rows, _header.columns);

    _data = new char[matrixDataSize()];
    memcpy(_data, m._data, matrixDataSize());

    _hasIndexColumn = m._hasIndexColumn;//MSDIFType::hasIndexColumn(header.signature);
    _gainColumnIndex = m._gainColumnIndex;
}

MSDIFMatrix::~MSDIFMatrix()
{
    if (_data)
        free(_data);
}

void MSDIFMatrix::newSize(uint32_t rows, uint32_t columns)
{
    if (_data)
        free(_data);

    _header.rows = rows;
    _header.columns = columns;
    _data = malloc(matrixDataSize());
}

void MSDIFMatrix::resizeRows(uint32_t rows)
{
    float* oldData = (float*)_data;
    size_t oldDataSize = matrixDataSize();

    _header.rows = rows;
    _data = malloc(matrixDataSize());

    if (!oldData)
        return;

    if (matrixDataSize() < oldDataSize)
        oldDataSize = matrixDataSize();
    memcpy(_data, oldData, oldDataSize);

    delete oldData;
}

void MSDIFMatrix::_resizeRowsColumns(uint32_t rows, uint32_t columns)
{
    float* oldData = (float*)_data;
    int n_rows = _header.rows;
    int n_cols = _header.columns;

    int old_cols = _header.columns;

    _header.rows = rows;
    _header.columns = columns;
    _data = malloc(matrixDataSize());

    if (rows < n_rows)
        n_rows = rows;
    if (columns < n_cols)
        n_cols = columns;

    for (int x = 0; x < n_cols; x++)
        for (int y = 0; y < n_rows; y++) {
            int old_idx = x + y * old_cols;
            int new_idx = x + y * _header.columns;

            ((float*)_data)[new_idx] = oldData[old_idx];
        }
}

void MSDIFMatrix::resize(uint32_t rows, uint32_t columns)
{
    if (columns == _header.columns)
        resizeRows(rows);
    else
        _resizeRowsColumns(rows, columns);
}

mFileError MSDIFMatrix::fromFile(std::ifstream& file)
{
    _header.fromFile(file);

    //        if (byteSize == 0)
    //            return meBadMatrixDataSize;

    //size_t matrix_data_size = header.rows * header.columns * header.byteSize();

    _data = new char[matrixDataSize()];
    file.read((char*)_data, matrixDataSize());

    if (_header.dataType == mTFloat4)
        for (int i = 0; i < _header.rows * _header.columns; i++) {
            swapEndianness(((float*)_data)[i]);
        }

    int padding_size = (matrixDataSize() % 8) ? (8 - (matrixDataSize() % 8)) : 0;
    if ((matrixDataSize() % 8) && (_header.byteSize() == 4))
        padding_size = 4;

    char* padding = new char[padding_size];
    file.read((char*)padding, padding_size);
    delete[] padding;

    return meOK;
}

uint32_t MSDIFMatrix::matrixDataSize()
{
    return _header.rows * _header.columns * _header.byteSize();
}

inline int MSDIFMatrix::paddingSize()
{
    int padding_size = (matrixDataSize() % 8) ? (8 - (matrixDataSize() % 8)) : 0;
    if ((matrixDataSize() % 8) && (_header.byteSize() == 4))
        padding_size = 4;

    return padding_size;
}

mFileError MSDIFMatrix::toFile(std::ofstream& file)
{
    printf("write matrix\n");
    printf("write: %s", info().c_str());

    _header.toFile(file);

    printf("write: %s", info().c_str());

    char* b_data = new char[matrixDataSize()];

    char* vv = values<char*>();

    if (_header.dataType != mTChar)
        for (int i = 0; i < _header.rows * _header.columns; i++) {
            ((float*)b_data)[i] = ((float*)vv)[i];

            // todo: ???
            swapEndianness(((float*)b_data)[i]);
        }
    else
        for (int i = 0; i < _header.rows * _header.columns; i++) {
            ((char*)b_data)[i] = ((char*)vv)[i];

            // todo: ???
            //swapEndianness(((float*)b_data)[i]);
        }

    file.write((char*)b_data, matrixDataSize());

    printf("write matrix size %i\n", matrixDataSize());

    char* padding = new char[paddingSize()];
    file.write((char*)padding, paddingSize());

    delete[] b_data;
    delete[] padding;

    return meOK;
}

std::string MSDIFMatrix::info()
{
    std::string ret;

    char s[5];
    for (int i = 0; i < 4; i++)
        s[i] = _header.signature[i];
    s[4] = '\0';

    ret += "--[Matrix] type: " + std::string(s);
    ret += " Cell size: " + std::to_string(_header.byteSize());
    ret += " Rows: " + std::to_string(_header.rows);
    ret += " Columns: " + std::to_string(_header.columns);
    ret += "\n";

    return ret;
}

void MSDIFMatrix::applyGain(float g)
{
    if (!_gainColumnIndex) return;

    for (int i=0;i<_header.rows;i++)
    {
        float gain = valuesAtRow<float>(i)[_gainColumnIndex];
        setCellValue(_gainColumnIndex,i,gain*g);

    }
}

size_t MSDIFMatrix::maximumIndexValue()
{
    if (!_hasIndexColumn) return 0;
    
    size_t ret = 0;
    for (int i=0;i<_header.rows;i++)
    {
        size_t v = valuesAtRow<float>(i)[0];
        if (v>ret) ret = v;
    }
    return ret;
}

void MSDIFMatrix::shiftIndices(size_t idx)
{
    if (!_hasIndexColumn) return ;
    
    for (int i=0;i<_header.rows;i++)
    {
        //rowAt<float>(i)[0] += idx;
        
        // todo: check if index is 1st column
        ((float*)_data)[_header.columns*i] += idx;
    }
    
}

// ==========

template <>
void MSDIFMatrix::setValues<std::string>(std::string nv)
{
    if (_data)
        free(_data);

    _data = malloc(nv.length());

    setValues<char*>(const_cast<char*>(nv.c_str()));
}

template <>
std::string MSDIFMatrix::values<std::string>()
{
    if (!_data)
        return "";

    char ret[matrixDataSize()];
    for (int i = 0; i < matrixDataSize(); i++)
        ret[i] = ((char*)_data)[i];
    return std::string((char*)ret);
}

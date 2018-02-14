//
//  mSDIFMatrix.hpp

/* Copyright 2018 Alex Nadzharov
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//  Created on 28/01/2018.

#ifndef mSDIFMatrix_hpp
#define mSDIFMatrix_hpp

#include "mSDIFUtils.hpp"

#include <stdio.h>

#include <string>

#include <fstream>
#include <iostream>

///> @brief base POD structure for SDIF Matrix file i/o
struct MSDIFMatrixHeaderStruct {
    char signature[4];
    uint32_t dataType;
    uint32_t rows = 0;
    uint32_t columns = 0;
};

///> @brief SDIF Matrix header
struct MSDIFMatrixHeader : public MSDIFMatrixHeaderStruct {

    void operator&=(const MSDIFMatrixHeader& h);

    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    int byteSize();

    void setSignature(std::string s);
};

///> @brief class that represents SDIF matrix
class MSDIFMatrix {
    friend class MSDIFFrame;

    void* _data = 0;

    //
    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    MSDIFMatrixHeader _header;

    MSDIFMatrix();

    ///> @brief resize data storage and keep data
    ///> @details this is called when number of columns needs to be changed
    void _resizeRowsColumns(uint32_t rows, uint32_t columns);

    // for edits
    bool _hasIndexColumn = false;
    int _gainColumnIndex = -1;

public:
    ///> @brief if the matrix type is not found, creates with supplied parameters otherwise uses only 'rows' value
    MSDIFMatrix(std::string signature, uint32_t rows = 1, uint32_t columns = 1, uint32_t type = mTChar);
    MSDIFMatrix(const MSDIFMatrix& m);
    ~MSDIFMatrix();

    int rows() { return _header.rows; }
    int columns() { return _header.columns; }

    void newSize(uint32_t rows, uint32_t columns);

    void resize(uint32_t rows, uint32_t columns);
    void resizeRows(uint32_t rows);

    void addRow();
    void addColumn();

    void deleteRow(size_t idx);
    void deleteColumn(size_t idx);

    uint32_t matrixDataSize();
    int paddingSize();

    char* signature() { return _header.signature; }

    std::string info();

    // ===== editing

    void applyGain(float g);

    // ===== for specific data types:

    size_t maximumIndexValue();
    void shiftIndices(size_t idx);

    // ======= templates =======

    template <typename T>
    const T* data()
    {
        return _data;
    }

    template <typename T>
    T* valuesAtRow(size_t idx)
    {
        if (idx >= (_header.rows))
            return 0;

        if (_header.columns < 1)
            return 0;

        T* d = new T[_header.columns];

        // todo replace later
        for (int i = 0; i < _header.columns; i++) {
            d[i] = static_cast<T*>(_data)[idx * _header.columns + i];
        }

        return d;
    }

    template <typename T>
    T* valuesAtColumn(size_t idx)
    {
        if (idx >= (_header.columns))
            return 0;

        if (_header.rows < 1)
            return 0;

        T* d = new T[_header.rows];

        // todo: replace later
        for (int i = 0; i < _header.rows; i++) {
            d[i] = static_cast<T*>(_data)[i * _header.columns + idx];
        }

        return d;
    }

    template <typename T>
    void setColumnValues(size_t idx, T* d)
    {
        if (idx >= (_header.columns))
            return;

        if (_header.rows < 1)
            return;

        // todo: replace later
        for (int i = 0; i < _header.rows; i++) {
            ((T*)_data)[i * _header.columns + idx] = d[i];
        }
    }

    template <typename T>
    void setRowValues(size_t idx, T* d)
    {
        if (idx >= (_header.rows))
            return;

        if (_header.rows < 1)
            return;

        // todo: replace later
        for (int i = 0; i < _header.columns; i++) {
            ((T*)_data)[i + idx * _header.columns] = d[i];
        }
    }

    // ==========

    template <typename T>
    bool is()
    {
        return sizeof(T) == _header.byteSize();
    }

    template <typename T>
    T values()
    {
        if (!_data)
            return 0;

        void* ret = malloc(matrixDataSize());
        memcpy(ret, _data, matrixDataSize());
        return static_cast<T>(ret);
    }

    template <typename T>
    void setValues(T nv)
    {
        if (!_data)
            return;

        for (int i = 0; i < matrixDataSize(); i++)
            ((char*)_data)[i] = ((char*)nv)[i];
    }

    template <typename T>
    void setCellValue(size_t x, size_t y, T d)
    {
        if (y >= (_header.rows))
            return;

        if (x >= (_header.columns))
            return;

        if (_header.rows < 1)
            return;

        // todo: replace later
        ((T*)_data)[x + y * _header.columns] = d;
    }
};

template <>
std::string MSDIFMatrix::values<std::string>();

template <>
void MSDIFMatrix::setValues<std::string>(std::string nv);

#endif /* mSDIFMatrix_hpp */

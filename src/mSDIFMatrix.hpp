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
//#include "mMatrixValues.hpp"

#include <stdio.h>

#include <string>

#include <fstream>
#include <iostream>

struct MSDIFMatrixHeaderStruct {
    char signature[4]; // matrix type
    uint32_t dataType;
    uint32_t rows = 0;
    uint32_t columns = 0;
};

struct MSDIFMatrixHeader : public MSDIFMatrixHeaderStruct {

    void operator=(const MSDIFMatrixHeader& h);

    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    int byteSize();

    void setSignature(std::string s);
};

//
class MSDIFMatrix {
    friend class MSDIFFrame;

    void* data = 0;

    //
    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    MSDIFMatrixHeader header;

    MSDIFMatrix();

public:
    ///> @brief if the matrix type is not found, creates with supplied parameters
    MSDIFMatrix(std::string signature, uint32_t rows, uint32_t columns = 1, uint32_t type = mTChar);
    ~MSDIFMatrix();

    int rows() { return header.rows; }
    int columns() { return header.columns; }

    void newSize(uint32_t rows, uint32_t columns);
    void resize(uint32_t rows, uint32_t columns);

    void addRow();
    void addColumn();

    void deleteRow(size_t idx);
    void deleteColumn(size_t idx);

    uint32_t matrixDataSize();
    int paddingSize();

    char* signature() { return header.signature; }

    std::string info();

    // ======= templates =======

    template <typename T>
    T* rowAt(size_t idx)
    {
        if (idx >= (header.rows))
            return 0;

        if (header.columns < 1)
            return 0;

        T* d = new T[header.rows];

        // todo replace later
        for (int i = 0; i < header.columns; i++) {
            d[i] = static_cast<T*>(data)[idx * header.columns + i];
        }

        return d;
    }

    template <typename T>
    T* columnAt(size_t idx)
    {
        if (idx >= (header.columns))
            return 0;

        if (header.rows < 1)
            return 0;

        T* d = new T[header.rows];

        // todo replace later
        for (int i = 0; i < header.rows; i++) {
            d[i] = static_cast<T*>(data)[i * header.columns + idx];
        }

        return d;
    }

    template <typename T>
    void setColumn(size_t idx, T* d)
    {
        if (idx >= (header.columns))
            return;

        if (header.rows < 1)
            return;

        // todo replace later
        for (int i = 0; i < header.rows; i++) {
            ((T*)data)[i * header.columns + idx] = d[i];
        }
    }

    template <typename T>
    bool is()
    {
        return sizeof(T) == header.byteSize();
    }

    template <typename T>
    T values()
    {
        if (!data)
            return 0;

        //if (header.byteSize() != sizeof(T)) return 0;

        void* ret = malloc(matrixDataSize());
        memcpy(ret, data, matrixDataSize());
        return static_cast<T>(ret);
    }

    template <typename T>
    void setValues(T nv)
    {
        if (!data)
            return;

        //if (header.byteSize() != sizeof(T)) return;

        // int s = matrixDataSize();

        for (int i = 0; i < matrixDataSize(); i++)
            ((char*)data)[i] = ((char*)nv)[i];
    }
};

template <>
std::string MSDIFMatrix::values<std::string>();

template<>
void MSDIFMatrix::setValues<std::string>(std::string nv);

#endif /* mSDIFMatrix_hpp */

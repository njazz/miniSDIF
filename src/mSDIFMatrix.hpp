//
//  mSDIFMatrix.hpp
//  
//
//  Created by Alex on 28/01/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#ifndef mSDIFMatrix_hpp
#define mSDIFMatrix_hpp

#include "mSDIFStructures.hpp"
//#include "mMatrixValues.hpp"

#include <stdio.h>

#include <string>

#include <iostream>
#include <fstream>

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
    void* data = 0;

public:
    MSDIFMatrixHeader header;

    //

    MSDIFMatrix()
    {
    }

    MSDIFMatrix(std::string signature, uint32_t rows, uint32_t columns, uint32_t type)
    {
        header.setSignature(signature);
        header.columns = columns;
        header.rows = rows;
        header.dataType = type;

        data = malloc(matrixDataSize());
    }

    ~MSDIFMatrix()
    {
        // delete data;
    }
    //

    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

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
    T* values()
    {
        if (!data)
            return 0;

        return static_cast<T*>(data);
    }

    template <typename T>
    void setValues(T nv)
    {
        // int s = matrixDataSize();

        for (int i = 0; i < matrixDataSize(); i++)
            ((char*)data)[i] = nv[i];
    }

    inline uint32_t matrixDataSize() { return header.rows * header.columns * header.byteSize(); }
    int paddingSize();

    std::string info();
};

#endif /* mSDIFMatrix_hpp */

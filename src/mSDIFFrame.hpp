//
//  mSDIFFrame.hpp
//
//  Created by Alex on 28/01/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#ifndef mSDIFFrame_hpp
#define mSDIFFrame_hpp

#include "mSDIFStructures.hpp"
#include "mSDIFMatrix.hpp"

#include <stdio.h>

#include <string>

#include <iostream>
#include <fstream>

struct MSDIFFrameHeaderStruct {
    char signature[4]; // frame type
    uint32_t frameSize = 16; //
    double time = -1; // todo: -inf
    int32_t streamID = -1;
    uint32_t matrixCount = 0; //
};

struct MSDIFFrameHeader : public MSDIFFrameHeaderStruct {
    void operator=(const MSDIFFrameHeader& h);

    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    void setSignature(std::string s);
};

typedef std::vector<MSDIFMatrix*> MSDIFMatrixVector;

class MSDIFFrame {
    friend class MSDIFFile;

    MSDIFFrameHeader header;
    MSDIFMatrixVector _matrices;

    MSDIFFrame();

    //
    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);
public:

    MSDIFFrame(std::string signature, int32_t streamID);

    //
    char* signature() { return header.signature; }
    uint32_t frameSize() { return header.frameSize; }

    double time() { return header.time; }
    void setTime(double t) { header.time = t; }

    int32_t streamID() { return header.streamID; }
    void setStreamID(int32_t s) { header.streamID = s; }

    uint32_t matrixCount() { return header.matrixCount; }

    //

    MSDIFMatrixVector& matrices() { return _matrices; }; // todo
    MSDIFMatrixVector matricesWithSignature(std::string signature);

    void addMatrix(MSDIFMatrix* m);
    void removeMatrixAt(size_t idx);
    void insertMatrix(size_t idx, MSDIFMatrix* fr);
    void removeAllMatrices();

    std::string info();
};

#endif /* mSDIFFrame_hpp */

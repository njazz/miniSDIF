//
//  mSDIFFrame.hpp
//  
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
    uint32_t frameSize; // todo
    double time;
    int32_t streamID;
    uint32_t matrixCount = 0; // todo
};

struct MSDIFFrameHeader : public MSDIFFrameHeaderStruct {
    void operator=(const MSDIFFrameHeader& h);

    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    void setSignature(std::string s);
};

typedef std::vector<MSDIFMatrix*> MSDIFMatrixVector;

class MSDIFFrame {
    MSDIFFrameHeader header;
    MSDIFMatrixVector _matrices;

public:
    MSDIFFrame(){};

    MSDIFFrame(std::string signature, int32_t streamID)
    {
        header.setSignature(signature);
        header.streamID = streamID;
    }
    //
    char* signature() { return header.signature; }

    uint32_t frameSize() { return header.frameSize; }

    double time() { return header.time; }
    void setTime(double t) { header.time = t; }

    int32_t streamID() { return header.streamID; }
    void setStreamID(int32_t s) { header.streamID = s; }

    uint32_t matrixCount() { return header.matrixCount; }

    //

    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    MSDIFMatrixVector& matrices() { return _matrices; }; // todo
    MSDIFMatrixVector matricesWithSignature(std::string signature);

    void addMatrix(MSDIFMatrix* m)
    {
        _matrices.push_back(m);
        header.matrixCount++;
        header.frameSize = 16 + m->matrixDataSize();
    }
    void removeAllMatrices()
    {
        _matrices.clear();
        header.matrixCount = 0;
    }

    std::string info();
};

#endif /* mSDIFFrame_hpp */

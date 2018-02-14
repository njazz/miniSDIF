//
//  mSDIFFrame.hpp

/* Copyright 2018 Alex Nadzharov
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//  Created on 28/01/2018.

#ifndef mSDIFFrame_hpp
#define mSDIFFrame_hpp

#include "mSDIFMatrix.hpp"
#include "mSDIFUtils.hpp"

#include <stdio.h>

#include <string>

#include <fstream>
#include <iostream>

///> \brief MSDIFFrameHeaderStruct : base POD structure for SDIF Frame i/o
struct MSDIFFrameHeaderStruct {
    char signature[4]; // frame type
    uint32_t frameSize = 16; //
    double time = -1; // todo: -inf
    int32_t streamID = -1;
    uint32_t matrixCount = 0; //
};

struct MSDIFFrameHeader : public MSDIFFrameHeaderStruct {
    void operator&=(const MSDIFFrameHeader& h);

    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    void setSignature(std::string s);
};

typedef std::vector<MSDIFMatrix*> MSDIFMatrixVector;

///> \brief MSDIFFrame : class that represents SDIF frame
class MSDIFFrame {
    friend class MSDIFFile;

    MSDIFFrameHeader header;
    MSDIFMatrixVector _matrices;

    MSDIFFrame();

    //
    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    void calculateFrameSize();

    //
    float _timeOffset = 0;
    float _timeScale = 1;
    float _gain = 1;

public:
    MSDIFFrame(std::string signature, int32_t streamID);
    MSDIFFrame(MSDIFFrame& f);

    //
    char* signature() { return header.signature; }
    uint32_t frameSize() { return header.frameSize; }

    double time() { return header.time * _timeScale + _timeOffset; }
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

    // non-destructive editing
    // time
    void setTimeOffset(float t_o) { _timeOffset = t_o; }
    void setTimeScale(float t_s) { _timeScale = t_s; }
    float timeOffset() { return _timeOffset; }
    float timeScale() { return _timeScale; }
    void applyTime()
    {
        setTime(time());
        _timeOffset = 0;
        _timeScale = 1;
    }
    
    // gain
    void setGain(float g){_gain = g;}
    float gain() {return _gain;}
    void applyGain()
    {
          for (auto m : _matrices)
              m->applyGain(_gain);
          _gain = 1;
    }
    
};

#endif /* mSDIFFrame_hpp */

//
//  mSDIFFile.h

/* Copyright 2018 Alex Nadzharov

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//  Created on 28/01/2018.

#ifndef mSDIFFile_h
#define mSDIFFile_h

#include "mSDIFUtils.hpp"

#include <string>

#include <fstream>
#include <iostream>

#include "mSDIFFrame.hpp"
#include "mSDIFTypes.hpp"

//

///> \brief MSDIFFileHeaderStruct : base POD structure for SDIF File i/o
struct MSDIFFileHeaderStruct {
    char signature[4]; // "SDIF"
    uint32_t headerFrameSize;
    uint32_t specificationVersion;
    uint32_t padding;
};

struct MSDIFFileHeader : public MSDIFFileHeaderStruct {
    MSDIFFileHeader& operator=(const MSDIFFileHeader& h);

    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    void setSignature(std::string s);
};

typedef std::vector<MSDIFFrame> MSDIFFrameVector;

//

///> \brief MSDIFFile : class that represents SDIF file
class MSDIFFile {
    MSDIFFileHeader header;

    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    MSDIFFrameVector _frames;

    //
    float _timeOffset = 0;
    float _timeScale = 1;

    //
    void _mergeFramesWithSameTime();

public:
    MSDIFFile();

    mFileError readFile(std::string fileName);
    mFileError writeFile(std::string fileName);

    MSDIFFrameVector frames() { return _frames; };

    MSDIFFrameVector framesWithSignature(std::string signature);
    MSDIFFrameVector framesWithNotSignature(std::string signature);
    MSDIFFrameVector framesWithTimeRange(double start, double end);
    MSDIFFrameVector framesWithStreamID(uint32_t streamID);

    uint32_t frameCount() { return (uint32_t)_frames.size(); }

    void addFrame(MSDIFFrame &fr);
//    void removeFrame(MSDIFFrame* fr);
    void removeFrameAt(size_t idx);
    void insertFrame(size_t idx, MSDIFFrame& fr);

    void removeAllFrames();
//    void removeFramesWithSignature(std::string signature);

    void replaceFrames(MSDIFFrameVector& fv) { _frames = fv; }

    std::string info();

    // editing
    void mergeFramesWithSignature(std::string signature, MSDIFFile* file);
    void sortFramesByTime();

    //    void reverse();
    void applyTime();
    //    void applyGain();

    // non-destructive editing:
    // time
    void setTimeOffset(float t_o);
    void setTimeScale(float t_s);

    float timeOffset() { return _timeOffset; }
    float timeScale() { return _timeScale; }

    // gain
    //    void setGain(float g);
    //    void setFadeIn(float f);
    //    void setFadeOut(float f);
    //    float gain();
    //    float fadeIn();
    //    float fadeOut();

    // ==========
    template <typename T>
    void createFrameWithMatix(std::string signature, int streamID, float time, T data, int rows)
    {
        MSDIFMatrix* m = new MSDIFMatrix(signature, rows);
        m->setValues<T>(data);
        MSDIFFrame f = new MSDIFFrame(signature, streamID);
        f.setTime(time);
        f.addMatrix(m);
        addFrame(f);
    }

    template <typename T>
    void insertFrameWithMatix(size_t idx, std::string signature, int streamID, float time, T data, int rows)
    {
        //int rows =  sizeof(data);
        MSDIFMatrix* m = new MSDIFMatrix(signature, rows);
        m->setValues<T>(data);
        MSDIFFrame f(signature, streamID);
        f.setTime(time);
        f.addMatrix(m);
        insertFrame(idx, f);
    }
};

//

#endif /* mSDIFFile_h */

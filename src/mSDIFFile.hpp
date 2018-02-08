//
//  mSDIFFile.h
//  
//  Created by Alex on 28/01/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#ifndef mSDIFFile_h
#define mSDIFFile_h

#include "mSDIFStructures.hpp"

#include <string>

#include <iostream>
#include <fstream>

#include "mSDIFFrame.hpp"
#include "mSDIFTypes.hpp"

//

struct MSDIFFileHeaderStruct {
    char signature[4];                  // "SDIF"
    uint32_t headerFrameSize;
    uint32_t specificationVersion;
    uint32_t padding;
};

struct MSDIFFileHeader : public MSDIFFileHeaderStruct {
    void operator=(const MSDIFFileHeader& h);

    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    void setSignature(std::string s);
};

//
typedef std::vector<MSDIFFrame*> MSDIFFrameVector;

class MSDIFFile {
    MSDIFFileHeader header;

    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

     MSDIFFrameVector _frames;
public:
    MSDIFFile();

    mFileError readFile(std::string fileName);
    mFileError writeFile(std::string fileName);

    MSDIFFrameVector frames() {return _frames;};

    MSDIFFrameVector framesWithSignature(std::string signature);
    MSDIFFrameVector framesWithTimeRange(double start, double end);
    MSDIFFrameVector framesWithStreamID(uint32_t streamID);

    uint32_t frameCount(){return (uint32_t)_frames.size();}
    
    void addFrame(MSDIFFrame* fr);
    void removeFrameAt(size_t idx);
    void insertFrame(size_t idx,MSDIFFrame* fr);
    void removeAllFrames();

    std::string info();

    // ==========
    template <typename T>
    void createFrameWithMatix(std::string signature,int streamID, float time, T data, int rows)
    {
        MSDIFMatrix* m = new MSDIFMatrix(signature, rows);
        m->setValues<T>(data);
        MSDIFFrame* f = new MSDIFFrame(signature, streamID);
        f->setTime(time);
        f->addMatrix(m);
        addFrame(f);
    }

    template <typename T>
    void insertFrameWithMatix(size_t idx, std::string signature,int streamID, float time, T data, int rows)
    {
        //int rows =  sizeof(data);
        MSDIFMatrix* m = new MSDIFMatrix(signature, rows);
        m->setValues<T>(data);
        MSDIFFrame* f = new MSDIFFrame(signature, streamID);
        f->setTime(time);
        f->addMatrix(m);
        insertFrame(idx,f);
    }
};

//

#endif /* mSDIFFile_h */

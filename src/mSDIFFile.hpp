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

//

struct MSDIFFileHeaderStruct {
    char signature[4]; // SDIFÏ
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

public:
    MSDIFFile();

    MSDIFFrameVector frames;

    mFileError fromFile(std::ifstream& file);
    mFileError toFile(std::ofstream& file);

    mFileError readFile(std::string fileName);
    mFileError writeFile(std::string fileName);

    MSDIFFrameVector framesWithSignature(std::string signature);
    MSDIFFrameVector framesWithTimeRange(double start, double end);
    MSDIFFrameVector framesWithStreamID(uint32_t streamID);

    void addFrame(MSDIFFrame* fr);
    void removeFrameAt(size_t idx);
    void insertFrame(size_t idx,MSDIFFrame* fr);
    void removeAllFrames();

    std::string info();
};

//

#endif /* mSDIFFile_h */







//
//  mSDIFFile.cpp
//
//
//  Created by Alex on 28/01/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#include <stdio.h>

#include "mSDIFFile.hpp"

void MSDIFFileHeader::operator=(const MSDIFFileHeader& h)
{
    for (int i = 0; i < 4; i++)
        signature[i] = h.signature[i];

    headerFrameSize = h.headerFrameSize;
    specificationVersion = h.specificationVersion;
    padding = h.padding;
};

mFileError MSDIFFileHeader::fromFile(std::ifstream& file)
{
    MSDIFFileHeader* newHeader = new MSDIFFileHeader;

    //
    size_t hdr_size = 16;
    file.read((char*)newHeader, hdr_size);
    if (strncmp(newHeader->signature, "SDIF", 4))
        return meSDIFHeaderError;

    swapEndianness(newHeader->headerFrameSize);
    swapEndianness(newHeader->specificationVersion);

    for (int i = 0; i < 4; i++)
        signature[i] = newHeader->signature[i];

    headerFrameSize = newHeader->headerFrameSize;
    specificationVersion = newHeader->specificationVersion;
    padding = newHeader->padding;

    delete newHeader;

    return meOK;
};

mFileError MSDIFFileHeader::toFile(std::ofstream& file)
{
    MSDIFFileHeader* newHeader = new MSDIFFileHeader;

    newHeader = this;

    swapEndianness(newHeader->headerFrameSize);
    swapEndianness(newHeader->specificationVersion);

    size_t hdr_size = 16;
    file.write((char*)newHeader, hdr_size);
    return meOK;
}

void MSDIFFileHeader::setSignature(std::string s)
{
    if (s.length() < 4)
        return;
    for (int i = 0; i < 4; i++)
        signature[i] = s[i];
}

//

MSDIFFile::MSDIFFile()
{
    header.setSignature("SDIF");
    header.headerFrameSize = 8;
    header.specificationVersion = 3;
    header.padding = 0;
};

mFileError MSDIFFile::fromFile(std::ifstream& file)
{
    mFileError err = header.fromFile(file);

    if (err != meOK)
        return err;

    int c = 0;
    while (!(file.eof())) {
        printf("reading frame %i\n", c++);

        MSDIFFrame* newFrame = new MSDIFFrame;

        mFileError err = newFrame->fromFile(file);
        if (!(err == meOK)) {
            delete newFrame;
            return err;
        }

        if (newFrame->frameSize() >= 12)
            _frames.push_back(newFrame);
        else {
            delete newFrame;
            return meOK;
        }

        //fuse
        if (c > 1024)
            return meOK;

        //delete newFrame;
    }

    return meOK;
};

mFileError MSDIFFile::toFile(std::ofstream& file)
{
    printf("write file\n");

    mFileError err = header.toFile(file);

    if (err != meOK)
        return err;

    for (MSDIFFrameVector::iterator it = _frames.begin(); it != _frames.end(); ++it) {

        mFileError err = (*it)->toFile(file);
        if (!(err == meOK)) {
            return err;
        }
    }

    return meOK;
}

//

mFileError MSDIFFile::readFile(std::string fileName)
{

    std::ifstream file;
    file.open(fileName);

    //
    if (!file.is_open())
        return meFileError;

    mFileError err = fromFile(file);

    file.close();

    return err;

    //    MSDIFFileHeader* header = new MSDIFFileHeader;
    //    MSDIFFrame* frameHeader;
}

mFileError MSDIFFile::writeFile(std::string fileName)
{
    std::ofstream file;
    file.open(fileName);

    //
    if (!file.is_open())
        return meFileError;

    mFileError err = toFile(file);

    file.close();

    return err;
}

std::string MSDIFFile::info()
{
    std::string ret = "[SDIF File] ";
    ret += " Version: " + std::to_string(header.specificationVersion) + " Frame count: " + std::to_string(_frames.size()) + "\n";

    for (MSDIFFrameVector::iterator it = _frames.begin(); it != _frames.end(); ++it) {
        if (*it)
            ret += (*it)->info();
    }

    return ret;
}

MSDIFFrameVector MSDIFFile::framesWithTimeRange(double start, double end)
{
    MSDIFFrameVector ret;

    for (MSDIFFrameVector::iterator it = _frames.begin(); it != _frames.end(); ++it) {
        if (((*it)->time() >= start) && ((*it)->time() < end))
            ret.push_back(*it);
    }

    return ret;
}

MSDIFFrameVector MSDIFFile::framesWithStreamID(uint32_t streamID)
{
    MSDIFFrameVector ret;

    for (MSDIFFrameVector::iterator it = _frames.begin(); it != _frames.end(); ++it) {
        if ((*it)->streamID() == streamID)
            ret.push_back(*it);
    }

    return ret;
}

MSDIFFrameVector MSDIFFile::framesWithSignature(std::string signature)
{
    MSDIFFrameVector ret;

    for (MSDIFFrameVector::iterator it = _frames.begin(); it != _frames.end(); ++it) {
        if (!strncmp((*it)->signature(), signature.c_str(), 4))
            ret.push_back(*it);
    }

    return ret;
}

//

void MSDIFFile::addFrame(MSDIFFrame* fr)
{
    _frames.push_back(fr);
}

void MSDIFFile::removeFrameAt(size_t idx)
{
    if (idx >= _frames.size())
        return;

    _frames.erase(_frames.begin() + idx);
}
void MSDIFFile::insertFrame(size_t idx, MSDIFFrame* fr)
{
    if (idx >= _frames.size())
        return;

    _frames.insert(_frames.begin() + idx, fr);
}

void MSDIFFile::removeAllFrames()
{
    _frames.clear();
}

}


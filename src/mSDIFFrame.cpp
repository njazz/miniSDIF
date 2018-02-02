//
//  mSDIFFrame.cpp
//  
//
//  Created by Alex on 28/01/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#include "mSDIFFrame.hpp"

void MSDIFFrameHeader::operator=(const MSDIFFrameHeader& h)
{
    for (int i = 0; i < 4; i++)
        signature[i] = h.signature[i];

    frameSize = h.frameSize;
    time = h.time;
    streamID = h.streamID;
    matrixCount = h.matrixCount;
};

mFileError MSDIFFrameHeader::fromFile(std::ifstream& file)
{
    MSDIFFrameHeader* newHeader = new MSDIFFrameHeader;

    //
    size_t hdr_size = 24;
    file.read((char*)newHeader, hdr_size);

    swapEndianness(newHeader->frameSize);
    swapEndianness(newHeader->streamID);
    swapEndianness(newHeader->matrixCount);
    swapEndianness(newHeader->time);

    for (int i = 0; i < 4; i++)
        signature[i] = newHeader->signature[i];
    frameSize = newHeader->frameSize;
    time = newHeader->time;
    streamID = newHeader->streamID;
    matrixCount = newHeader->matrixCount;

    delete newHeader;

    return meOK;
}

mFileError MSDIFFrameHeader::toFile(std::ofstream& file)
{
    MSDIFFrameHeader* newHeader = new MSDIFFrameHeader;

    *newHeader = *this;

    swapEndianness(newHeader->frameSize);
    swapEndianness(newHeader->streamID);
    swapEndianness(newHeader->matrixCount);
    swapEndianness(newHeader->time);

    //
    size_t hdr_size = 24;
    file.write((char*)newHeader, hdr_size);

    return meOK;
}

void MSDIFFrameHeader::setSignature(std::string s)
{
    if (s.length()<4) return;
    for (int i=0;i<4;i++)
        signature[i] = s[i];
}
//

mFileError MSDIFFrame::fromFile(std::ifstream& file)
{
    mFileError err = header.fromFile(file);
    if (err != meOK)
        return err;

    //size_t frame_header_size = header.frameSize;

    if (header.frameSize < 12)
        return meOK;

    //fuse
    if (header.matrixCount>1024)
        header.matrixCount = 1024;

    for (int i = 0; i < header.matrixCount; i++) {

        //printf("read matrix %i\n", i);

        MSDIFMatrix* newMatrix = new MSDIFMatrix;

        mFileError err = newMatrix->fromFile(file);
        if (err != meOK) {
            delete newMatrix;
            return err;
        }

        _matrices.push_back(newMatrix);

        printf("<%i>: %s", i, newMatrix->info().c_str());

        //delete newMatrix;
    }

    return meOK;
}

mFileError MSDIFFrame::toFile(std::ofstream& file)
{
    printf("write frame\n");

    mFileError err = header.toFile(file);
    if (err != meOK)
        return err;

    for (MSDIFMatrixVector::iterator it = _matrices.begin(); it != _matrices.end(); ++it) {
        mFileError err = (*it)->toFile(file);
        if (err != meOK) {
            return err;
        }
    }

    return meOK;
}

std::string MSDIFFrame::info()
{
    std::string ret;

    char s[5];
    for (int i = 0; i < 4; i++)
        s[i] = header.signature[i];
    s[4] = '\0';

    ret += "* Frame type: " + std::string(s);
    ret += " Stream ID: " + std::to_string(header.streamID);
    ret += " Matrix count: " + std::to_string(header.matrixCount);
    ret += " Time: " + std::to_string(header.time);
    ret += "\n";

    for (MSDIFMatrixVector::iterator it = _matrices.begin(); it != _matrices.end(); ++it) {
        ret += (*it)->info();
    }

    return ret;
}

MSDIFMatrixVector MSDIFFrame::matricesWithSignature(std::string signature)
{
    MSDIFMatrixVector ret;

    for (MSDIFMatrixVector::iterator it = _matrices.begin(); it != _matrices.end(); ++it) {
        if (strncmp((*it)->header.signature, signature.c_str(), 4))
            ret.push_back(*it);
    }

    return ret;
}



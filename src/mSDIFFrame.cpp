//
//  mSDIFFrame.cpp

/* Copyright 2018 Alex Nadzharov
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//  Created on 28/01/2018.

#include "mSDIFFrame.hpp"

void MSDIFFrameHeader::operator&=(const MSDIFFrameHeader& h)
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

MSDIFFrame::MSDIFFrame(){};

MSDIFFrame::MSDIFFrame(std::string signature, int32_t streamID)
{
    header.setSignature(signature);
    header.streamID = streamID;
}

MSDIFFrame::MSDIFFrame(MSDIFFrame& f) : header(f.header)
{
    //header = f.header;
    header.matrixCount=0;
    
    for (auto m : f.matrices())
    {
        addMatrix(new MSDIFMatrix(*m));
    }
}

mFileError MSDIFFrame::fromFile(std::ifstream& file)
{
    mFileError err = header.fromFile(file);
    if (err != meOK)
        return err;

    if (header.frameSize < 12)
        return meOK;

    // todo: replace?
    if (header.matrixCount>1024)
        header.matrixCount = 1024;

    for (int i = 0; i < header.matrixCount; i++) {

        MSDIFMatrix* newMatrix = new MSDIFMatrix;

        mFileError err = newMatrix->fromFile(file);
        if (err != meOK) {
            delete newMatrix;
            return err;
        }

        _matrices.push_back(newMatrix);

        printf("<%i>: %s", i, newMatrix->info().c_str());
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

    ret += "-[Frame] Type: " + std::string(s);
    ret += " Stream ID: " + std::to_string(header.streamID);
    ret += " Matrices: " + std::to_string(header.matrixCount);
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
        if (!strncmp((*it)->_header.signature, signature.c_str(), 4))
            ret.push_back(*it);
    }

    return ret;
}

void MSDIFFrame::calculateFrameSize()
{
    header.frameSize = 16;
    for (auto m : _matrices)
    {
        header.frameSize+= m->matrixDataSize();
    }
}

//
void MSDIFFrame::addMatrix(MSDIFMatrix* m)
{
    _matrices.push_back(m);
    header.matrixCount++;
    calculateFrameSize();
}

void MSDIFFrame::removeMatrixAt(size_t idx)
{
    if (idx>=_matrices.size())
        return;
    
    _matrices.erase(_matrices.begin()+idx);
    header.matrixCount--;
    calculateFrameSize();
}

void MSDIFFrame::insertMatrix(size_t idx, MSDIFMatrix* fr)
{
    if (idx>=_matrices.size())
        return;
    
    _matrices.insert(_matrices.begin()+idx, fr);
    header.matrixCount++;
    calculateFrameSize();
}

void MSDIFFrame::removeAllMatrices()
{
    _matrices.clear();
    header.matrixCount = 0;
    header.frameSize = 16;
}



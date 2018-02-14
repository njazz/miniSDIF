//
//  mSDIFFile.cpp

/* Copyright 2018 Alex Nadzharov
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//  Created on 28/01/2018.

#include <stdio.h>

#include "mSDIFFile.hpp"

MSDIFFileHeader& MSDIFFileHeader::operator=(const MSDIFFileHeader& h)
{
    for (int i = 0; i < 4; i++)
        signature[i] = h.signature[i];

    headerFrameSize = h.headerFrameSize;
    specificationVersion = h.specificationVersion;
    padding = h.padding;

    return *this;
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
    MSDIFFileHeader* newHeader = this;//new MSDIFFileHeader;

    //newHeader = this;

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
    header.padding = 1;
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

MSDIFFrameVector* MSDIFFile::framesWithTimeRange(double start, double end)
{
    MSDIFFrameVector* ret = new MSDIFFrameVector;

    for (MSDIFFrameVector::iterator it = _frames.begin(); it != _frames.end(); ++it) {
        if (((*it)->time() >= start) && ((*it)->time() < end))
            ret->push_back(*it);
    }

    return ret;
}

MSDIFFrameVector* MSDIFFile::framesWithStreamID(uint32_t streamID)
{
    MSDIFFrameVector* ret = new MSDIFFrameVector;

    for (MSDIFFrameVector::iterator it = _frames.begin(); it != _frames.end(); ++it) {
        if ((*it)->streamID() == streamID)
            ret->push_back(*it);
    }

    return ret;
}

MSDIFFrameVector* MSDIFFile::framesWithSignature(std::string signature)
{
    MSDIFFrameVector* ret = new MSDIFFrameVector;

    for (MSDIFFrameVector::iterator it = _frames.begin(); it != _frames.end(); ++it) {
        if (!strncmp((*it)->signature(), signature.c_str(), 4))
            ret->push_back(*it);
    }

    return ret;
}

MSDIFFrameVector* MSDIFFile::framesWithNotSignature(std::string signature)
{
    MSDIFFrameVector* ret = new MSDIFFrameVector;

    for (MSDIFFrameVector::iterator it = _frames.begin(); it != _frames.end(); ++it) {
        if (strncmp((*it)->signature(), signature.c_str(), 4))
            ret->push_back(*it);
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

void MSDIFFile::removeFrame(MSDIFFrame* fr)
{
    MSDIFFrameVector::iterator it = std::find(_frames.begin(), _frames.end(), fr);
    if (it != _frames.end())
        _frames.erase(it);
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

void MSDIFFile::removeFramesWithSignature(std::string signature)
{
    for (auto fr : _frames) {
        if (!strncmp(fr->signature(), signature.c_str(), 4))
            removeFrame(fr);
    }
}

void MSDIFFile::applyTime()
{
    for (auto f : _frames) {
        if (f->time())
            f->applyTime();
    }
}

inline size_t _maximumIndexValue(MSDIFFrameVector vec)
{
    size_t ret = 0;
    for (auto v : vec) {
        for (auto m : v->matrices()) {
            if (ret < m->maximumIndexValue())
                ret = m->maximumIndexValue();
        }
    }

    return ret;
}

//inline void _shiftIndices(MSDIFFrameVector* vec, size_t idx)
//{
//    for (auto v : *vec) {
//        for (auto m : v->matrices()) {
//            m->shiftIndices(idx);
//        }
//    }
//}

inline void _shiftIndices(MSDIFFrame* f, size_t idx)
{
    for (auto m : f->matrices()) {
        m->shiftIndices(idx);
    }
}

MSDIFFrame* _mergeFramesProc(MSDIFFrame* f1, MSDIFFrame* f2, size_t* i1, size_t* i2, size_t shift)
{
    if (!f2) {
        if (!f1)
            return 0;
        else
            return f1;
    }
    if (!f1) {
        if (!f2)
            return 0;
        else
            return f2;
    }

    if (f1->time() < f2->time()) {
        (*i1)--;
        return f1;
    } else {
        (*i2)--;

        MSDIFFrame* nf = new MSDIFFrame(*f2);
        _shiftIndices(nf, shift);
        //
        return nf;
    }
}

void MSDIFFile::mergeFramesWithSignature(std::string signature, MSDIFFile* file)
{
    MSDIFFrameVector* frames1 = framesWithSignature(signature);
    MSDIFFrameVector* frames2 = file->framesWithSignature(signature);

    MSDIFFrameVector nf;

    size_t shift = _maximumIndexValue(frames());

    size_t f_c1 = frames1->size();
    size_t f_c2 = frames2->size();

    while (f_c2 && f_c1) {

        MSDIFFrame* fr = _mergeFramesProc(frames2->at(frameCount() - f_c1), frames2->at(frames2->size() - f_c2), &f_c1, &f_c2, shift);

        if (fr)
            nf.push_back(fr);
        else
            f_c2--;
    }

    while (f_c2) {
        nf.push_back(frames2->at(frames2->size() - f_c2));
        f_c2--;
    }
    while (f_c1) {
        nf.push_back(frames()[frameCount() - f_c1]);
        f_c1--;
    }

    removeFramesWithSignature("1TRC");
    for (MSDIFFrame* ff : nf)
        addFrame(ff);

    delete frames1;
    delete frames2;
}

void MSDIFFile::setTimeOffset(float t_o)
{
    _timeOffset = t_o;
    for (auto f : _frames) {
        if (f->time())
            f->setTimeOffset(t_o);
    }
}
void MSDIFFile::setTimeScale(float t_s)
{
    _timeScale = t_s;
    for (auto f : _frames) {
        if (f->time())
            f->setTimeScale(t_s);
    }
}

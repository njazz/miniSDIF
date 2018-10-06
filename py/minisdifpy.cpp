//
//  minisdifpy.cpp
//
//
//  Created by Alex on 30/09/2018.
//

#include <stdio.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <pybind11/numpy.h>

namespace py = pybind11;

#include <vector>
#include <string>

#include "math.h"

#include "../src/mSDIF.hpp"

PYBIND11_MODULE(minisdifpy, m)
{
  py::enum_<mFileError>(m, "SDIFFileError")
    .value("OK", mFileError::meOK)
    .value("meNoObject", mFileError::meNoObject)
    .value("meFileError", mFileError::meFileError)
    .value("meSDIFHeaderError", mFileError::meSDIFHeaderError)
    .value("meBadFrameHeaderSize", mFileError::meBadFrameHeaderSize)
    .value("meBadMatrixDataSize", mFileError::meBadMatrixDataSize)
    .export_values();

    py::class_<MSDIFFile>(m, "SDIFFile")
        .def(py::init())

        .def("readFile", &MSDIFFile::readFile)
        .def("writeFile", &MSDIFFile::writeFile)

        .def("frames", &MSDIFFile::frames)

        .def("framesWithSignature", &MSDIFFile::framesWithSignature)
        .def("framesWithNotSignature", &MSDIFFile::framesWithNotSignature)
        .def("framesWithTimeRange", &MSDIFFile::framesWithTimeRange)
        .def("framesWithStreamID", &MSDIFFile::framesWithStreamID)

        .def("frameCount", &MSDIFFile::frameCount)

        .def("addFrame", &MSDIFFile::addFrame)
        .def("removeFrameAt", &MSDIFFile::removeFrameAt)
        .def("insertFrame", &MSDIFFile::insertFrame)

        .def("removeAllFrames", &MSDIFFile::removeAllFrames)

        .def("replaceFrames", &MSDIFFile::replaceFrames)

        .def("info", &MSDIFFile::info)

        .def("mergeFramesWithSignature", &MSDIFFile::mergeFramesWithSignature)
        .def("sortFramesByTime", &MSDIFFile::sortFramesByTime)

        .def("applyTime", &MSDIFFile::applyTime)

        .def("setTimeOffset", &MSDIFFile::setTimeOffset)
        .def("setTimeScale", &MSDIFFile::setTimeScale)

        .def("timeOffset", &MSDIFFile::timeOffset)
        .def("timeScale", &MSDIFFile::timeScale);



    py::class_<MSDIFFrame>(m, "SDIFFrame")
        .def(py::init())
        .def(py::init<std::string, int32_t>())

        .def("signature", &MSDIFFrame::signature)
        .def("frameSize", &MSDIFFrame::frameSize)

        .def("time",[](const MSDIFFrame& f){ return f.time();})
        .def("setTime", &MSDIFFrame::setTime)

        .def("streamID", &MSDIFFrame::streamID)
        .def("setStreamID", &MSDIFFrame::setStreamID)

        .def("matrixCount", &MSDIFFrame::matrixCount)

        .def("matrices", [](const MSDIFFrame& f){ return f.matrices();})
        .def("matrixAt", [](const MSDIFFrame& f, size_t i){ return f.matrices()[i];})

        .def("matricesWithSignature", &MSDIFFrame::matricesWithSignature)

        .def("addMatrix", &MSDIFFrame::addMatrix)
        .def("removeMatrixAt", &MSDIFFrame::removeMatrixAt)
        .def("insertMatrix", &MSDIFFrame::insertMatrix)
        .def("removeAllMatrices", &MSDIFFrame::removeAllMatrices)

        .def("setTimeOffset", &MSDIFFrame::setTimeOffset)
        .def("setTimeScale", &MSDIFFrame::setTimeScale)
        .def("timeOffset", &MSDIFFrame::timeOffset)
        .def("timeScale", &MSDIFFrame::timeScale)
        .def("applyTime", &MSDIFFrame::applyTime)

        .def("setGain", &MSDIFFrame::setGain)
        .def("gain", &MSDIFFrame::gain)
        .def("applyGain", &MSDIFFrame::applyGain)

        .def("mergeWithFrame", &MSDIFFrame::mergeWithFrame);

    py::class_<MSDIFMatrix>(m, "SDIFMatrix")
        .def(py::init())
        .def(py::init<std::string, uint32_t, uint32_t, uint32_t>())

        .def("rows", &MSDIFMatrix::rows)
        .def("columns", &MSDIFMatrix::columns)

        .def("newSize", &MSDIFMatrix::newSize)

        .def("resize", &MSDIFMatrix::resize)
        .def("resizeRows", &MSDIFMatrix::resizeRows)

        .def("matrixDataSize", &MSDIFMatrix::matrixDataSize)
        .def("paddingSize", &MSDIFMatrix::paddingSize)

        .def("signature", &MSDIFMatrix::signature)

        .def("info", &MSDIFMatrix::info)

        .def("applyGain", &MSDIFMatrix::applyGain)

        .def("maximumIndexValue", &MSDIFMatrix::maximumIndexValue)
        .def("shiftIndices", &MSDIFMatrix::shiftIndices)

        .def("data", &MSDIFMatrix::dataVec<float>)
        .def("valuesAtRow", &MSDIFMatrix::valuesAtRowVec<float>)
        .def("valuesAtColumn", &MSDIFMatrix::valuesAtColumnVec<float>)

        .def("setRowValues", &MSDIFMatrix::setRowValuesVec<float>)
        .def("setColumnValues", &MSDIFMatrix::setColumnValuesVec<float>);


    py::class_<MSDIFType>(m, "SDIFType")
        .def(py::init(&MSDIFType::fromSignature))
        .def("fromSignature", &MSDIFType::fromSignature)
        .def("signature", &MSDIFType::signature)
        .def("dataType", &MSDIFType::dataType)
        .def("columnNames", &MSDIFType::columnNames)
        .def("description", &MSDIFType::description)

        .def("byteSize", &MSDIFType::byteSize)

        .def("hasIndexColumn", &MSDIFType::hasIndexColumn)
        .def("columnIndex", &MSDIFType::columnIndex);
}

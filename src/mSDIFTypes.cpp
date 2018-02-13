//  mSDIFTypes.cpp
//  miniSDIF

/* Copyright 2018 Alex Nadzharov
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//  Created on 03/02/2018.

// /////
// This file is optional. It aims to provide templates when creating new SDIF matrices/frames.
// /////

#include "mSDIFTypes.hpp"

#include "json.hpp"
using json = nlohmann::json;

const json MTypes = {
    //
    // supporting types
    { "1NVT", { { "description", "NameValueTable" },
                  { "columns", { "NVTText" } },
                  { "type", mTChar } } },

    { "1TYP", { { "description", "TypeDefinitions" },
                  { "columns", { "TYPText" } },
                  { "type", mTChar } } },

    { "1IDS", { { "description", "StreamInfo" },
                  { "columns", { "IDSText" } },
                  { "type", mTChar } } },
    //
    //
    {
        "1GAI", { { "description", "LinearGain" },
                    { "columns", { "Gain" } },
                    { "type", mTFloat4 } }
        // 1-row
    },

    { "1WIN", { { "description", "Window" },
                  { "columns", { "Samples" } },
                  { "type", mTFloat4 } } },
    {
        "IWIN", { { "description", "WindowInfo" },
                    { "columns", { "WindowIdentifier", "WindowSize" } },
                    { "type", mTFloat4 } }
        // 1-row
    },
    { "1CHA", { { "description", "Channels" },
                { "columns", { "Channel1", "Channel2" } },
                { "type", mTFloat4 } },
    //
    //
    { "1FQ0", {} },
    //
    // Sinusoidal Modelling
    { "1PIC", {} }, { "1TRC", { { "description", "SinusoidalTracks" }, { "columns", { "Index", "Frequency", "Amplitude", "Phase" } }, { "type", mTFloat4 }

                              } },
    { "1HRM", {} }, { "1HRE", {} },
    //
    //
    { "1ENV", {} }, { "1TFC", {} }, { "1CEC", {} }, { "1ARA", {} }, { "1ARK", {} },
    //
    { "1FOF", {} }, { "1RES", {} }, { "1DIS", {} }, { "1NOI", {} }, { "1FOB", {} }, { "1REB", {} },
    //
    { "ISTF", {} }, { "1STF", {} },
    //
    { "INRG", {} }, { "1NRG", {} }, { "1BND", {} },
    //
    { "1TDS", {} },
    //
    { "1PEM", {} }, { "ITMR", {} }, { "ITMI", {} },
    //
    { "1VUN", {} }, { "1VUF", {} }, { "1MRK", {} }, { "1VUV", {} },
    //
    { "EMPM", {} }, { "EMJR", {} },
    { "EFPM", {} }
};

MSDIFType::MSDIFType(){};

MSDIFType* MSDIFType::fromSignature(std::string signature)
{
    if (MTypes.find(signature) == MTypes.end())
        return 0;

    json t1 = MTypes[signature];
    if (t1.is_null())
        return 0;

    json t2 = t1["description"];
    if (t2.is_null())
        return 0;

    MSDIFType* ret = new MSDIFType();

    ret->_description = t1["description"];
    auto n = t1["columns"];

    for (auto s : n) {
        ret->_columnNames.push_back(s);
    }
    ret->_dataType = t1["type"];

    ret->_signature = signature;

    return ret;
}

int MSDIFType::byteSize()
{
    // V3
    int ret = _dataType % 256;

    // V2
    if (_dataType == mTFloat4a)
        ret = 4;
    if (_dataType == mTFloat4b)
        ret = 4;
    if (_dataType == mTFloat8a)
        ret = 8;
    if (_dataType == mTFloat8b)
        ret = 8;

    return ret;
}

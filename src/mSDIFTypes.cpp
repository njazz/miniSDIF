
//
//  mSDIFTypes.cpp
//  miniSDIF
//
//  Created by Alex on 03/02/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

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
    { "1GAI", {} },
    { "1WIN", {} },
    { "IWIN", {} },
    { "1CHA", {} },
    //
    { "1FQ0", {} },
    //
    // Sinusoidal Modelling
    { "1PIC", {} },
    { "1TRC", { { "description", "SinusoidalTracks" },
                  { "columns", { "Index", "Frequency", "Amplitude", "Phase" } },
                  { "type", mTFloat4 }

              } },
    { "1HRM", {} },
    { "1HRE", {} },
    //
    //
    { "1ENV", {} },
    { "1TFC", {} },
    { "1CEC", {} },
    { "1ARA", {} },
    { "1ARK", {} },
    //
    { "1FOF", {} },
    { "1RES", {} },
    { "1DIS", {} },
    { "1NOI", {} },
    { "1FOB", {} },
    { "1REB", {} },
    //
    { "ISTF", {} },
    { "1STF", {} },
    //
    { "INRG", {} },
    { "1NRG", {} },
    { "1BND", {} },
    //
    { "1TDS", {} },
    //
    { "1PEM", {} },
    { "ITMR", {} },
    { "ITMI", {} },
    //
    { "1VUN", {} },
    { "1VUF", {} },
    { "1MRK", {} },
    { "1VUV", {} },
    //
    { "EMPM", {} },
    { "EMJR", {} },
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

    // std::cout << t1 <<"\n";

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

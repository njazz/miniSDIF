//
//  mSDIFTypes.hpp
//  miniSDIF
//
//  Created by Alex on 03/02/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#ifndef mSDIFTypes_hpp
#define mSDIFTypes_hpp

#include <stdio.h>

#include <string>
#include <vector>

#include "mSDIFStructures.hpp"

class MSDIFType
{
    std::string _signature;
    uint32_t _dataType;
    std::vector<std::string> _columnNames;
    std::string _description;
    
    MSDIFType();
public:
    static MSDIFType* fromSignature(std::string signature);
    
    inline std::string signature(){return _signature;};
    inline uint32_t dataType(){return _dataType;};
    inline std::vector<std::string> columnNames(){return _columnNames;};
    inline std::string description(){return _description;};

    int byteSize();
};
#endif /* mSDIFTypes_hpp */

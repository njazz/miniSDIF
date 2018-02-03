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
    std::string signature;
    uint32_t dataType;
    
    std::vector<std::string> columnNames;
    
    std::string description;
    
    MSDIFType();
public:
    static MSDIFType* fromSignature(std::string signature);
};
#endif /* mSDIFTypes_hpp */

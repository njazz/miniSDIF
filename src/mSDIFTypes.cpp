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
    // supporting
    "1NVT", { "description", "NameValueTable",
                "columns", { "NVTText" },
                "type", "Char" },
    "1TYP", { "description", "TypeDefinitions",
                "columns", { "TYPText" },
                "type", "Char" },
    "1IDS", {},
    //
    "1GAI", {}, "1WIN", {}, "IWIN", {}, "1CHA", {},
    //
    "1FQ0", {},
    // Sinusoidal Modelling
    "1PIC", {}, "1TRC", { "description", "SinusoidalTracks",
                            "columns", { "Index", "Frequency", "Amplitude", "Phase" },
                            "type", "Float4"

                        },
    "1HRM", {}, "1HRE", {},
    //
    "1ENV", {}, "1TFC", {}, "1CEC", {}, "1ARA", {}, "1ARK", {},
    //
    "1FOF", {}, "1RES", {}, "1DIS", {}, "1NOI", {}, "1FOB", {}, "1REB", {},
    //
    "ISTF", {}, "1STF", {},
    //
    "INRG", {}, "1NRG", {}, "1BND", {},
    //
    "1TDS", {},
    //
    "1PEM", {}, "ITMR", {}, "ITMI", {},
    //
    "1VUN", {}, "1VUF", {}, "1MRK", {}, "1VUV", {},
    //
    "EMPM", {}, "EMJR", {}, "EFPM", {},
};

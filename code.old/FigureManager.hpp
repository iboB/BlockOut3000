// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

// management of figures. lifetime... generation,,,

class FigureTemplate;
class FigureSet;

struct FigureManager
{
public:
    static void initialize();
    static void cleanup();

    static bool readFigures(const std::string& input);

    // return nullptr if no such set exists
    static const FigureSet* getFigureSet(const std::string& name);
};
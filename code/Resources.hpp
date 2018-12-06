// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <string>

class Flat3DPVM;
class CubeTemplate;

struct Resources
{
    static void initialize();
    static void cleanup();

    static Flat3DPVM& shaderFlat3D();
    static const CubeTemplate& cubeTemplate();
    static const std::string& figuresData();
};

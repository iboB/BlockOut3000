// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Resources.hpp"

#include "Flat3DPVM.hpp"
#include "CubeTemplate.hpp"

#include <cassert>

namespace
{

struct ResourcesImpl
{
    Flat3DPVM shaderFlat3D;
    CubeTemplate cube;
};

ResourcesImpl* instance;

}

void Resources::initialize()
{
    assert(!instance);
    instance = new ResourcesImpl;
}

void Resources::cleanup()
{
    delete instance;
    instance = nullptr;
}

Flat3DPVM& Resources::shaderFlat3D()
{
    return instance->shaderFlat3D;
}

const CubeTemplate& Resources::cubeTemplate()
{
    return instance->cube;
}

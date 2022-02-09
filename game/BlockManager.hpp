// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "BlockDefinitionPtr.hpp"

#include <unordered_map>
#include <string>
#include <vector>

class BlockManager
{
public:
    BlockManager();
    ~BlockManager();

    struct BlockSet;
    using BlockSetPtr = std::shared_ptr<BlockSet>;
    struct BlockSet
    {
        std::string name;
        std::vector<BlockSetPtr> includedSets;
        std::vector<BlockDefinitionPtr> blockDefinitions;
    };

private:
    // registry of all known blocks
    std::unordered_map<std::string, BlockDefinitionPtr> m_blocks;
    std::unordered_map<std::string, BlockSetPtr> m_sets;
};

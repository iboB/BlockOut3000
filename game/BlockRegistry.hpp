// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "BlockTemplatePtr.hpp"

#include <string>
#include <vector>

class BlockRegistry
{
public:
    BlockRegistry();
    ~BlockRegistry();

    struct BlockSet;
    using BlockSetPtr = std::shared_ptr<BlockSet>;
    struct BlockSet
    {
        std::string name;
        std::vector<BlockSetPtr> includedSets;
        std::vector<BlockTemplatePtr> blockDefinitions;
    };

private:
    std::vector<BlockTemplatePtr> m_blocks; // all known blocks
    std::vector<BlockSetPtr> m_sets; // all sets
};

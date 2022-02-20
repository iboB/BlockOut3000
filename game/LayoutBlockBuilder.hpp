// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "lib/GUILayout.hpp"

class LayoutBlockBuilder final : public GUILayout
{
public:
    virtual const char* name() const override { return "block builder"; }

    const NamedElement& curBlock() const { return m_curBlock; }
    const NamedElement& curBlockLayers() const { return m_curBlockLayers; }
    const NamedElement& curBlockPreview() const { return m_curBlockPreview; }

private:
    virtual void updateElements() override;
    void createElements();

    NamedElement m_blockSets;
    NamedElement m_curBlockSet;
    NamedElement m_curBlock;
    NamedElement m_curBlockLayers;
    NamedElement m_curBlockPreview;
    NamedElement m_undoRedo;
};

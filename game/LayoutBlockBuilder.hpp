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

    const NamedElement& sets() const { return m_sets; }
    const NamedElement& curSet() const { return m_curSet; }
    const NamedElement& blockData() const { return m_blockData; }
    const NamedElement& blockLayers() const { return m_blockLayers; }
    const NamedElement& blockPreview() const { return m_blockPreview; }
    const NamedElement& undoRedo() const { return m_undoRedo; }

private:
    virtual void updateElements() override;
    void createElements();

    NamedElement m_sets;
    NamedElement m_curSet;
    NamedElement m_blockData;
    NamedElement m_blockLayers;
    NamedElement m_blockPreview;
    NamedElement m_undoRedo;
};

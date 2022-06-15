// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "LayoutBlockBuilder.hpp"

void LayoutBlockBuilder::updateElements()
{
    if (m_namedElements.empty()) createElements();
}

void LayoutBlockBuilder::createElements()
{
    set_contain(m_root, LAY_ROW);

    auto leftA = item();
    set_size_xy(leftA, 200, 0);
    set_behave(leftA, LAY_VFILL);
    set_contain(leftA, LAY_COLUMN);
    insert(m_root, leftA);

    {
        m_sets.name = "Block Sets";
        auto id = item();
        set_size_xy(id, 0, 300);
        set_behave(id, LAY_HFILL);
        insert(leftA, id);
        m_sets.id = id;
        registerNamedElement(m_sets);
    }

    {
        m_curSet.name = "Set";
        auto id = item();
        set_behave(id, LAY_FILL);
        insert(leftA, id);
        m_curSet.id = id;
        registerNamedElement(m_curSet);
    }

    auto leftB = item();
    set_size_xy(leftB, 200, 0);
    set_behave(leftB, LAY_VFILL);
    set_contain(leftB, LAY_COLUMN);
    insert(m_root, leftB);

    {
        m_blockData.name = "Block";
        auto id = item();
        set_size_xy(id, 0, 300);
        set_behave(id, LAY_HFILL);
        insert(leftB, id);
        m_blockData.id = id;
        registerNamedElement(m_blockData);
    }

    {
        m_blockLayers.name = "Layers";
        auto id = item();
        set_behave(id, LAY_FILL);
        insert(leftB, id);
        m_blockLayers.id = id;
        registerNamedElement(m_blockLayers);
    }

    {
        m_blockPreview.name = "Preview";
        auto id = item();
        set_behave(id, LAY_FILL);
        insert(m_root, id);
        m_blockPreview.id = id;
        registerNamedElement(m_blockPreview);
    }

    {
        m_undoRedo.name = "Actions";
        auto id = item();
        set_size_xy(id, 200, 0);
        set_behave(id, LAY_VFILL);
        insert(m_root, id);
        m_undoRedo.id = id;
        registerNamedElement(m_undoRedo);
    }
}

void LayoutBlockBuilder::tweakElements() {
    auto min = std::min(m_blockPreview.size.x, m_blockPreview.size.y);
    m_blockPreview.size = {min, min};
}

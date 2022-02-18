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

    auto left = item();
    set_size_xy(left, 200, 0);
    set_behave(left, LAY_VFILL);
    set_contain(left, LAY_COLUMN);
    insert(m_root, left);

    {
        m_currentBlock.name = "Block";
        auto id = item();
        set_size_xy(id, 0, 300);
        set_behave(id, LAY_HFILL);
        insert(left, id);
        m_currentBlock.id = id;
        registerNamedElement(m_currentBlock);
    }

    {
        m_layers.name = "Layers";
        auto id = item();
        set_behave(id, LAY_HFILL | LAY_VFILL);
        insert(left, id);
        m_layers.id = id;
        registerNamedElement(m_layers);
    }

    auto right = item();
    set_behave(right, LAY_HFILL | LAY_VFILL);
    set_contain(right, LAY_COLUMN);
    insert(m_root, right);

    {
        m_right1.name = "Right A";
        auto id = item();
        set_size_xy(id, 0, 330);
        set_behave(id, LAY_HFILL);
        insert(right, id);
        m_right1.id = id;
        registerNamedElement(m_right1);
    }

    {
        m_right2.name = "Right B";
        auto id = item();
        set_behave(id, LAY_HFILL | LAY_VFILL);
        insert(right, id);
        m_right2.id = id;
        registerNamedElement(m_right2);
    }
}

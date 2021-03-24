// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "LayoutExperimental.hpp"

void LayoutExperimental::updateElements()
{
    if (m_namedElements.empty()) createElements();
}

void LayoutExperimental::createElements()
{
    set_contain(m_root, LAY_ROW);

    {
        m_leftColumn.name = "Left column";
        auto id = item();
        set_size_xy(id, 200, 0);
        set_behave(id, LAY_VFILL);
        insert(m_root, id);
        m_leftColumn.id = id;
        registerNamedElement(m_leftColumn);
    }

    auto right = item();
    set_behave(right, LAY_HFILL | LAY_VFILL);
    set_contain(right, LAY_COLUMN);
    insert(m_root, right);

    {
        m_right1.name = "Right 1";
        auto id = item();
        set_size_xy(id, 0, 250);
        set_behave(id, LAY_HFILL);
        insert(right, id);
        m_right1.id = id;
        registerNamedElement(m_right1);
    }

    {
        m_right2.name = "Right 2";
        auto id = item();
        set_behave(id, LAY_HFILL | LAY_VFILL);
        insert(right, id);
        m_right2.id = id;
        registerNamedElement(m_right2);
    }
}

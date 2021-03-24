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
    lay_set_contain(&m_lctx, m_root, LAY_ROW);

    {
        m_leftColumn.name = "Left column";
        auto id = lay_item(&m_lctx);
        lay_set_size_xy(&m_lctx, id, 200, 0);
        lay_set_behave(&m_lctx, id, LAY_VFILL);
        lay_insert(&m_lctx, m_root, id);
        m_leftColumn.id = id;
        registerNamedElement(m_leftColumn);
    }

    auto right = lay_item(&m_lctx);
    lay_set_behave(&m_lctx, right, LAY_HFILL | LAY_VFILL);
    lay_set_contain(&m_lctx, right, LAY_COLUMN);
    lay_insert(&m_lctx, m_root, right);

    {
        m_right1.name = "Right 1";
        auto id = lay_item(&m_lctx);
        lay_set_size_xy(&m_lctx, id, 0, 250);
        lay_set_behave(&m_lctx, id, LAY_HFILL);
        lay_insert(&m_lctx, right, id);
        m_right1.id = id;
        registerNamedElement(m_right1);
    }

    {
        m_right2.name = "Right 2";
        auto id = lay_item(&m_lctx);
        lay_set_behave(&m_lctx, id, LAY_HFILL | LAY_VFILL);
        lay_insert(&m_lctx, right, id);
        m_right2.id = id;
        registerNamedElement(m_right2);
    }
}

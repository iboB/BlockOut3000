// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "LayoutBlockTest.hpp"

void LayoutBlockTest::updateElements()
{
    if (m_namedElements.empty()) createElements();
}

void LayoutBlockTest::createElements()
{
    set_contain(m_root, LAY_ROW);

    auto menu = item();
    set_size_xy(menu, 200, 0);
    set_behave(menu, LAY_VFILL);
    insert(m_root, menu);
    m_menu.name = "Menu";
    m_menu.id = menu;
    registerNamedElement(m_menu);

    auto pbox = item();
    set_behave(pbox, LAY_VFILL | LAY_HFILL);
    set_contain(pbox, LAY_ROW);
    insert(m_root, pbox);

    auto left = item();
    set_behave(left, LAY_VFILL | LAY_HFILL);
    insert(pbox, left);
    m_left.name = "Left";
    m_left.id = left;
    registerNamedElement(m_left);

    auto right = item();
    set_behave(right, LAY_VFILL | LAY_HFILL);
    insert(pbox, right);
    m_right.name = "Right";
    m_right.id = right;
    registerNamedElement(m_right);
}

void LayoutBlockTest::tweakElements()
{
    auto min = std::min(m_left.size.x, m_left.size.y);
    m_right.size = m_left.size = {min, min};
}

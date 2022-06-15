// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "GUILayout.hpp"

#include "rlayout.hpp"

GUILayout::GUILayout()
{
    lay_init_context(&m_lctx);
    lay_reserve_items_capacity(&m_lctx, 16);
    m_root = item();
}

GUILayout::~GUILayout()
{
    lay_destroy_context(&m_lctx);
}

void GUILayout::registerNamedElement(NamedElement& elem)
{
    m_namedElements.push_back(&elem);
}

void GUILayout::update(ivec2 screenSize)
{
    if (screenSize == m_size) return;
    m_size = screenSize;

    lay_set_size_xy(&m_lctx, m_root, lay_scalar(m_size.x), lay_scalar(m_size.y));

    updateElements();

    lay_run_context(&m_lctx);

    // update elements
    for (auto& elem : m_namedElements)
    {
        auto rect = lay_get_rect(&m_lctx, elem->id);
        elem->topLeft.x = rect[0];
        elem->topLeft.y = rect[1];
        elem->size.x = rect[2];
        elem->size.y = rect[3];
    }

    tweakElements();
}

void GUILayout::resetContext()
{
    lay_reset_context(&m_lctx);
    m_namedElements.clear();
    m_root = item();
    lay_set_size_xy(&m_lctx, m_root, lay_scalar(m_size.x), lay_scalar(m_size.y));
}


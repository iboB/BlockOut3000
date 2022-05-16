// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "vec2.hpp"

#include "rlayout.hpp"

#include <span>
#include <string>
#include <vector>

class GUILayout
{
public:
    GUILayout();
    virtual ~GUILayout();

    virtual const char* name() const = 0;

    struct NamedElement
    {
        std::string name;

        lay_id id;

        ivec2 topLeft;
        ivec2 size;
    };

    void registerNamedElement(NamedElement& elem);

    std::span<const NamedElement* const> namedElements() const { return m_namedElements; }

    void update(ivec2 size);

protected:
    // shorthand for lay_ functions
    lay_id item() { return lay_item(&m_lctx); }
    void set_contain(lay_id item, uint32_t flags) { lay_set_contain(&m_lctx, item, flags); }
    void set_size_xy(lay_id item, lay_scalar width, lay_scalar height) { lay_set_size_xy(&m_lctx, item, width, height); }
    void set_behave(lay_id item, uint32_t flags) { lay_set_behave(&m_lctx, item, flags); }
    void insert(lay_id parent, lay_id child) { lay_insert(&m_lctx, parent, child); }

    void resetContext();
    virtual void updateElements() {}

    lay_context m_lctx;
    lay_id m_root;
    ivec2 m_size = {};

    std::vector<NamedElement*> m_namedElements;
};
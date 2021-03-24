// BlockOut 3000
// Copyright(c) 2017-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "vec2.hpp"

#include "rlayout.hpp"

#include <itlib/memory_view.hpp>
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

    itlib::const_memory_view<NamedElement*> namedElements() const { return itlib::make_memory_view(m_namedElements); }

    void update(ivec2 size);
    
protected:
    void resetContext();
    virtual void updateElements() {}
    
    lay_context m_lctx;
    lay_id m_root;
    ivec2 m_size = {};

    std::vector<NamedElement*> m_namedElements;
};
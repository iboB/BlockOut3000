// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "lib/GUILayout.hpp"

class LayoutBlockTest final : public GUILayout
{
public:
    virtual const char* name() const override { return "block test"; }

    const NamedElement& menu() const { return m_menu; }
    const NamedElement& left() const { return m_left; }
    const NamedElement& right() const { return m_right; }

private:
    virtual void updateElements() override;
    void createElements();
    virtual void tweakElements() override;

    NamedElement m_menu;
    NamedElement m_left;
    NamedElement m_right;
};

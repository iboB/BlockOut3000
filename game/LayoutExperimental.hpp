// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "lib/GUILayout.hpp"

class LayoutExperimental final : public GUILayout
{
    virtual const char* name() const override { return "experimental"; }

    virtual void updateElements() override;

    void createElements();

    NamedElement m_leftColumn;
    NamedElement m_right1, m_right2;
};

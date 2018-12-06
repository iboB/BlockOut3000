// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <Magnum/Shaders/Flat.h>

class Flat3DPVM : public Magnum::Shaders::Flat3D
{
public:
    void setProjView(const Magnum::Matrix4& projView) {
        m_projView = projView;
        updateSuper();
    }

    const Magnum::Matrix4& projView() const {
        return m_projView;
    }

    void setModel(const Magnum::Matrix4& model) {
        m_model = model;
        updateSuper();
    }

private:
    void updateSuper();

    Magnum::Matrix4 m_projView;
    Magnum::Matrix4 m_model;
};
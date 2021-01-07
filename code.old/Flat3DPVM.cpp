// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Flat3DPVM.hpp"

void Flat3DPVM::updateSuper()
{
    setTransformationProjectionMatrix(m_projView * m_model);
}

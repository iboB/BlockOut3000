// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "AppStatePtr.hpp"
#include "BlockTemplatePtr.hpp"

AppStatePtr MakeState_BlockTest(BlockTemplatePtr block, AppStatePtr resume);

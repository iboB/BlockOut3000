// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "AppMode.hpp"

AppMode::~AppMode() = default;
bool AppMode::init() { return true; }
void AppMode::update(ms_t) {}
void AppMode::defaultRender(ivec2) {}
bool AppMode::handleEvent(const sapp_event&) { return false; }

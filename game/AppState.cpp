// BlockOut 3000
// Copyright(c) 2017-2022 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "AppState.hpp"

AppState::~AppState() = default;
bool AppState::activate() { return true; }
void AppState::update(ms_t, ivec2) {}
void AppState::defaultRender(ivec2) {}
bool AppState::handleEvent(const sapp_event&) { return false; }
void AppState::deactivate() {}
AppStatePtr AppState::getNextState() { return {}; }

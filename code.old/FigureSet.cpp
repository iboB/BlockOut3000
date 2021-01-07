// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "FigureSet.hpp"
#include "Random.hpp"

#include <algorithm>

// a named set of figures

using namespace std;

void FigureSet::addSet(const FigureSet* s)
{
    m_figureTemplates.insert(m_figureTemplates.end(), s->m_figureTemplates.begin(), s->m_figureTemplates.end());

    // erase duplicate figures
    sort(m_figureTemplates.begin(), m_figureTemplates.end());
    m_figureTemplates.erase(unique(m_figureTemplates.begin(), m_figureTemplates.end()), m_figureTemplates.end());
}

const FigureTemplate* FigureSet::getRandomFigureTemplate() const
{
    //return m_figureTemplates[0];
    return m_figureTemplates[Random::u32() % m_figureTemplates.size()];
}

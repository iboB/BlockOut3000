// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include <string>
#include <vector>

// a named set of figures

class FigureTemplate;

class FigureSet
{
public:

    const std::string& name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    const std::string& description() const { return m_description; }
    void setDescription(const std::string& desc) { m_description = desc; }

    void addFigure(FigureTemplate* f) { m_figureTemplates.push_back(f); }
    void addSet(const FigureSet* s);

    const std::vector<FigureTemplate*>& figureTemplates() const { return m_figureTemplates; }

    const FigureTemplate* getRandomFigureTemplate() const;

private:
    std::string m_name;
    std::string m_description;
    std::vector<FigureTemplate*> m_figureTemplates;
};

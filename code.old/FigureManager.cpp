// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "FigureManager.hpp"
#include "FigureTemplate.hpp"
#include "FigureSet.hpp"

#include "SimpleLexer.hpp"

#include <cassert>
#include <iostream>

using namespace std;
using namespace Magnum;

namespace
{

struct FigureManagerImpl
{
    FigureManagerImpl()
    {
    }

    bool readFigures(const std::string& input)
    {
        istringstream fin(input);

        Lexer lex(fin);

        while (true)
        {
            string token = lex.getNextToken();
            if (token == Lexer::EoF)
            {
                break;
            }
            else if (token == "set")
            {
                FigureSet* set = parseSet(lex);
                m_figureSets[set->name()].reset(set);
            }
            else
            {
                cerr << "Expected \"set\", but got " << token << endl;
                m_errorParsing = true;
                break;
            }
        }

        if (!m_errorParsing)
        {
            for (auto& fig : m_allFigureTemplates)
            {
                fig->preparePhysicalData();
            }
        }

        return !m_errorParsing;
    }

    void readOpenBlock(Lexer& lex)
    {
        string token = lex.getNextToken();
        if (token != "{")
        {
            cerr << "Expected \"{\"" << endl;
            m_errorParsing = true;
        }
    }

    Vector3i parseFigureElement(Lexer& lex)
    {
        Vector3i result;
        result.x() = lex.getInt();
        result.y() = lex.getInt();
        result.z() = lex.getInt();
        return result;
    }

    Vector3 parseFigureRotationCenter(Lexer& lex)
    {
        Vector3 result;
        result.x() = lex.getFloat();
        result.y() = lex.getFloat();
        result.z() = lex.getFloat();
        return result;
    }

    FigureTemplate* parseFigureTemplate(Lexer& lex)
    {
        std::unique_ptr<FigureTemplate> theFigure(new FigureTemplate);

        theFigure->setName(lex.getQuotedString());

        readOpenBlock(lex);

        while (true)
        {
            string token = lex.getNextToken();

            if (token == "e")
            {
                theFigure->addElement(parseFigureElement(lex));
            }
            else if (token == "c")
            {
                auto c = parseFigureRotationCenter(lex);
                theFigure->setRotationCenter(c);
            }
            else if (token == "}")
            {
                return theFigure.release();
            }
            else
            {
                cerr << "Expected \"e\" or \"c[axis]\" or \"}\", but got " << token << endl;
                m_errorParsing = true;
                return nullptr;
            }
        }

        assert(false); // wtf
        return nullptr;
    }

    FigureSet* parseSet(Lexer& lex)
    {
        std::unique_ptr<FigureSet> theSet(new FigureSet);

        theSet->setName(lex.getQuotedString());
        auto& sets = m_figureSets;

        auto sfound = sets.find(theSet->name());
        if (sfound != sets.end())
        {
            cerr << "Set \"" << theSet->name() << "\" has already been defined" << endl;
            m_errorParsing = true;
            return nullptr;
        }

        readOpenBlock(lex);

        while (true)
        {
            string token = lex.getNextToken();
            if (token == "desc")
            {
                theSet->setDescription(lex.getQuotedString());
            }
            else if (token == "figure")
            {
                FigureTemplate* fig = parseFigureTemplate(lex);
                m_allFigureTemplates.emplace_back(fig);
                theSet->addFigure(fig);
            }
            else if (token == "}")
            {
                return theSet.release();
            }
            else if (token == "include")
            {
                string inc = lex.getQuotedString();
                if (inc == theSet->name())
                {
                    cerr << "Trying to include an the set \"" << inc << "\" in itself" << endl;
                    m_errorParsing = true;
                    return nullptr;
                }

                auto found = sets.find(inc);

                if (found == sets.end())
                {
                    cerr << "Trying to include an unknown set \"" << inc << "\"" << endl;
                    m_errorParsing = true;
                    return nullptr;
                }

                const FigureSet* includeSet = found->second.get();
                theSet->addSet(includeSet);
            }
            else
            {
                cerr << "Expected \"figure\" or \"desc\" or \"}\", but got " << token << endl;
                m_errorParsing = true;
                return nullptr;
            }
        }

        assert(false); // shouldn't get here
        return nullptr;
    }

    bool m_errorParsing = false;
    unordered_map<string, unique_ptr<FigureSet>> m_figureSets;
    vector<unique_ptr<FigureTemplate>> m_allFigureTemplates;
};

FigureManagerImpl* instance;

}

void FigureManager::initialize()
{
    assert(!instance);
    instance = new FigureManagerImpl;
}

void FigureManager::cleanup()
{
    delete instance;
    instance = nullptr;
}

bool FigureManager::readFigures(const std::string& input)
{
    return instance->readFigures(input);
}

const FigureSet* FigureManager::getFigureSet(const string& name)
{
    auto s = instance->m_figureSets.find(name);

    if (s == instance->m_figureSets.end())
        return nullptr;

    return s->second.get();
}

// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "SimpleLexer.hpp"

#include <iostream>
#include <algorithm>
#include <cctype>

// a simple lexer class that gets tokes from an istream

using namespace std;

namespace
{
namespace str
{
inline std::string& trim(std::string& str)
{
    str.erase(str.begin(), find_if(str.begin(), str.end(),
        [](char& ch)->bool { return !std::isspace(ch); }));
    str.erase(find_if(str.rbegin(), str.rend(),
        [](char& ch)->bool { return !std::isspace(ch); }).base(), str.end());
    return str;
}
}
}

const string Lexer::EoF = "{eof}";

Lexer::Lexer(istream& in)
    : m_input(in)
    , m_lineNumber(0)
    , m_hasReachedEnd(false)
{
}

string Lexer::getNextToken()
{
    if(m_lineStream.eof())
    {
        m_currentLine.clear();

        while(m_currentLine.empty())
        {
            if(m_input.eof())
            {
                if(m_hasReachedEnd)
                {
                    cerr << "Read past end of stream" << endl;
                    return EoF;
                }
                else
                {
                    m_hasReachedEnd = true;
                    return EoF;
                }
            }

            getline(m_input, m_currentLine);
            ++m_lineNumber;

            // elliminate comments
            size_t i = m_currentLine.find('#');
            if(i != string::npos)
                m_currentLine.erase(i);

            str::trim(m_currentLine);
        }

        m_lineStream.str(m_currentLine);
        m_lineStream.clear();
    }

    string token;
    m_lineStream >> token;

    if(token.empty()) // this could happen at most once, so there's no risk of stack overflow
        return getNextToken();

    //merge text in quotes as a single token
    if(token.front() == '"' && token.length() > 1 && token.back() != '"')
    {
        string restOfQuotedString;
        getline(m_lineStream, restOfQuotedString, '"');

        token += restOfQuotedString;
        token += '"';

        if(m_lineStream.eof())
        {
            cerr << "String " << token << " has no closing quotation mark" << endl;
            return "";
        }
    }

    return token;
}

string Lexer::getQuotedString()
{
    string token = getNextToken();

    if(token.length() < 2 || token.front() != '"' || token.back() != '"')
    {
        cerr << "Expected string in quotation marks" << endl;
        return "";
    }

    // remove quotes
    token.erase(token.begin());
    token.pop_back();

    return token;
}

int Lexer::getInt()
{
    string token = getNextToken();
    return strtol(token.c_str(), nullptr, 10);
}

float Lexer::getFloat()
{
    string token = getNextToken();
    return strtof(token.c_str(), nullptr);
}

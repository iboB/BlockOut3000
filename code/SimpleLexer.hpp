// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// a simple lexer class that gets tokes from an istream

#include <istream>
#include <string>
#include <sstream>

class Lexer
{
public:
    Lexer(std::istream& in);

    static const std::string EoF;

    unsigned currentLineNumber() const { return m_lineNumber; }

    std::string getNextToken();

    std::string getQuotedString();
    int getInt();
    float getFloat();

private:
    std::istream& m_input;
    std::string m_currentLine;
    std::istringstream m_lineStream;
    unsigned m_lineNumber;
    bool m_hasReachedEnd;
};

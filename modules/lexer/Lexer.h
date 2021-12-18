

#ifndef TKOM_21Z_DANIEL_LEXER_H
#define TKOM_21Z_DANIEL_LEXER_H

#include <iosfwd>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "../token/Token.h"
class Lexer {
public:
    int line = 1;
    int column = 0;
    std::istream &handle;
    bool end_of_file = false;
    bool first_line = true;
    bool first_indentation = true;
    char current_char;
    Token active_token;

    char peekNextChar();
    bool parseIndentation();
    bool parseKeywordOrId();
    bool parseOperators();
    bool parseString();
    bool parseNumber();
    bool parseDate();
    bool parseTimeDiff();
    bool ignore_comment();

    Lexer(std::istream &my_handle);
    Token getNextToken();
    void getNextChar();
    bool endOfFile();
    std::string getWholeLine(std::streampos &getFrom);
};


#endif //TKOM_21Z_DANIEL_LEXER_H

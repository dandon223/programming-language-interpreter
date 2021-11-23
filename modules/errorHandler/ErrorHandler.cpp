
#include <iostream>
#include "ErrorHandler.h"


void ErrorHandler::printLexerError(Token token, std::string line) {
    std::cerr << "\nError in Lexer.: "<< TokenValueToString(token.value) <<" on line "<<token.line_number<<" in column " <<token.column_number << std::endl;
    std::cerr << line << std::endl;
}
void ErrorHandler::printParserError(Token token, std::string line) {
    std::cerr << "\nError in Parser on line "<<token.line_number<<" in column " <<token.column_number << std::endl;
    std::cerr << line << std::endl;
}
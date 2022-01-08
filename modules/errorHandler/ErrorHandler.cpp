
#include <iostream>
#include "ErrorHandler.h"


void ErrorHandler::printLexerError(Token token, std::string line) {
    std::cerr << "\nError in Lexer.: "<< TokenValueToString(token.value) <<" on line "<<token.line_number<<" in column " <<token.column_number << std::endl;
    std::cerr << line << std::endl;
    LexerException lexerException = LexerException();
    lexerException.error = line;
    throw lexerException;
}
void ErrorHandler::printParserError(Token token, std::string line) {
    std::cerr << "\nError in Parser on line "<<token.line_number<<" in column " <<token.column_number << std::endl;
    std::cerr << line << std::endl;
    ParserException parserException = ParserException();
    parserException.error = line;
    throw parserException;
}
void ErrorHandler::printInterpreterError(std::string line) {
    std::cerr << "\nError while interpreting\n";
    std::cerr << line << std::endl;
    InterpreterException interpreterException = InterpreterException();
    interpreterException.error = line;
    throw interpreterException;
}
void ErrorHandler::printInterpreterError(std::string message, int line, int column) {
    std::cerr << "\nError while interpreting\n";
    std::cerr << message << " on line "<<line<<" in column "<<column<<std::endl;
    InterpreterException interpreterException = InterpreterException();
    interpreterException.error = message;
    throw interpreterException;
}
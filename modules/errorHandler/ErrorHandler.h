

#ifndef TKOM_21Z_DANIEL_ERRORHANDLER_H
#define TKOM_21Z_DANIEL_ERRORHANDLER_H
#include <exception>
#include "../token/Token.h"
class LexerException: public std::exception{
public:
    std::string error= "";
    LexerException() = default;
};
class ParserException: public std::exception{
public:
    std::string error= "";
    ParserException() = default;
};
class InterpreterException: public std::exception{
public:
    std::string error= "";
    InterpreterException() = default;
};
class ErrorHandler {
private:
public:
    ErrorHandler() = delete;
    ErrorHandler(const ErrorHandler &) = delete;
    ~ErrorHandler() = delete;
    ErrorHandler &operator=(const ErrorHandler &) = delete;
    ErrorHandler &operator=(ErrorHandler &) = delete;
    static void printLexerError(Token token , std::string line);
    static void printParserError(Token token , std::string line);
    static void printInterpreterError(std::string line);
};


#endif //TKOM_21Z_DANIEL_ERRORHANDLER_H

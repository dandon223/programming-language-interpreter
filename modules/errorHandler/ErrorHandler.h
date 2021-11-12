

#ifndef TKOM_21Z_DANIEL_ERRORHANDLER_H
#define TKOM_21Z_DANIEL_ERRORHANDLER_H

#include "../token/Token.h"
class ErrorHandler {
private:
public:
    ErrorHandler() = delete;
    ErrorHandler(const ErrorHandler &) = delete;
    ~ErrorHandler() = delete;
    ErrorHandler &operator=(const ErrorHandler &) = delete;
    ErrorHandler &operator=(ErrorHandler &) = delete;
    static void printLexerError(Token token , std::string line);
};


#endif //TKOM_21Z_DANIEL_ERRORHANDLER_H

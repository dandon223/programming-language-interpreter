

#ifndef TKOM_21Z_DANIEL_PARSER_H
#define TKOM_21Z_DANIEL_PARSER_H

#include "../token/Token.h"
#include "../errorHandler/ErrorHandler.h"
#include "Program.h"
#include "../lexer/Lexer.h"
#include <optional>
class Parser {
private:
    Token currentToken;
    Lexer lex;
public:
    Parser(Lexer lexer);
    std::optional<Program>TryToParseProgram();
private:
    void getNextToken();
    void expect(TokenType ttype, std::string message);
    void expect_and_accept(TokenType ttype, std::string message);
    std::optional<Declaration>TryToParseVariableDeclaration();
    TypeOfData getTypeOfData(TokenType type);
};


#endif //TKOM_21Z_DANIEL_PARSER_H

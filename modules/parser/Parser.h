

#ifndef TKOM_21Z_DANIEL_PARSER_H
#define TKOM_21Z_DANIEL_PARSER_H

#include "../token/Token.h"
#include "../errorHandler/ErrorHandler.h"
#include "Program.h"
#include "../lexer/Lexer.h"
#include <memory>
class Parser {
private:
    Token currentToken;
    Lexer lex;
public:
    Parser(Lexer lexer);
    std::shared_ptr<Program>TryToParseProgram();
private:
    void getNextToken();
    void expect(TokenType ttype, std::string message);
    void expect_and_accept(TokenType ttype, std::string message);
    std::shared_ptr<Declaration>TryToParseVariableDeclaration();
    std::shared_ptr<IExpression>TryToParseExpression();
    std::shared_ptr<IExpression> TryToParseAdvancedExpression();
    std::shared_ptr<IExpression> parseBasicExpression();
    TypeOfData getTypeOfData(TokenType type);
    std::string getOperatorType();
    std::shared_ptr<Expression>CreateExpression(std::string basicString, std::shared_ptr<IExpression> expression, std::shared_ptr<IExpression> expression1);
    std::shared_ptr<AdvExpression>CreateAdvExpression(std::string basicString, std::shared_ptr<IExpression> expression, std::shared_ptr<IExpression> expression1);
};


#endif //TKOM_21Z_DANIEL_PARSER_H

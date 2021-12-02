

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
    std::unique_ptr<Program>TryToParseProgram();
private:
    void getNextToken();
    void expect(TokenType ttype, std::string message);
    void expect_and_accept(TokenType ttype, std::string message);
    std::unique_ptr<INode>TryToParseVariableDeclaration();
    std::unique_ptr<INode>TryToParseFunction();
    std::unique_ptr<IExpression>TryToParseExpression();
    std::unique_ptr<IExpression> TryToParseParenthesisExpresion();
    std::unique_ptr<IExpression> TryToParseAdvancedExpression();
    std::unique_ptr<IExpression> TryToParseBasicExpression();
    std::unique_ptr<FunCall> TryToParseFunctionCall(std::string id);
    std::vector<std::shared_ptr<IExpression>> TryToParseArguments();
    std::vector<VariableDeclr> TryToParseParameters();
    std::unique_ptr<INode> TryToParseFunctionOrVarDefinition();
    TypeOfData getTypeOfData(TokenType type);
    std::string getOperatorType();
    std::unique_ptr<Expression>CreateExpression(std::string basicString, std::unique_ptr<IExpression> expression, std::unique_ptr<IExpression> expression1);
    std::unique_ptr<AdvExpression>CreateAdvExpression(std::string basicString, std::unique_ptr<IExpression> expression, std::unique_ptr<IExpression> expression1);
};


#endif //TKOM_21Z_DANIEL_PARSER_H

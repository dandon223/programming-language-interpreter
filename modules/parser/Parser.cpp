

#include "Parser.h"

Parser::Parser(Lexer lexer) : lex(lexer)
{
    currentToken = lex.getNextToken();
}
void Parser::getNextToken()
{
    currentToken = lex.getNextToken();
}
std::optional<Program> Parser::TryToParseProgram()
{
    std::vector<Declaration> variable_declarations;
    std::cout << "Beginning Parsing\n";
    while (auto s = TryToParseVariableDeclaration())
    {
        if (s)
            variable_declarations.push_back(s.value());
        else
        {
            expect(TokenType::End_of_text, "no end of text in TryToParseProgram");
        }
    }
    if (!variable_declarations.empty()){
        std::cout << "Parsing Successfull\n";
    }
    else{
        std::cout << "Parsing Unsuccessful\n";
    }
    return Program(variable_declarations);
}
std::optional<Declaration> Parser::TryToParseVariableDeclaration(){
    if(currentToken.type == TokenType::End_of_text)
        return std::nullopt;
    expect_and_accept(TokenType::Indentation,"no indentation at the begining of variable declaration");
    if (currentToken.type != TokenType::Int || currentToken.type == TokenType::End_of_text)
        return std::nullopt;

    VariableDeclr var;
    std::optional<Expression> assignable;
    var.typeOfData = getTypeOfData(currentToken.type);
    getNextToken();

    expect(TokenType::Id, "no id in declare statement");
    var.id = std::get<std::string>(currentToken.value);
    getNextToken();

    if (currentToken.type != TokenType::Assignment)
        return Declaration(var, assignable);

//    getNextToken();
//    auto svar = parseExpression();
//    if (!svar)
//    {
//        ParserException(currentToken, "no expression after assign op in declaration");
//        return std::nullopt;
//    }
//    assignable = svar;
//
//    expect_and_accept(TokenType::Semicolon, "no semicolon after assignment2");
//    return DeclareStatement(var, assignable);
    return std::nullopt;
}
TypeOfData Parser::getTypeOfData(TokenType type){
    return TypeOfData::Integer;

}
void Parser::expect_and_accept(TokenType ttype, std::string message)
{
    if (currentToken.type == ttype){
        getNextToken();
        return;
    }
    ErrorHandler::printParserError(currentToken,message);
}
void Parser::expect(TokenType ttype, std::string message)
{
    if (currentToken.type == ttype)
        return;
    ErrorHandler::printParserError(currentToken,message);
}

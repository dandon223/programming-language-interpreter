


#include "Parser.h"

Parser::Parser(Lexer lexer) : lex(lexer)
{
    currentToken = lex.getNextToken();
}
void Parser::getNextToken()
{
    currentToken = lex.getNextToken();
}
std::unique_ptr<Program> Parser::TryToParseProgram()
{
    std::vector<std::unique_ptr<Declaration>> variable_declarations;
    std::cout << "Beginning Parsing\n";
    expect_and_accept(TokenType::Indentation,"no indentation at the begining of variable declaration");
    while (auto s = TryToParseVariableDeclaration())
    {
        if (s){
            variable_declarations.push_back(std::move(s));
        }
        else{
            expect(TokenType::End_of_text, "no end of text in TryToParseProgram");
        }
        if(currentToken.type != TokenType::End_of_text)
            expect_and_accept(TokenType::Indentation,"no indentation at the begining of variable declaration");
    }
    if (!variable_declarations.empty()){
        std::cout << "Parsing Successfull\n";
    }
    else{
        std::cout << "Parsing Unsuccessful\n";
    }
    return std::make_unique<Program>(Program(std::move(variable_declarations)));
}
std::unique_ptr<Declaration> Parser::TryToParseVariableDeclaration(){
    if (currentToken.type != TokenType::Int)
        return nullptr;

    VariableDeclr var;
    std::unique_ptr<IExpression> assignable;
    var.typeOfData = getTypeOfData(currentToken.type);
    getNextToken();

    expect(TokenType::Id, "no id in declare statement");
    var.id = std::get<std::string>(currentToken.value);
    getNextToken();

    if (currentToken.type != TokenType::Assignment)
        return std::make_unique<Declaration>( Declaration(var, nullptr));

    getNextToken();
    assignable = TryToParseExpression();
    if (!assignable)
    {
        ErrorHandler::printParserError(currentToken, "no expression after assign op in declaration");
        return nullptr;
    }
    return std::make_unique<Declaration>(Declaration(var, std::move(assignable)));
}
std::unique_ptr<IExpression> Parser::TryToParseExpression() {
    std::string my_operator;
    auto left = TryToParseAdvancedExpression();
    if (left == nullptr)
        return nullptr;
    while (currentToken.type == TokenType::Plus || currentToken.type == TokenType::Minus)
    {
        my_operator = getOperatorType();
        getNextToken();
        auto right = TryToParseAdvancedExpression();
        if(right == nullptr)
            ErrorHandler::printParserError(currentToken, "no expression after "+my_operator);
        left = CreateExpression(my_operator,std::move(left),std::move(right));
    }
    return left;
}
std::unique_ptr<IExpression> Parser::TryToParseAdvancedExpression()
{

    std::string my_operator;
    auto left = TryToParseBasicExpression();
    if (left == nullptr)
        return nullptr;
    while (currentToken.type == TokenType::Multiply || currentToken.type == TokenType::Divide)
    {
        my_operator = getOperatorType();
        getNextToken();
        auto right = TryToParseBasicExpression();
        if(right == nullptr)
            ErrorHandler::printParserError(currentToken, "no expression after "+my_operator);
        left = CreateAdvExpression(my_operator,std::move(left),std::move(right));
    }
    return left;
}
std::unique_ptr<IExpression> Parser::TryToParseBasicExpression()
{

    std::variant<int,double,std::string> basic;
    bool wasMinus = false;
    if (currentToken.type == TokenType::Minus)
    {
        wasMinus = true;
        getNextToken();
    }
    if (currentToken.type != TokenType::Number && currentToken.type != TokenType::StringValue){
        return nullptr;
    }
    else if (currentToken.type == TokenType::Number)
    {
        if(currentToken.value.index()==0)
            basic = std::get<int>(currentToken.value);
        else
            basic = std::get<double>(currentToken.value);
        getNextToken();
        return std::make_unique<BasicExpression>(BasicExpression(basic, wasMinus));
    }
    else if (currentToken.type == TokenType::StringValue){
        basic = std::get<std::string>(currentToken.value);
        getNextToken();
        return std::make_unique<BasicExpression>(BasicExpression(basic, wasMinus));
    }
    return nullptr;

}
std::unique_ptr<Expression> Parser::CreateExpression(std::string my_operator, std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right) {

    return std::make_unique<Expression>(Expression(my_operator, std::move(left),std::move(right)));
}
std::unique_ptr<AdvExpression> Parser::CreateAdvExpression(std::string my_operator, std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right) {

    return std::make_unique<AdvExpression>(AdvExpression(my_operator, std::move(left),std::move(right)));
}
TypeOfData Parser::getTypeOfData(TokenType type){
    return TypeOfData::Integer;
}
std::string Parser::getOperatorType(){
    if(currentToken.type == TokenType::Plus)
        return "+";
    if(currentToken.type == TokenType::Minus)
        return "-";
    if(currentToken.type == TokenType::Multiply)
        return "*";
    if(currentToken.type == TokenType::Divide)
        return "/";
    return "";
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



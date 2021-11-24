


#include "Parser.h"

Parser::Parser(Lexer lexer) : lex(lexer)
{
    currentToken = lex.getNextToken();
}
void Parser::getNextToken()
{
    currentToken = lex.getNextToken();
}
std::shared_ptr<Program> Parser::TryToParseProgram()
{
    std::vector<std::shared_ptr<Declaration>> variable_declarations;
    std::cout << "Beginning Parsing\n";
    while (auto s = TryToParseVariableDeclaration())
    {
        if (s)
            variable_declarations.push_back(std::move(s));
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
    return std::make_shared<Program>(Program(variable_declarations));
}
std::shared_ptr<Declaration> Parser::TryToParseVariableDeclaration(){
    if(currentToken.type == TokenType::End_of_text)
        return nullptr;
    expect_and_accept(TokenType::Indentation,"no indentation at the begining of variable declaration");
    if (currentToken.type != TokenType::Int || currentToken.type == TokenType::End_of_text)
        return nullptr;

    VariableDeclr var;
    std::shared_ptr<IExpression> assignable;
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
    return std::make_shared<Declaration>(Declaration(var, std::move(assignable)));
}
std::shared_ptr<IExpression> Parser::TryToParseExpression() {
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
std::shared_ptr<IExpression> Parser::TryToParseAdvancedExpression()
{

    std::string my_operator;
    auto left = parseBasicExpression();
    if (left == nullptr)
        return nullptr;
    while (currentToken.type == TokenType::Multiply || currentToken.type == TokenType::Divide)
    {
        left = std::dynamic_pointer_cast<AdvExpression>(left);
        my_operator = getOperatorType();
        getNextToken();
        auto right = parseBasicExpression();
        if(right == nullptr)
            ErrorHandler::printParserError(currentToken, "no expression after "+my_operator);
        left = CreateAdvExpression(my_operator,std::move(left),std::move(right));
    }
    return left;
}
std::shared_ptr<IExpression> Parser::parseBasicExpression()
{

    std::variant<int,std::string> basic;
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
        basic = std::get<int>(currentToken.value);
        getNextToken();
        return std::make_shared<BasicExpression>(BasicExpression(basic, wasMinus));
    }
    else if (currentToken.type == TokenType::StringValue){
        basic = std::get<std::string>(currentToken.value);
        getNextToken();
        return std::make_shared<BasicExpression>(BasicExpression(basic, wasMinus));
    }
    return nullptr;

}
std::shared_ptr<Expression> Parser::CreateExpression(std::string my_operator, std::shared_ptr<IExpression> left, std::shared_ptr<IExpression> right) {

    return std::make_shared<Expression>(Expression(my_operator, std::move(left),std::move(right)));
}
std::shared_ptr<AdvExpression> Parser::CreateAdvExpression(std::string my_operator, std::shared_ptr<IExpression> left, std::shared_ptr<IExpression> right) {

    return std::make_shared<AdvExpression>(AdvExpression(my_operator, std::move(left),std::move(right)));
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



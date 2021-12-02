


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
    std::vector<std::unique_ptr<Function>> functions;
    std::cout << "Beginning Parsing\n";
    expect_and_accept(TokenType::Indentation,"no indentation at the begining of variable declaration");
    while (auto s = TryToParseFunctionOrVarDefinition())
    {
        std::unique_ptr<Function> function(dynamic_cast<Function*>(s.get()));
        std::unique_ptr<Declaration> declaration(dynamic_cast<Declaration*>(s.release()));

        if(function)
            functions.push_back(std::move(function));
        else if(declaration)
            variable_declarations.push_back(std::move(declaration));
        else{
            expect(TokenType::End_of_text, "no end of text in TryToParseProgram");
        }
        if(currentToken.type != TokenType::End_of_text)
            expect_and_accept(TokenType::Indentation,"no indentation at the begining of variable declaration");
    }
    if (!variable_declarations.empty() || !functions.empty()){
        std::cout << "Parsing Successfull\n";
    }
    else{
        std::cout << "Parsing Unsuccessful or empty file\n";
    }
    return std::make_unique<Program>(Program(std::move(variable_declarations),std::move(functions)));
}
std::unique_ptr<INode> Parser::TryToParseFunctionOrVarDefinition(){
    auto s = TryToParseFunction();
    if(s)
        return s;
    s = TryToParseVariableDeclaration();
    if(s)
        return s;
    return nullptr;
}
std::unique_ptr<INode> Parser::TryToParseFunction(){
    if(currentToken.type != TokenType::Function) // 'fun'
        return nullptr;
    getNextToken();

    if (currentToken.type != TokenType::Int && currentToken.type != TokenType::Float  // typ
        && currentToken.type != TokenType::String && currentToken.type != TokenType::Date && currentToken.type != TokenType::Time_diff){
        ErrorHandler::printParserError(currentToken,"no proper Type after 'fun'");
        return nullptr;
    }
    TypeOfData typeOfData = getTypeOfData(currentToken.type);
    getNextToken();

    expect(TokenType::Id, "no id in declare statement");
    std::string id = std::get<std::string>(currentToken.value);
    getNextToken();
    std::vector<VariableDeclr> params = TryToParseParameters();
    expect_and_accept(TokenType::Right_parentheses,"no ) after parameters");
    return std::make_unique<Function>(Function(id,typeOfData,params));
}
std::vector<VariableDeclr> Parser::TryToParseParameters(){
    std::vector<VariableDeclr> parameters;

    expect(TokenType::Left_parentheses, "No left parentheses in function declaration\n");
    if (currentToken.type == TokenType::Right_parentheses)
    {
        return parameters;
    }
    do
    {
        getNextToken();
        if (currentToken.type == TokenType::Right_parentheses)
        {
            return parameters;
        }
        if (currentToken.type != TokenType::Int && currentToken.type != TokenType::Float  // typ
            && currentToken.type != TokenType::String && currentToken.type != TokenType::Date && currentToken.type != TokenType::Time_diff)
            ErrorHandler::printParserError(currentToken,"No valid type or no type");
        TypeOfData tod = getTypeOfData(currentToken.type);
        getNextToken();
        expect(TokenType::Id, "no id after type\n");
        std::string paramid = std::get<std::string>(currentToken.value);
        parameters.push_back(VariableDeclr(paramid, tod));
        getNextToken();
    } while (currentToken.type == TokenType::Comma);

    return parameters;
}
std::unique_ptr<INode> Parser::TryToParseVariableDeclaration(){
    if (currentToken.type != TokenType::Int && currentToken.type != TokenType::Float
    && currentToken.type != TokenType::String && currentToken.type != TokenType::Date && currentToken.type != TokenType::Time_diff)
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

    std::unique_ptr<INode> basic;
    bool wasMinus = false;
    if (currentToken.type == TokenType::Minus)
    {
        wasMinus = true;
        getNextToken();
    }
    if(currentToken.type == TokenType::Number)
    {
        if(currentToken.value.index()==0)
            basic = std::make_unique<Int>(std::get<int>(currentToken.value));//std::get<int>(currentToken.value);
        else
            basic = std::make_unique<Double>(std::get<double>(currentToken.value));
        getNextToken();
        return std::make_unique<BasicExpression>(BasicExpression(std::move(basic), wasMinus));
    }
    else if (currentToken.type == TokenType::StringValue){
        basic = std::make_unique<String>(std::get<std::string>(currentToken.value));
        getNextToken();
        return std::make_unique<BasicExpression>(BasicExpression(std::move(basic), wasMinus));
    }
    else if (currentToken.type == TokenType::DateValue){
        basic = std::make_unique<Date>(std::get<Date>(currentToken.value));
        getNextToken();
        return std::make_unique<BasicExpression>(BasicExpression(std::move(basic), wasMinus));
    }
    else if (currentToken.type == TokenType::TimeDiffValue){
        basic =std::make_unique<TimeDiff>(std::get<TimeDiff>(currentToken.value));
        getNextToken();
        return std::make_unique<BasicExpression>(BasicExpression(std::move(basic), wasMinus));
    }
    else if(currentToken.type == TokenType::Id){
        std::string id = std::get<std::string>(currentToken.value);
        basic = std::make_unique<VariableAccess>(VariableAccess(id));
        getNextToken();
        auto function = TryToParseFunctionCall(id);
        if(function)
            basic = std::move(function);
        return std::make_unique<BasicExpression>(BasicExpression(std::move(basic), wasMinus));
    }
    else if(currentToken.type == TokenType::Left_parentheses){
        auto expr = TryToParseParenthesisExpresion();
        return expr;
    }
    return nullptr;

}
std::unique_ptr<IExpression> Parser::TryToParseParenthesisExpresion(){
    expect_and_accept(TokenType::Left_parentheses, "no left parenthesis");
    auto expr = TryToParseExpression();
    expect_and_accept(TokenType::Right_parentheses, "no right parenthesis");
    return expr;
}
std::unique_ptr<FunCall> Parser::TryToParseFunctionCall(std::string id){
    if(currentToken.type != TokenType::Left_parentheses)
        return nullptr;
    getNextToken();
    std::vector<std::shared_ptr<IExpression>> arguments = TryToParseArguments();
    return std::make_unique<FunCall>(FunCall(id,std::move(arguments)));
}
std::vector<std::shared_ptr<IExpression>> Parser::TryToParseArguments(){
    std::vector<std::shared_ptr<IExpression>> arguments;
    if (currentToken.type == TokenType::Right_parentheses)
    {
        getNextToken();
        return arguments;
    }
    bool x;
    do
    {
        auto s = TryToParseExpression();
        if(s == nullptr)
            ErrorHandler::printParserError(currentToken,"Failed to parse arguments");
        if(s!= nullptr)
            arguments.push_back(std::move(s));
        if (currentToken.type == TokenType::Comma){
            getNextToken();
            x = true;
        }
        else{
            x = false;
        }

    } while (x);
    expect_and_accept(TokenType::Right_parentheses,"No ) after arguments");
    return arguments;
}
std::unique_ptr<Expression> Parser::CreateExpression(std::string my_operator, std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right) {

    return std::make_unique<Expression>(Expression(my_operator, std::move(left),std::move(right)));
}
std::unique_ptr<AdvExpression> Parser::CreateAdvExpression(std::string my_operator, std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right) {

    return std::make_unique<AdvExpression>(AdvExpression(my_operator, std::move(left),std::move(right)));
}
TypeOfData Parser::getTypeOfData(TokenType type){
    if (type == TokenType::Int)
        return TypeOfData::Integer;
    if (type == TokenType::Float)
        return TypeOfData::Double;
    if (type == TokenType::String)
        return TypeOfData::Message;
    if (type == TokenType::Date)
        return TypeOfData::Date;
    else
        return TypeOfData::TimeDiff;
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



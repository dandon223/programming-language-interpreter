


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
    int indentation = 0;
    if(currentToken.type == TokenType::Indentation)
        indentation = currentToken.length;
    expect_and_accept(TokenType::Indentation,"no indentation at the begining of variable declaration");
    while (auto s = TryToParseFunctionOrVarDefinition(indentation))
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
        if(currentToken.type != TokenType::End_of_text){
            if(currentToken.type == TokenType::Indentation)
                indentation = currentToken.length;
            expect_and_accept(TokenType::Indentation,"no indentation at the begining of variable declaration");
        }
    }
    expect_and_accept(TokenType::End_of_text,"expected end of stream, did not parse everything");
    if (!variable_declarations.empty() || !functions.empty())
        std::cout << "Parsing Successfull\n";
    else
        std::cout << "Parsing Unsuccessful or empty file\n";
    return std::make_unique<Program>(Program(std::move(variable_declarations),std::move(functions)));
}
std::unique_ptr<INode> Parser::TryToParseFunctionOrVarDefinition(int indentation){
    if(indentation>0){
        ErrorHandler::printParserError(currentToken,"indentation before function def or global variable should be 0");
        return nullptr;
    }
    auto s = TryToParseFunction(indentation);
    if(s)
        return s;
    s = TryToParseVariableDeclaration();
    if(s)
        return s;
    return nullptr;
}
std::unique_ptr<INode> Parser::TryToParseFunction(int indentation){
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
    expect_and_accept(TokenType::Colon,"no : after )");
    auto body = TryToParseBody(indentation);
    return std::make_unique<Function>(Function(id,typeOfData,params,std::move(body)));
}
std::unique_ptr<Body> Parser::TryToParseBody(int indentation) {
    std::vector<std::shared_ptr<INode>> statements;
    while(currentToken.type == TokenType::Indentation && currentToken.length == indentation+4){
        getNextToken();
        if(auto s = TryToParseVariableDeclaration()){
            statements.push_back(std::move(s));
        }else if(auto x = TryToParseAssignOrFunCall()){
            statements.push_back(std::move(x));
        }else if(auto z = TryToParseReturnStatement()){
            statements.push_back(std::move(z));
        }else if(auto y = TryToParseWhileStatement(indentation+4)){
            statements.push_back(std::move(y));
        }else if(auto c = TryToParseIfStatement(indentation+4)){
            statements.push_back(std::move(c));
        }else if(auto q = TryToParseElseStatement(indentation+4)){
            statements.push_back(std::move(q));
        }
    }
    return std::make_unique<Body>(Body(std::move(statements)));
}
std::unique_ptr<Else> Parser::TryToParseElseStatement(int indentation){
    if (currentToken.type != TokenType::Else)
        return nullptr;
    getNextToken();
    expect_and_accept(TokenType::Colon, "no : after else");
    auto body = TryToParseBody(indentation);
    if(body == nullptr)
        ErrorHandler::printParserError(currentToken,"if statement has no body");
    return std::make_unique<Else>(Else(std::move(body)));
}
std::unique_ptr<If> Parser::TryToParseIfStatement(int indentation){
    if (currentToken.type != TokenType::If)
        return nullptr;
    getNextToken();
    auto condition = TryToParseCondition();
    if(condition == nullptr)
        ErrorHandler::printParserError(currentToken,"not able to parse condition");
    expect_and_accept(TokenType::Colon, "no : after condition");
    auto body = TryToParseBody(indentation);
    if(body == nullptr)
        ErrorHandler::printParserError(currentToken,"if statement has no body");
    return std::make_unique<If>(If(std::move(condition), std::move(body)));
}
std::unique_ptr<While> Parser::TryToParseWhileStatement(int indentation){
    if (currentToken.type != TokenType::While)
        return nullptr;
    getNextToken();
    auto condition = TryToParseCondition();
    if(condition == nullptr)
        ErrorHandler::printParserError(currentToken,"not able to parse condition");
    expect_and_accept(TokenType::Colon, "no : after condition");
    auto body = TryToParseBody(indentation);
    if(body == nullptr)
        ErrorHandler::printParserError(currentToken,"while statement has no body");
    return std::make_unique<While>(While(std::move(condition), std::move(body)));
}
std::unique_ptr<Return> Parser::TryToParseReturnStatement(){
    if (currentToken.type != TokenType::Return)
        return nullptr;
    getNextToken();
    auto expr = TryToParseExpression();
    return std::make_unique<Return>(Return(std::move(expr)));
}
std::unique_ptr<INode> Parser::TryToParseAssignOrFunCall(){
    if (currentToken.type != TokenType::Id)
        return nullptr;
    std::string id = std::get<std::string>(currentToken.value);
    getNextToken();
    if (currentToken.type == TokenType::Left_parentheses)
        return TryToParseFunctionCall(id);
    if (currentToken.type == TokenType::Assignment)
        return TryToParseAssignStatement(id);
    ErrorHandler::printParserError(currentToken, "expected ( or =\n");
    return nullptr;
}
std::unique_ptr<AssignStatement> Parser::TryToParseAssignStatement(std::string id){
    if (currentToken.type != TokenType::Assignment)
    {
        ErrorHandler::printParserError(currentToken, " No assignment op. Expected assignment op ");
        return nullptr;
    }
    getNextToken();
    auto var = std::make_unique<VariableAccess>(VariableAccess(id));

    auto s = TryToParseExpression();
    if(!s)
        ErrorHandler::printParserError(currentToken,  "failed to parse expression in AssignStatement\n");;
    if (currentToken.type == TokenType::Right_parentheses)
        getNextToken();
    return std::make_unique<AssignStatement>(AssignStatement(std::move(var), std::move(s)));
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
        std::string param_id = std::get<std::string>(currentToken.value);
        parameters.push_back(VariableDeclr(param_id, tod));
        getNextToken();
    } while (currentToken.type == TokenType::Comma);

    return parameters;
}
std::unique_ptr<Declaration> Parser::TryToParseVariableDeclaration(){
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
std::unique_ptr<IExpression> Parser::TryToParseCondition() {
    std::string my_operator;
    auto left = TryToParseRelationalCondition();
    if (left == nullptr)
        return nullptr;
    while (currentToken.type == TokenType::And_operator|| currentToken.type == TokenType::Or_operator)
    {
        my_operator = getOperatorType();
        getNextToken();
        auto right = TryToParseRelationalCondition();
        if(right == nullptr)
            ErrorHandler::printParserError(currentToken, "no expression after "+my_operator);
        left = CreateCondition(my_operator,std::move(left),std::move(right));
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
std::unique_ptr<IExpression> Parser::TryToParseRelationalCondition(){
    std::string my_operator;
    auto left = TryToParseBasicCondition();
    if (left == nullptr)
        return nullptr;
    while (currentToken.type == TokenType::Equals || currentToken.type == TokenType::Not_equals || currentToken.type == TokenType::Lesser_than ||
            currentToken.type == TokenType::Lesser_or_equal_than || currentToken.type == TokenType::Greater_than ||currentToken.type == TokenType::Greater_or_equal_than )
    {
        my_operator = getOperatorType();
        getNextToken();
        auto right = TryToParseBasicCondition();
        if(right == nullptr)
            ErrorHandler::printParserError(currentToken, "no expression after "+my_operator);
        left = CreateRelationalCondition(my_operator,std::move(left),std::move(right));
    }
    return left;
}
std::unique_ptr<IExpression> Parser::TryToParseBasicExpression()
{
    std::unique_ptr<INode> basic;
    bool wasNegation = false;
    if (currentToken.type == TokenType::Negation){
        wasNegation = true;
        getNextToken();
    }
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
        return std::make_unique<BasicExpression>(BasicExpression(std::move(basic), wasMinus,wasNegation));
    }
    else if (currentToken.type == TokenType::StringValue){
        basic = std::make_unique<String>(std::get<std::string>(currentToken.value));
        getNextToken();
        return std::make_unique<BasicExpression>(BasicExpression(std::move(basic), wasMinus,wasNegation));
    }
    else if (currentToken.type == TokenType::DateValue){
        basic = std::make_unique<Date>(std::get<Date>(currentToken.value));
        getNextToken();
        return std::make_unique<BasicExpression>(BasicExpression(std::move(basic), wasMinus,wasNegation));
    }
    else if (currentToken.type == TokenType::TimeDiffValue){
        basic =std::make_unique<TimeDiff>(std::get<TimeDiff>(currentToken.value));
        getNextToken();
        return std::make_unique<BasicExpression>(BasicExpression(std::move(basic), wasMinus,wasNegation));
    }
    else if(currentToken.type == TokenType::Id){
        std::string id = std::get<std::string>(currentToken.value);
        basic = std::make_unique<VariableAccess>(VariableAccess(id));
        getNextToken();
        auto function = TryToParseFunctionCall(id);
        if(function)
            basic = std::move(function);
        return std::make_unique<BasicExpression>(BasicExpression(std::move(basic), wasMinus,wasNegation));
    }
    else if(currentToken.type == TokenType::Left_parentheses){
        auto expr = TryToParseParenthesisExpresion();
        expr->wasMinus = wasMinus;
        expr->wasNegation = wasNegation;
        return expr;
    }
    return nullptr;
}
std::unique_ptr<IExpression> Parser::TryToParseBasicCondition(){
    bool wasNegation = false;
    if (currentToken.type == TokenType::Negation){
        wasNegation = true;
        getNextToken();
    }
    bool wasMinus = false;
    if (currentToken.type == TokenType::Minus)
    {
        wasMinus = true;
        getNextToken();
    }
    if(currentToken.type == TokenType::Left_parentheses){
        auto condition = TryToParseParenthesisCondition();
        while(currentToken.type == TokenType::Minus || currentToken.type == TokenType::Plus || currentToken.type == TokenType::Multiply || currentToken.type == TokenType::Divide){
            std::string op =getOperatorType();
            getNextToken();
            auto right = TryToParseExpression();
            condition = CreateExpression(op,std::move(condition),std::move(right));
        }
        condition->wasNegation = wasNegation;
        condition->wasMinus = wasMinus;
        return condition;
    }
    else if(auto expression = TryToParseExpression()){
        expression->wasNegation = wasNegation;
        expression->wasMinus = wasMinus;
        return expression;
    }
    return nullptr;
}
std::unique_ptr<IExpression> Parser::TryToParseParenthesisCondition(){
    expect_and_accept(TokenType::Left_parentheses, "no left parenthesis");
    auto condition = TryToParseCondition();
    expect_and_accept(TokenType::Right_parentheses, "no right parenthesis");
    return std::make_unique<ParenthesisExpression>(ParenthesisExpression(std::move(condition)));
}
std::unique_ptr<IExpression> Parser::TryToParseParenthesisExpresion(){
    expect_and_accept(TokenType::Left_parentheses, "no left parenthesis");
    auto expression = TryToParseExpression();
    expect_and_accept(TokenType::Right_parentheses, "no right parenthesis");
    return std::make_unique<ParenthesisExpression>(ParenthesisExpression(std::move(expression)));
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
std::unique_ptr<RelationalCondition> Parser::CreateRelationalCondition(std::string my_operator, std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right){

    return std::make_unique<RelationalCondition>(RelationalCondition(my_operator, std::move(left),std::move(right)));
}
std::unique_ptr<Condition> Parser::CreateCondition(std::string my_operator, std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right) {

    return std::make_unique<Condition>(Condition(my_operator, std::move(left),std::move(right)));
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
    if(currentToken.type == TokenType::And_operator)
        return "and";
    if(currentToken.type == TokenType::Or_operator)
        return "or";
    if(currentToken.type == TokenType::Lesser_than)
        return "<";
    if(currentToken.type == TokenType::Lesser_or_equal_than)
        return "<=";
    if(currentToken.type == TokenType::Greater_than)
        return ">";
    if(currentToken.type == TokenType::Greater_or_equal_than)
        return ">=";
    if(currentToken.type == TokenType::Equals)
        return "==";
    if(currentToken.type == TokenType::Not_equals)
        return "!=";
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



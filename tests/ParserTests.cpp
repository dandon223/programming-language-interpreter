#include <iostream>
#include "boost/test/unit_test.hpp"
#include "../modules/lexer/Lexer.h"
#include "../modules/parser/Parser.h"
#include "../modules/parser/Parser.cpp"
#include "../modules/token/Token.h"
#include "../modules/errorHandler/ErrorHandler.h"
#include <memory>


BOOST_AUTO_TEST_SUITE(Parser_basic_tests)
    BOOST_AUTO_TEST_CASE( Global_variables1 )
    {
        std::string text = "int a";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            BOOST_CHECK(program->declarations.size() == 1);
            if (program->declarations.size() == 1) {
                BOOST_CHECK(program->declarations[0]->var.typeOfData == TypeOfData::Integer);
                BOOST_CHECK(program->declarations[0]->var.id == "a");
                BOOST_CHECK(program->declarations[0]->assignable == nullptr);
            }
        }
    }
    BOOST_AUTO_TEST_CASE( Global_variables2 )
    {
        std::string text = "int a = 10";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            BOOST_CHECK(program->declarations.size() == 1);
            if (program->declarations.size() == 1) {
                BOOST_CHECK(program->declarations[0]->var.typeOfData == TypeOfData::Integer);
                BOOST_CHECK(program->declarations[0]->var.id == "a");
                BOOST_CHECK(program->declarations[0]->assignable != nullptr);
                if(program->declarations[0]->assignable != nullptr){
                    BOOST_CHECK( dynamic_cast<BasicExpression*>(program->declarations[0]->assignable.get()));
                    if(auto expression = dynamic_cast<BasicExpression*>(program->declarations[0]->assignable.get())){
                        BOOST_CHECK(dynamic_cast<Int*>(expression->basic.get()));
                        if(auto integer =dynamic_cast<Int*>(expression->basic.get())){
                            BOOST_CHECK(integer->value == 10);
                        }
                    }
                }
            }
        }
    }
    BOOST_AUTO_TEST_CASE( priority_of_expressions1 )
    {
        std::string text = "int a = 10 * 1 + 5";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();

        auto variable_declaration = VariableDeclr(VariableDeclr());
        variable_declaration.id = 'a'; variable_declaration.typeOfData = TypeOfData::Integer;
        auto ten = std::make_unique<Int>(Int(10));
        auto one = std::make_unique<Int>(Int(1));
        auto five = std::make_unique<Int>(Int(5));
        auto basic1 = std::make_unique<BasicExpression>(BasicExpression(std::move(ten),false,false));
        auto basic2 = std::make_unique<BasicExpression>(BasicExpression(std::move(one),false,false));
        auto basic3 = std::make_unique<BasicExpression>(BasicExpression(std::move(five),false,false));
        auto advanced = std::make_unique<AdvExpression>(AdvExpression("*",std::move(basic1),std::move(basic2)));
        auto expression = std::make_unique<Expression>(Expression("+",std::move(advanced),std::move(basic3)));
        auto declaration = std::make_unique<Declaration>(Declaration(variable_declaration,std::move(expression)));
        std::vector<std::unique_ptr<Declaration>> declarations; declarations.push_back(std::move(declaration));
        std::vector<std::unique_ptr<Function>> functions;
        auto program_test = std::make_unique<Program>(Program(std::move(declarations),std::move(functions)));
        PrintVisitor printVisitor = PrintVisitor();
        PrintVisitor printVisitor1 =PrintVisitor();
        program_test->accept(printVisitor);
        program->accept(printVisitor1);
        BOOST_CHECK(printVisitor.debug == printVisitor1.debug);
    }
    BOOST_AUTO_TEST_CASE( function_basic_return )
    {
        std::string text = "fun int a():\n"
                           "\treturn";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();

        std::string name = "a";
        TypeOfData return_data = TypeOfData::Integer;
        std::vector<VariableDeclr> parameters;
        auto statement = std::make_unique<Return>(Return(nullptr));
        std::vector<std::shared_ptr<INode>> statements;
        statements.push_back(std::move(statement));
        auto function_body = std::make_unique<Body>(Body(std::move(statements)));


        auto functionDef = std::make_unique<Function>(Function(name,return_data,std::move(parameters),std::move(function_body)));
        std::vector<std::unique_ptr<Declaration>> declarations;
        std::vector<std::unique_ptr<Function>> functions;
        functions.push_back(std::move(functionDef));
        auto program_test = std::make_unique<Program>(Program(std::move(declarations),std::move(functions)));
        PrintVisitor printVisitor = PrintVisitor();
        PrintVisitor printVisitor1 =PrintVisitor();
        program_test->accept(printVisitor);
        program->accept(printVisitor1);
        BOOST_CHECK(printVisitor.debug == printVisitor1.debug);
    }
    BOOST_AUTO_TEST_CASE( hard_function )
    {
        std::string text = "fun int start(): \n"
                           "\tstring napis = \"A\" \n"
                           "\tif napis ==\"A\": \n"
                           "\t\tprint(\"Tak\") \n"
                           "\telse: \n"
                           "\t\tprint(napis) \n"
                           "\treturn 0 ";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();

        std::string name = "start";
        TypeOfData return_data = TypeOfData::Integer;
        std::vector<VariableDeclr> parameters;
        auto return_statement = std::make_unique<Int>(0);
        auto exp = std::make_unique<BasicExpression>(std::move(return_statement),false,false);
        auto statement = std::make_unique<Return>(Return(std::move(exp)));
        //  declaration
        auto variable_decl = VariableDeclr(VariableDeclr()); variable_decl.id = "napis";variable_decl.typeOfData = TypeOfData::Message;
        auto string = std::make_unique<String>(String("A"));
        auto basic_expr = std::make_unique<BasicExpression>(BasicExpression(std::move(string),false,false));
        auto declaration = std::make_unique<Declaration>(Declaration(variable_decl,std::move(basic_expr)));
        std::vector<std::shared_ptr<INode>> statements;
        // if
        auto variable_access = std::make_unique<VariableAccess>(VariableAccess("napis"));
        auto basic_expr1 = std::make_unique<BasicExpression>(BasicExpression(std::move(variable_access),false,false));

        auto string1 = std::make_unique<String>(String("A"));
        auto basic_expr2 = std::make_unique<BasicExpression>(BasicExpression(std::move(string1),false,false));

        auto relative_condition = std::make_unique<RelationalCondition>(RelationalCondition("==",std::move(basic_expr1),std::move(basic_expr2)));

        auto string2 = std::make_unique<String>(String("Tak"));
        auto basic_expr3 = std::make_unique<BasicExpression>(BasicExpression(std::move(string2),false,false));
        std::vector<std::shared_ptr<IExpression>> b; b.push_back(std::move(basic_expr3));
        auto fun_call = std::make_unique<FunCall>(FunCall("print",std::move(b)));
        std::vector<std::shared_ptr<INode>> if_body_statements;
        if_body_statements.push_back(std::move(fun_call));

        auto if_body = std::make_unique<Body>(Body(std::move(if_body_statements)));

        auto if_statement = std::make_unique<If>(If(std::move(relative_condition),std::move(if_body)));
        // else
        auto variable_access2 = std::make_unique<VariableAccess>(VariableAccess("napis"));
        auto basic_expr4 = std::make_unique<BasicExpression>(BasicExpression(std::move(variable_access2),false,false));
        std::vector<std::shared_ptr<IExpression>> c; c.push_back(std::move(basic_expr4));
        auto fun_call2 = std::make_unique<FunCall>(FunCall("print",std::move(c)));
        std::vector<std::shared_ptr<INode>> else_body_statements;
        else_body_statements.push_back(std::move(fun_call2));

        auto else_body = std::make_unique<Body>(Body(std::move(else_body_statements)));

        auto else_statement = std::make_unique<Else>(Else(std::move(else_body)));


        statements.push_back(std::move(declaration));
        statements.push_back(std::move(if_statement));
        statements.push_back(std::move(else_statement));
        statements.push_back(std::move(statement));
        auto function_body = std::make_unique<Body>(Body(std::move(statements)));

        auto functionDef = std::make_unique<Function>(Function(name,return_data,std::move(parameters),std::move(function_body)));
        std::vector<std::unique_ptr<Declaration>> declarations;
        std::vector<std::unique_ptr<Function>> functions;
        functions.push_back(std::move(functionDef));
        auto program_test = std::make_unique<Program>(Program(std::move(declarations),std::move(functions)));
        PrintVisitor printVisitor = PrintVisitor();
        PrintVisitor printVisitor1 =PrintVisitor();
        program_test->accept(printVisitor);
        program->accept(printVisitor1);
        BOOST_CHECK(printVisitor.debug == printVisitor1.debug);
    }
    BOOST_AUTO_TEST_CASE( parsing_arguments_empty )
    {
        std::string text = ")";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        auto arguments = parser.TryToParseArguments();
        BOOST_CHECK(arguments.empty());
    }
    BOOST_AUTO_TEST_CASE( parsing_arguments_one_argument )
    {
        std::string text = "1)";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        auto arguments = parser.TryToParseArguments();
        BOOST_CHECK(arguments.size()==1);
    }
    BOOST_AUTO_TEST_CASE( parsing_arguments_no_right_parenthesis )
    {
        std::string text = "1";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        try {
            auto args = parser.TryToParseArguments();
            BOOST_CHECK(false);
        }catch(ParserException e){
            BOOST_CHECK(e.error == "No ) after arguments" );
        }
    }
    BOOST_AUTO_TEST_CASE( parsing_fun_call_no_lef_parenthesis )
    {
        std::string text = ")";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        auto fun_call = parser.TryToParseFunctionCall("a");
        BOOST_CHECK(fun_call == nullptr);
    }
    BOOST_AUTO_TEST_CASE( parsing_fun_call )
    {
        std::string text = "(i,1)";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        auto fun_call = parser.TryToParseFunctionCall("a");
        BOOST_CHECK(fun_call != nullptr);
    }
    BOOST_AUTO_TEST_CASE( parsing_parenthesis_expresion )
    {
        std::string text = "()";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        auto par_expr = parser.TryToParseParenthesisExpresion();
        BOOST_CHECK(par_expr != nullptr);
    }
    BOOST_AUTO_TEST_CASE( parsing_parenthesis_condition )
    {
        std::string text = "()";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        auto par_con = parser.TryToParseParenthesisCondition();
        BOOST_CHECK(par_con != nullptr);
    }
    BOOST_AUTO_TEST_CASE( parsing_condition )
    {
        std::string text = "(1 + 2)";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        auto par_con = parser.TryToParseBasicCondition();
        PrintVisitor printVisitor = PrintVisitor();
        par_con->accept(printVisitor);
        std::string real = "Entered ParenthesisExpression[]\n"
                           "  Entered Expression[]\n"
                           "    Entered BasicExpression [Type = Int, Value = 1]\n"
                           "  Operator: +\n"
                           "    Entered BasicExpression [Type = Int, Value = 2]\n";
        BOOST_CHECK(real ==printVisitor.debug);
    }
    BOOST_AUTO_TEST_CASE( parsing_condition2 )
    {
        std::string text = "!(a > b) != 10";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        auto par_con = parser.TryToParseCondition();
        PrintVisitor printVisitor = PrintVisitor();
        par_con->accept(printVisitor);
        std::string real = "Entered RelationalCondition []\n"
                           "  Entered ParenthesisExpression[ Negation]\n"
                           "    Entered RelationalCondition []\n"
                           "      Entered BasicExpression [Type = VariableAccess, Value = a]\n"
                           "    Operator: >\n"
                           "      Entered BasicExpression [Type = VariableAccess, Value = b]\n"
                           "Operator: !=\n"
                           "  Entered BasicExpression [Type = Int, Value = 10]\n";
        BOOST_CHECK(real ==printVisitor.debug);
    }
    BOOST_AUTO_TEST_CASE( parsing_expression )
    {
        std::string text = "1 + 4 * -2";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        auto par_con = parser.TryToParseExpression();
        PrintVisitor printVisitor = PrintVisitor();
        par_con->accept(printVisitor);
        std::string real ="Entered Expression[]\n"
                          "  Entered BasicExpression [Type = Int, Value = 1]\n"
                          "Operator: +\n"
                          "  Entered AdvExpression []\n"
                          "    Entered BasicExpression [Type = Int, Value = 4]\n"
                          "  Operator: *\n"
                          "    Entered BasicExpression [Minus, Type = Int, Value = 2]\n";
        BOOST_CHECK(real ==printVisitor.debug);
    }
    BOOST_AUTO_TEST_CASE( parsing_expression_2 )
    {
        std::string text = "-(1 + (4 * -2))";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        auto par_con = parser.TryToParseExpression();
        PrintVisitor printVisitor = PrintVisitor();
        par_con->accept(printVisitor);
        std::string real ="Entered ParenthesisExpression[, Minus]\n"
                          "  Entered Expression[]\n"
                          "    Entered BasicExpression [Type = Int, Value = 1]\n"
                          "  Operator: +\n"
                          "    Entered ParenthesisExpression[]\n"
                          "      Entered AdvExpression []\n"
                          "        Entered BasicExpression [Type = Int, Value = 4]\n"
                          "      Operator: *\n"
                          "        Entered BasicExpression [Minus, Type = Int, Value = 2]\n";
        BOOST_CHECK(real ==printVisitor.debug);
    }
    BOOST_AUTO_TEST_CASE( parsing_parameters )
    {
        std::string text = "(int a , int c)";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        auto par_con = parser.TryToParseParameters();
        PrintVisitor printVisitor = PrintVisitor();
        BOOST_CHECK(par_con.size()== 2);
    }
    BOOST_AUTO_TEST_CASE( parsing_if )
    {
        std::string text = "if a -1 < b:\n"
                           "\treturn";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        lexer.getNextToken();
        Parser parser = Parser(lexer);
        auto par_con = parser.TryToParseIfStatement(0);
        PrintVisitor printVisitor = PrintVisitor();
        std::string real ="Entered If\n"
                          "  Entered RelationalCondition []\n"
                          "    Entered Expression[]\n"
                          "      Entered BasicExpression [Type = VariableAccess, Value = a]\n"
                          "    Operator: -\n"
                          "      Entered BasicExpression [Type = Int, Value = 1]\n"
                          "  Operator: <\n"
                          "    Entered BasicExpression [Type = VariableAccess, Value = b]\n"
                          "  ,Body [\n"
                          "    Entered Return\n";
        par_con->accept(printVisitor);
        BOOST_CHECK(printVisitor.debug == real);
    }

BOOST_AUTO_TEST_SUITE_END()

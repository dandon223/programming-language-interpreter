#include <iostream>
#include "boost/test/unit_test.hpp"
#include "../modules/lexer/Lexer.h"
#include "../modules/parser/Parser.h"
#include "../modules/parser/Parser.cpp"
#include "../modules/token/Token.h"
#include "../modules/errorHandler/ErrorHandler.h"

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
BOOST_AUTO_TEST_SUITE_END()

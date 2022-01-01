#include <iostream>
#include "boost/test/unit_test.hpp"
#include "../modules/lexer/Lexer.h"
#include "../modules/parser/Parser.h"
#include "../modules/interpreter/Interpreter.h"
#include "../modules/interpreter/Interpreter.cpp"

BOOST_AUTO_TEST_SUITE(Interpreter_basic_tests)

    BOOST_AUTO_TEST_CASE( Global_vairables_easy )
        {
                std::string text = "int a = 1\n"
                                   "fun int main():\n"
                                   "\treturn";
            std::istringstream handle(text);
            Lexer lexer = Lexer(handle);
            Parser parser = Parser(lexer);
            auto program = parser.TryToParseProgram();
            Interpreter interpreter = Interpreter();
            BOOST_CHECK(program != nullptr);
            if (program != nullptr) {
                program->accept(interpreter);
                BOOST_CHECK(interpreter.global_scope.exists("a"));
                if(interpreter.global_scope.exists("a")){
                    BOOST_CHECK(std::get<int>(interpreter.global_scope.getValue("a"))==1);
                }
            }
        }
    BOOST_AUTO_TEST_CASE( Global_vairables_add1 )
    {
        std::string text = "int a = -1+1\n"
                           "fun int main():\n"
                           "\treturn";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            program->accept(interpreter);
            BOOST_CHECK(interpreter.global_scope.exists("a"));
            if(interpreter.global_scope.exists("a")){
                BOOST_CHECK(std::get<int>(interpreter.global_scope.getValue("a"))==0);
            }
        }
    }
    BOOST_AUTO_TEST_CASE( Global_vairables_add2 )
    {
        std::string text = "int a = -(1+1)\n"
                           "fun int main():\n"
                           "\treturn";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            program->accept(interpreter);
            BOOST_CHECK(interpreter.global_scope.exists("a"));
            if(interpreter.global_scope.exists("a")){
                BOOST_CHECK(std::get<int>(interpreter.global_scope.getValue("a"))==-2);
            }
        }
    }
    BOOST_AUTO_TEST_CASE( Global_vairables_multiply )
    {
        std::string text = "int a = -2*1\n"
                           "fun int main():\n"
                           "\treturn";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            program->accept(interpreter);
            BOOST_CHECK(interpreter.global_scope.exists("a"));
            if(interpreter.global_scope.exists("a")){
                BOOST_CHECK(std::get<int>(interpreter.global_scope.getValue("a"))==-2);
            }
        }
    }
    BOOST_AUTO_TEST_CASE( Global_vairables_add_and_multiply )
    {
        std::string text = "int a = -(1+2)*3+1\n"
                           "fun int main():\n"
                           "\treturn";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            program->accept(interpreter);
            BOOST_CHECK(interpreter.global_scope.exists("a"));
            if(interpreter.global_scope.exists("a")){
                BOOST_CHECK(std::get<int>(interpreter.global_scope.getValue("a"))==-8);
            }
        }
    }
    BOOST_AUTO_TEST_CASE( Global_vairables_bad_adding1 )
    {
        std::string text = "bool a = False +1\n"
                           "fun int main():\n"
                           "\treturn";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            BOOST_CHECK_THROW(program->accept(interpreter),InterpreterException);
        }
    }

BOOST_AUTO_TEST_SUITE_END()

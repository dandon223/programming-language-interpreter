#include <iostream>
#include "boost/test/unit_test.hpp"
#include "../modules/lexer/Lexer.h"
#include "../modules/parser/Parser.h"
#include "../modules/interpreter/Interpreter.h"
#include "../modules/interpreter/Interpreter.cpp"

BOOST_AUTO_TEST_SUITE(Interpreter_tests)

    BOOST_AUTO_TEST_CASE( Global_variables_easy )
        {
                std::string text = "int a = 1\n"
                                   "fun int main():\n"
                                   "\treturn 1";
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
    BOOST_AUTO_TEST_CASE( Global_variables_add1 )
    {
        std::string text = "int a = -1+1\n"
                           "fun int main():\n"
                           "\treturn 1";
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
    BOOST_AUTO_TEST_CASE( Global_variables_add2 )
    {
        std::string text = "int a = -(1+1)\n"
                           "fun int main():\n"
                           "\treturn 1";
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
    BOOST_AUTO_TEST_CASE( Global_variables_multiply )
    {
        std::string text = "int a = -2*1\n"
                           "fun int main():\n"
                           "\treturn 1";
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
    BOOST_AUTO_TEST_CASE( Global_variables_add_and_multiply )
    {
        std::string text = "int a = -(1+2)*3+1\n"
                           "fun int main():\n"
                           "\treturn 1";
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
    BOOST_AUTO_TEST_CASE( Global_variables_bad_adding1 )
    {
        std::string text = "bool a = False +1\n"
                           "fun int main():\n"
                           "\treturn 1";
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
    BOOST_AUTO_TEST_CASE( returning_from_main )
    {
        std::string text =  "fun int main():\n"
                            "\treturn 1";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            program->accept(interpreter);
            BOOST_CHECK(!interpreter.results.empty());
            if(!interpreter.results.empty())
                BOOST_CHECK(std::get<int>(interpreter.results.back()) ==1);
        }
    }
    BOOST_AUTO_TEST_CASE( if_statement_easy )
    {
        std::string text =  "fun int main():\n"
                            "\tif 1 < 2:\n"
                            "\t\treturn 1";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            program->accept(interpreter);
            BOOST_CHECK(!interpreter.results.empty());
            if(!interpreter.results.empty())
                BOOST_CHECK(std::get<int>(interpreter.results.back()) ==1);
        }
    }
    BOOST_AUTO_TEST_CASE( if_statement_medium )
    {
        std::string text =  "fun int main():\n"
                            "\tif 1 < 2 and [1999:01:01] >= [1998:11:10]:\n"
                            "\t\treturn 1";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            program->accept(interpreter);
            BOOST_CHECK(!interpreter.results.empty());
            if(!interpreter.results.empty())
                BOOST_CHECK(std::get<int>(interpreter.results.back()) ==1);
        }
    }
    BOOST_AUTO_TEST_CASE( if_statement_hard )
    {
        std::string text =  "fun int main():\n"
                            "\tif !(!(1 < 2) and [1999:01:01] >= [1998:11:10]):\n"
                            "\t\treturn 1";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            program->accept(interpreter);
            BOOST_CHECK(!interpreter.results.empty());
            if(!interpreter.results.empty())
                BOOST_CHECK(std::get<int>(interpreter.results.back()) ==1);
        }
    }
    BOOST_AUTO_TEST_CASE( if_statement_error )
    {
        std::string text =  "fun int main():\n"
                            "\tif 1 > True:\n"
                            "\t\treturn 1";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr)
            BOOST_CHECK_THROW(program->accept(interpreter),InterpreterException);
    }
    BOOST_AUTO_TEST_CASE( returning_bad_type )
    {
        std::string text =  "fun int main():\n"
                            "\treturn [1999:01:01]";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr)
            BOOST_CHECK_THROW(program->accept(interpreter),InterpreterException);
    }
    BOOST_AUTO_TEST_CASE( no_variable_in_scope )
    {
        std::string text =  "fun int main():\n"
                            "\tif 1<2:\n"
                            "\t\tint a = 1\n"
                            "\treturn a";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr)
            BOOST_CHECK_THROW(program->accept(interpreter),InterpreterException);
    }
    BOOST_AUTO_TEST_CASE( if_else_statement_easy )
    {
        std::string text =  "fun int main():\n"
                            "\tif 1 > 2:\n"
                            "\t\treturn 1\n"
                            "\telse:\n"
                            "\t\treturn 2";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            program->accept(interpreter);
            BOOST_CHECK(!interpreter.results.empty());
            if(!interpreter.results.empty())
                BOOST_CHECK(std::get<int>(interpreter.results.back()) ==2);
        }
    }
    BOOST_AUTO_TEST_CASE( else_not_after_if )
    {
        std::string text =  "fun int main():\n"
                            "\tif 1 > 2:\n"
                            "\t\treturn 1\n"
                            "\tint a = 1\n"
                            "\telse:\n"
                            "\t\treturn 2";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr)
            BOOST_CHECK_THROW(program->accept(interpreter),InterpreterException);
    }
    BOOST_AUTO_TEST_CASE( while_statement_easy )
    {
        std::string text = "fun int main():\n"
                           "\tint a = 3\n"
                           "\tint b =0\n"
                           "\twhile a >1:\n"
                           "\t\ta = a -1\n"
                           "\t\tb =b +1\n"
                           "\treturn b";
        std::istringstream handle(text);
        Lexer lexer = Lexer(handle);
        Parser parser = Parser(lexer);
        auto program = parser.TryToParseProgram();
        Interpreter interpreter = Interpreter();
        BOOST_CHECK(program != nullptr);
        if (program != nullptr) {
            program->accept(interpreter);
            BOOST_CHECK(!interpreter.results.empty());
            if(!interpreter.results.empty())
                BOOST_CHECK(std::get<int>(interpreter.results.back()) ==2);
        }
    }
BOOST_AUTO_TEST_SUITE_END()

#define BOOST_TEST_MAIN
#define BOOST_TEST_STATIC_LINK

#include <iostream>
#include "boost/test/unit_test.hpp"
#include "../modules/lexer/Lexer.h"
#include "../modules/lexer/Lexer.cpp"
#include "../modules/token/Token.h"
#include "../modules/token/Token.cpp"
#include "../modules/structures/Date.h"
#include "../modules/structures/Date.cpp"

BOOST_AUTO_TEST_SUITE(Basic_tests_of_symbols)
BOOST_AUTO_TEST_CASE( AloneSymbols )
{
    std::string text = "( ) , : ! > < = + - / * and or != == >= <= ";
    std::istringstream  handle(text);
    Lexer lexer = Lexer(handle);
    std::vector<Token> tokens;
    while(!lexer.endOfFile())
        tokens.push_back(lexer.getNextToken());
    BOOST_CHECK(tokens[0].type == TokenType::Indentation);
    BOOST_CHECK(tokens[1].type == TokenType::Left_parentheses);
    BOOST_CHECK(tokens[2].type == TokenType::Right_parentheses);
    BOOST_CHECK(tokens[3].type == TokenType::Comma);
    BOOST_CHECK(tokens[4].type == TokenType::Colon);
    BOOST_CHECK(tokens[5].type == TokenType::Negation);
    BOOST_CHECK(tokens[6].type == TokenType::Greater_than);
    BOOST_CHECK(tokens[7].type == TokenType::Lesser_than);
    BOOST_CHECK(tokens[8].type == TokenType::Assignment);
    BOOST_CHECK(tokens[9].type == TokenType::Plus);
    BOOST_CHECK(tokens[10].type == TokenType::Minus);
    BOOST_CHECK(tokens[11].type == TokenType::Divide);
    BOOST_CHECK(tokens[12].type == TokenType::Multiply);
    BOOST_CHECK(tokens[13].type == TokenType::And_operator);
    BOOST_CHECK(tokens[14].type == TokenType::Or_operator);
    BOOST_CHECK(tokens[15].type == TokenType::Not_equals);
    BOOST_CHECK(tokens[16].type == TokenType::Equals);
    BOOST_CHECK(tokens[17].type == TokenType::Greater_or_equal_than);
    BOOST_CHECK(tokens[18].type == TokenType::Lesser_or_equal_than);
    BOOST_CHECK(tokens[19].type == TokenType::End_of_text);
    BOOST_CHECK(tokens.size() == 20);

}
BOOST_AUTO_TEST_CASE( ReservedWords ){
        std::string text = "fun int float string date timeDiff return if else while ";
        std::istringstream  handle(text);
        Lexer lexer = Lexer(handle);
        std::vector<Token> tokens;
        while(!lexer.endOfFile())
            tokens.push_back(lexer.getNextToken());
        BOOST_CHECK(tokens[0].type == TokenType::Indentation);
        BOOST_CHECK(tokens[1].type == TokenType::Function);
        BOOST_CHECK(tokens[2].type == TokenType::Int);
        BOOST_CHECK(tokens[3].type == TokenType::Float);
        BOOST_CHECK(tokens[4].type == TokenType::String);
        BOOST_CHECK(tokens[5].type == TokenType::Date);
        BOOST_CHECK(tokens[6].type == TokenType::Time_diff);
        BOOST_CHECK(tokens[7].type == TokenType::Return);
        BOOST_CHECK(tokens[8].type == TokenType::If);
        BOOST_CHECK(tokens[9].type == TokenType::Else);
        BOOST_CHECK(tokens[10].type == TokenType::While);
        BOOST_CHECK(tokens[11].type == TokenType::End_of_text);
        BOOST_CHECK(tokens.size() == 12);
}
BOOST_AUTO_TEST_CASE( NewLinesTabAndComments ){
        std::string text = "     //FAFGAGAD  \n"
                           " \n"
                           "";
        std::istringstream  handle(text);
        Lexer lexer = Lexer(handle);
        std::vector<Token> tokens;
        while(!lexer.endOfFile())
            tokens.push_back(lexer.getNextToken());
        BOOST_CHECK(tokens[0].type == TokenType::Invalid);
        BOOST_CHECK(std::get<std::string>(tokens[0].value) == "spacebar after indentation");
        BOOST_CHECK(tokens[1].type == TokenType::Invalid);
        BOOST_CHECK(tokens[2].type == TokenType::Indentation);
        BOOST_CHECK(tokens[3].type == TokenType::End_of_text);
        BOOST_CHECK(tokens.size() == 4);

}
BOOST_AUTO_TEST_CASE( DateAndTimeDiff ){
        std::string text = "date a [2000:10:09]\n"
                           "date b [1999:01:05]\n"
                           "if a-b > {10y1m0d}:\n"
                           "\tprint(\"Yes\")";
        std::istringstream  handle(text);
        Lexer lexer = Lexer(handle);
        std::vector<Token> tokens;
        while(!lexer.endOfFile())
            tokens.push_back(lexer.getNextToken());
        BOOST_CHECK(tokens[0].type == TokenType::Indentation);
        BOOST_CHECK(tokens[1].type == TokenType::Date);
        BOOST_CHECK(tokens[2].type == TokenType::Id);
        BOOST_CHECK(tokens[3].type == TokenType::DateValue);

        BOOST_CHECK(tokens[4].type == TokenType::Indentation);
        BOOST_CHECK(tokens[5].type == TokenType::Date);
        BOOST_CHECK(tokens[6].type == TokenType::Id);
        BOOST_CHECK(tokens[7].type == TokenType::DateValue);

        BOOST_CHECK(tokens[8].type == TokenType::Indentation);
        BOOST_CHECK(tokens[9].type == TokenType::If);
        BOOST_CHECK(tokens[10].type == TokenType::Id);
        BOOST_CHECK(tokens[11].type == TokenType::Minus);
        BOOST_CHECK(tokens[12].type == TokenType::Id);
        BOOST_CHECK(tokens[13].type == TokenType::Greater_than);
        BOOST_CHECK(tokens[14].type == TokenType::TimeDiffValue);
        BOOST_CHECK(tokens[15].type == TokenType::Colon);

        BOOST_CHECK(tokens[16].type == TokenType::Indentation);
        BOOST_CHECK(tokens[17].type == TokenType::Id);
        BOOST_CHECK(tokens[18].type == TokenType::Left_parentheses);
        BOOST_CHECK(tokens[19].type == TokenType::StringValue);
        BOOST_CHECK(tokens[20].type == TokenType::Right_parentheses);
        BOOST_CHECK(tokens[21].type == TokenType::End_of_text);

        BOOST_CHECK(tokens.size() == 22);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Examples)

BOOST_AUTO_TEST_CASE( Example1 ){
        std::string text = "fun int start():\n"
                           "\tstring text=\"A\"\n"
                           "\t\tif text == \"A\":\n"
                           "\t\t\tprint(\"Yes\")\n"
                           "\t\telse:\n"
                           "\t\t\tint zmienna = 1\n"
                           "\t\t\twhile(zmienna <5):\n"
                           "\t\t\t\tprint(i)\n"
                           "\t\t\t\tzmienna = zmienna + 1\n"
                           "\t\treturn 0";
        std::istringstream  handle(text);
        Lexer lexer = Lexer(handle);
        std::vector<Token> tokens;
        while(!lexer.endOfFile())
            tokens.push_back(lexer.getNextToken());
        BOOST_CHECK(tokens[0].type == TokenType::Indentation);
        BOOST_CHECK(tokens[1].type == TokenType::Function);
        BOOST_CHECK(tokens[2].type == TokenType::Int);
        BOOST_CHECK(tokens[3].type == TokenType::Id);
        BOOST_CHECK(tokens[4].type == TokenType::Left_parentheses);
        BOOST_CHECK(tokens[5].type == TokenType::Right_parentheses);
        BOOST_CHECK(tokens[6].type == TokenType::Colon);

        BOOST_CHECK(tokens[7].type == TokenType::Indentation);
        BOOST_CHECK(tokens[8].type == TokenType::String);
        BOOST_CHECK(tokens[9].type == TokenType::Id);
        BOOST_CHECK(tokens[10].type == TokenType::Assignment);
        BOOST_CHECK(tokens[11].type == TokenType::StringValue);

        BOOST_CHECK(tokens[12].type == TokenType::Indentation);
        BOOST_CHECK(tokens[13].type == TokenType::If);
        BOOST_CHECK(tokens[14].type == TokenType::Id);
        BOOST_CHECK(tokens[15].type == TokenType::Equals);
        BOOST_CHECK(tokens[16].type == TokenType::StringValue);
        BOOST_CHECK(tokens[17].type == TokenType::Colon);

        BOOST_CHECK(tokens[18].type == TokenType::Indentation);
        BOOST_CHECK(tokens[19].type == TokenType::Id);
        BOOST_CHECK(tokens[20].type == TokenType::Left_parentheses);
        BOOST_CHECK(tokens[21].type == TokenType::StringValue);
        BOOST_CHECK(tokens[22].type == TokenType::Right_parentheses);

        BOOST_CHECK(tokens[23].type == TokenType::Indentation);
        BOOST_CHECK(tokens[24].type == TokenType::Else);
        BOOST_CHECK(tokens[25].type == TokenType::Colon);

        BOOST_CHECK(tokens[26].type == TokenType::Indentation);
        BOOST_CHECK(tokens[27].type == TokenType::Int);
        BOOST_CHECK(tokens[28].type == TokenType::Id);
        BOOST_CHECK(tokens[29].type == TokenType::Assignment);
        BOOST_CHECK(tokens[30].type == TokenType::Number);

        BOOST_CHECK(tokens[31].type == TokenType::Indentation);
        BOOST_CHECK(tokens[32].type == TokenType::While);
        BOOST_CHECK(tokens[33].type == TokenType::Left_parentheses);
        BOOST_CHECK(tokens[34].type == TokenType::Id);
        BOOST_CHECK(tokens[35].type == TokenType::Lesser_than);
        BOOST_CHECK(tokens[36].type == TokenType::Number);
        BOOST_CHECK(tokens[37].type == TokenType::Right_parentheses);
        BOOST_CHECK(tokens[38].type == TokenType::Colon);

        BOOST_CHECK(tokens[39].type == TokenType::Indentation);
        BOOST_CHECK(tokens[40].type == TokenType::Id);
        BOOST_CHECK(tokens[41].type == TokenType::Left_parentheses);
        BOOST_CHECK(tokens[42].type == TokenType::Id);
        BOOST_CHECK(tokens[43].type == TokenType::Right_parentheses);

        BOOST_CHECK(tokens[44].type == TokenType::Indentation);
        BOOST_CHECK(tokens[45].type == TokenType::Id);
        BOOST_CHECK(tokens[46].type == TokenType::Assignment);
        BOOST_CHECK(tokens[47].type == TokenType::Id);
        BOOST_CHECK(tokens[48].type == TokenType::Plus);
        BOOST_CHECK(tokens[49].type == TokenType::Number);

        BOOST_CHECK(tokens[50].type == TokenType::Indentation);
        BOOST_CHECK(tokens[51].type == TokenType::Return);
        BOOST_CHECK(tokens[52].type == TokenType::Number);
        BOOST_CHECK(tokens[53].type == TokenType::End_of_text);

        BOOST_CHECK(tokens.size() == 54);
}
BOOST_AUTO_TEST_SUITE_END()
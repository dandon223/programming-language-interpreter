#define BOOST_TEST_MAIN
#define BOOST_TEST_STATIC_LINK

#include <iostream>
#include "boost/test/unit_test.hpp"
#include "../modules/lexer/Lexer.h"
#include "../modules/lexer/Lexer.cpp"
#include "../modules/token/Token.h"

BOOST_AUTO_TEST_SUITE(Basic_tests_of_symbols);
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
BOOST_AUTO_TEST_SUITE_END()
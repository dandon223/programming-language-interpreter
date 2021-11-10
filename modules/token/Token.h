

#ifndef TKOM_21Z_DANIEL_TOKEN_H
#define TKOM_21Z_DANIEL_TOKEN_H

#include <variant>
#include <string>
#include <unordered_map>

enum class TokenType
{
    End_of_text = 0,
    Left_parentheses = 1,
    Right_parentheses = 2,
    Comma = 3,
    Negation = 4,
    And_operator = 5,
    Or_operator = 6,
    Not_equals = 7,
    Equals = 8,
    Greater_than = 9,
    Lesser_than = 10,
    Greater_or_equal_than = 11,
    Lesser_or_equal_than = 12,
    Assignment = 13,
    Plus = 14,
    Minus = 15,
    Divide = 16,
    Multiply = 17,
    Function = 18,
    Int = 19,
    Float = 20,
    Return = 21,
    If = 22,
    While = 23,
    Colon = 24,
    String = 25,
    Date = 26,
    Time_diff = 27,
    Else = 28,
    Invalid = 29,
    Indentation = 30,
    Id = 31,
    Number = 32,

};
const std::unordered_map<int, std::string> print = {

        {0, "End_of_text"},
        {1, "Left_parentheses"},
        {2,"Right_parentheses" },
        {3,"Comma" },
        {4,"Negation" },
        {5,"And_operator" },
        {6, "Or_operator"},
        {7, "Not_equals"},
        {8, "Equals"},
        {9,"Greater_than" },
        {10,"Lesser_than"},
        {11,"Greater_or_equal_than"},
        {12,"Lesser_or_equal_than"},
        {13, "Assignment"},
        {14, "Plus"},
        {15, "Minus"},
        {16, "Divide"},
        {17, "Multiply"},
        {18,"Function" },
        {19, "Int"},
        {20,"Float" },
        {21, "Return"},
        {22, "If"},
        {23, "While"},
        {24, "Colon"},
        {25,"String" },
        {26, "Date"},
        {27, "Time_diff"},
        {28, "Else"},
        {29, "Invalid"},
        {30, "Indentation"},
        {31, "Id"},
        {32, "Number"},
};
const std::unordered_map<std::string, TokenType> map = {

        {"(", TokenType::Left_parentheses},
        {")", TokenType::Right_parentheses},
        {",", TokenType::Comma},
        {":", TokenType::Colon},
        {"!", TokenType::Negation},
        {"and", TokenType::And_operator},
        {"or", TokenType::Or_operator},
        {"!=", TokenType::Not_equals},
        {"==", TokenType::Equals},
        {">", TokenType::Greater_than},
        {"<", TokenType::Lesser_than},
        {">=", TokenType::Greater_or_equal_than},
        {"<=", TokenType::Lesser_or_equal_than},
        {"=", TokenType::Assignment},
        {"+", TokenType::Plus},
        {"-", TokenType::Minus},
        {"/", TokenType::Divide},
        {"*", TokenType::Multiply},
        {"fun", TokenType::Function},
        {"int", TokenType::Int},
        {"float", TokenType::Float},
        {"string", TokenType::String},
        {"date", TokenType::Date},
        {"timeDiff", TokenType::Time_diff},
        {"return", TokenType::Return},
        {"if", TokenType::If},
        {"else", TokenType::Else},
        {"while", TokenType::While},
};
struct Token
{
    TokenType type;
    std::variant<int, std::string,double> value;
    int line_number ;
    int column_number ;
};

struct TokenGet {
    std::string operator()(int value) { return std::to_string(value); }
    std::string operator()(std::string value) { return value;}
    std::string operator()(double value) { return std::to_string(value); }
};
std::string TokenValueToString(const std::variant<int, std::string,double>& input);
std::string getStringToken(struct Token token);

#endif //TKOM_21Z_DANIEL_TOKEN_H

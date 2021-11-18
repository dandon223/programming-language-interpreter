
#include "Token.h"
std::string TokenValueToString(const std::variant<int, std::string,double,Date,TimeDiff>& input) {
    return std::visit(TokenGet{}, input);
}
std::string getStringToken(struct Token token){
    std::string result;
    result += "[ Type: "+ print.at(static_cast<int>(token.type)) +", Value: "+ TokenValueToString(token.value)+", Line: "
              +std::to_string(token.line_number) +", Column: " + std::to_string(token.column_number)+" Length: "+std::to_string(token.length)+"]";
    return result;
}


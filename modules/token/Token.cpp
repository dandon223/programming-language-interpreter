
#include "Token.h"
std::string TokenValueToString(const std::variant<int, std::string,double,Date,TimeDiff>& input) {
    return std::visit(TokenGet{}, input);
}


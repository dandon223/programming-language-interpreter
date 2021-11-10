
#include "Date.h"

Date::Date(int y, int m, int d) :year(y),month(m),day(d){}

std::string Date::toString() {
    return std::to_string(year)+":"+std::to_string(month)+":"+std::to_string(day);
}

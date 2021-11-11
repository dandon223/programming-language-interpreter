
#include "Date.h"

Date::Date(int y, int m, int d) :year(y),month(m),day(d){}

std::string Date::toString() {
    std::string year_date = std::to_string(year);
    std::string month_date = std::to_string(month);
    std::string day_date = std::to_string(day);
    if(year <10)
        year_date = "000" +year_date;
    else if(year <100)
        year_date = "00" +year_date;
    else if(year <1000)
        year_date = "0" +year_date;

    if(month < 10)
        month_date = "0" + month_date;

    if(day <10)
        day_date = "0" + day_date;
    return year_date+":"+month_date+":"+day_date;
}


TimeDiff::TimeDiff(int y, int m, int d) :year(y),month(m),day(d){}

std::string TimeDiff::toString() {
    return std::to_string(year)+"y"+std::to_string(month)+"m"+std::to_string(day)+"d";
}

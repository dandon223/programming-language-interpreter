

#ifndef TKOM_21Z_DANIEL_DATE_H
#define TKOM_21Z_DANIEL_DATE_H
#include <string>

class Date {
private:
    int year = 1970;
    int month = 1;
    int day = 1;
public:
    Date(int y, int m, int d);
    std::string toString();

};


#endif //TKOM_21Z_DANIEL_DATE_H

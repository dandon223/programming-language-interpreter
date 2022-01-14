

#include "Interpreter.h"
bool Interpreter::properType(TypeOfData type,variantTypes value){
    if(value.index() == 0)
        return true;
    if(type == TypeOfData::Integer && value.index()==1)
        return true;
    if(type == TypeOfData::Double && value.index()==2)
        return true;
    if(type == TypeOfData::Double && value.index()==1)
        return true;
    if(type == TypeOfData::Bool && value.index()==3)
        return true;
    if(type == TypeOfData::Message && value.index()==4)
        return true;
    if(type == TypeOfData::Date && value.index()==5)
        return true;
    if(type == TypeOfData::TimeDiff && value.index()==6)
        return true;
    return false;
}
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;
void Interpreter::TryToMultiply() {
    auto right_result = results.back();
    results.pop_back();
    auto left_result = results.back();
    std::visit(overload{
            [&](int&left, int &right) { results.back() = left * right; },
            [&](double &left, double&right ) { results.back() = left * right; },
            [&](int &left, double&right ) { results.back() = double(left) * right; },
            [&](double &left, int&right ) { results.back() = left * double(right); },
            [](auto&, auto& ) { ErrorHandler::printInterpreterError("cannot multiply those 2 arguments"); },
    }, left_result, right_result);
}
void Interpreter::TryToDivide() {
    auto right_result = results.back();
    results.pop_back();
    auto left_result = results.back();

    if(right_result.index()==1 && std::get<int>(right_result)==0)
        ErrorHandler::printInterpreterError("can not divide by 0");
    else if(right_result.index()==2 && std::get<double>(right_result)== 0)
        ErrorHandler::printInterpreterError("can not divide by 0");

    std::visit(overload{
            [&](int&left, int &right) { results.back() = left / right; },
            [&](double &left, double&right ) { results.back() = left / right; },
            [&](int &left, double&right ) { results.back() = double(left) / right; },
            [&](double &left, int&right ) { results.back() = left / double(right); },
            [](auto&, auto& ) { ErrorHandler::printInterpreterError("cannot divide those 2 arguments"); },
    }, left_result, right_result);
}
void Interpreter::TryToAdd() {
    auto right_result = results.back();
    results.pop_back();
    auto left_result = results.back();

    std::visit(overload{
            [&](int&left, int &right) { results.back() = left + right; },
            [&](double &left, double&right ) { results.back() = left + right; },
            [&](int &left, double&right ) { results.back() = double(left) + right; },
            [&](double &left, int&right ) { results.back() = left + double(right); },
            [&](Date &left, TimeDiff&right ) { results.back() = left + right; checkDate();},
            [&](TimeDiff &left, Date&right ) { results.back() = right + left; checkDate();},
            [&](std::string &left, std::string&right ) { results.back() = left + right; },
            [&](std::string &left, int&right ) { results.back() = left + std::to_string(right); },
            [&](std::string &left, double&right ) { results.back() = left + std::to_string(right); },
            [&](std::string &left, Date&right ) { results.back() = left + right.toString(); },
            [&](std::string &left, TimeDiff&right ) { results.back() = left + right.toString(); },
            [](auto &left, auto &right) { ErrorHandler::printInterpreterError("cannot add those 2 arguments"); },
    }, left_result, right_result);
}

void Interpreter::TryToSubstract() {
    auto right_result = results.back();
    results.pop_back();
    auto left_result = results.back();

    std::visit(overload{
            [&](int&left, int &right) { results.back() = left - right; },
            [&](double &left, double&right ) { results.back() = left - right; },
            [&](int &left, double&right ) { results.back() = double(left) - right; },
            [&](double &left, int&right ) { results.back() = left - double(right); },
            [&](Date &left, Date &right ) { results.back() = right - left; },
            [&](Date &left, TimeDiff &right ) { results.back() = left - right; checkDate();},
            [](auto&, auto& ) { ErrorHandler::printInterpreterError("cannot substract those 2 arguments"); },
    }, left_result, right_result);
}

void Interpreter::TryToCompareValues(std::string type){
    auto right_result = results.back();
    results.pop_back();
    auto left_result = results.back();

    if(type == "=="){
        std::visit(overload{
                [&](int&left, int &right) { results.back() = left == right; },
                [&](double &left, double&right ) { results.back() = left == right; },
                [&](int &left, double&right ) { results.back() = double(left) == right; },
                [&](double &left, int&right ) { results.back() = left == double(right); },
                [&](bool &left, bool&right ) { results.back() = left == right; },
                [&](TimeDiff &left, TimeDiff &right ) { results.back() = left.operator==(right); },
                [&](Date &left, Date &right ) { results.back() = left.operator==(right); },
                [](auto&, auto& ) { ErrorHandler::printInterpreterError("cannot ''=='' those 2 arguments"); },
        }, left_result, right_result);
    }
    else if(type == "!="){
        std::visit(overload{
                [&](int&left, int &right) { results.back() = left != right; },
                [&](double &left, double&right ) { results.back() = left != right; },
                [&](int &left, double&right ) { results.back() = double(left) != right; },
                [&](double &left, int&right ) { results.back() = left != double(right); },
                [&](bool &left, bool&right ) { results.back() = left != right; },
                [&](TimeDiff &left, TimeDiff &right ) { results.back() = left.operator!=(right); },
                [&](Date &left, Date &right ) { results.back() = left.operator==(right); },
                [](auto&, auto& ) { ErrorHandler::printInterpreterError("cannot ''!='' those 2 arguments"); },
        }, left_result, right_result);
    }
    else if(type == "<="){
        std::visit(overload{
                [&](int&left, int &right) { results.back() = left <= right; },
                [&](double &left, double&right ) { results.back() = left <= right; },
                [&](int &left, double&right ) { results.back() = double(left) <= right; },
                [&](double &left, int&right ) { results.back() = left <= double(right); },
                [&](TimeDiff &left, TimeDiff &right ) { results.back() = left.operator<=(right); },
                [&](Date &left, Date &right ) { results.back() = left.operator<=(right); },
                [](auto&, auto& ) { ErrorHandler::printInterpreterError("cannot ''!='' those 2 arguments"); },
        }, left_result, right_result);
    }
    else if(type == "<"){
        std::visit(overload{
                [&](int&left, int &right) { results.back() = left < right; },
                [&](double &left, double&right ) { results.back() = left < right; },
                [&](int &left, double&right ) { results.back() = double(left) < right; },
                [&](double &left, int&right ) { results.back() = left < double(right); },
                [&](TimeDiff &left, TimeDiff &right ) { results.back() = left.operator<(right); },
                [&](Date &left, Date &right ) { results.back() = left.operator<(right); },
                [](auto&, auto& ) { ErrorHandler::printInterpreterError("cannot ''!='' those 2 arguments"); },
        }, left_result, right_result);
    }
    else if(type == ">="){
        std::visit(overload{
                [&](int&left, int &right) { results.back() = left >= right; },
                [&](double &left, double&right ) { results.back() = left >= right; },
                [&](int &left, double&right ) { results.back() = double(left) >= right; },
                [&](double &left, int&right ) { results.back() = left >= double(right); },
                [&](TimeDiff &left, TimeDiff &right ) { results.back() = left.operator>=(right); },
                [&](Date &left, Date &right ) { results.back() = left.operator>=(right); },
                [](auto&, auto& ) { ErrorHandler::printInterpreterError("cannot ''!='' those 2 arguments"); },
        }, left_result, right_result);
    }
    else if(type == ">"){
        std::visit(overload{
                [&](int&left, int &right) { results.back() = left > right; },
                [&](double &left, double&right ) { results.back() = left > right; },
                [&](int &left, double&right ) { results.back() = double(left) > right; },
                [&](double &left, int&right ) { results.back() = left > double(right); },
                [&](TimeDiff &left, TimeDiff &right ) { results.back() = left.operator>(right); },
                [&](Date &left, Date &right ) { results.back() = left.operator>(right); },
                [](auto&, auto& ) { ErrorHandler::printInterpreterError("cannot ''!='' those 2 arguments"); },
        }, left_result, right_result);
    }
}
void Interpreter::TryToAndOr(std::string type){
    auto right_result = results.back();
    results.pop_back();
    auto left_result = results.back();

    if(type == "and"){
        std::visit(overload{
                [&](bool &left, bool&right ) { results.back() = left && right; },
                [](auto&, auto& ) { ErrorHandler::printInterpreterError("cannot ''and'' those 2 arguments"); },
        }, left_result, right_result);
    }
    else if(type == "or"){
        std::visit(overload{
                [&](bool &left, bool&right ) { results.back() = left || right; },
                [](auto&, auto& ) { ErrorHandler::printInterpreterError("cannot ''and'' those 2 arguments"); },
        }, left_result, right_result);
    }
}
std::string Interpreter::toString(variantTypes value){
    if(value.index() == 0)
        return "";
    if(value.index()==1)
        return std::to_string(std::get<int>(value));
    if(value.index()==2)
        return std::to_string(std::get<double>(value));
    if(value.index()==3){
        bool b = std::get<bool>(value);
        if(b)
            return "true";
        else
            return "false";
    }
    if(value.index()==4)
        return std::get<std::string>(value);
    if( value.index()==5)
        return std::get<Date>(value).toString();
    if(value.index()==6)
        return std::get<TimeDiff>(value).toString();
    return "";
}
void Interpreter::addPrintFunction(){
    std::string name = "print";
    TypeOfData typeOfData = TypeOfData::Message;
    std::vector<VariableDeclr> parameters;
    VariableDeclr parameter;
    parameter.id = "to_print";
    parameter.typeOfData = TypeOfData::Message;
    parameters.push_back(parameter);
    auto function = std::make_unique<Function>(Function(name,typeOfData,parameters, nullptr,0,0));
    functions.insert({name,std::move(function)});
}
void Interpreter::checkDate(){
    Date date = std::get<Date>(results.back());
    if(date.year <0 || date.month<0 || date.day<0)
        ErrorHandler::printInterpreterError("date can not have minus values");
    if(date.month>12)
        ErrorHandler::printInterpreterError("new date has more than 12 months");
    if (date.month == 3)
    {
        if ((date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0)){
            if(date.day>29)
                ErrorHandler::printInterpreterError("to many days in date");
        }
        else if(date.day>28)
            ErrorHandler::printInterpreterError("to many days in date");
    }
    else if (date.month == 5 || date.month == 7 || date.month == 10 || date.month == 12){
        if(date.day>30)
            ErrorHandler::printInterpreterError("to many days in date");
    }
    else
    if(date.day>31)
        ErrorHandler::printInterpreterError("to many days in date");
}


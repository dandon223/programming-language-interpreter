

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
            [](auto&, auto& ) { ErrorHandler::printInterpreterError("cannot add those 2 arguments"); },
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
            [&](Date &left, Date &right ) { results.back() = left - right; },
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


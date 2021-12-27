

#include "Interpreter.h"

bool Interpreter::properType(TypeOfData type,std::variant<std::monostate,int> value){
    if(value.index() == 0)
        return true;
    if(type == TypeOfData::Integer && value.index()==1)
        return true;
    return false;
}

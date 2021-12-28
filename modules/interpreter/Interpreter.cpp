

#include "Interpreter.h"

bool Interpreter::properType(TypeOfData type,variantTypes value){
    if(value.index() == 0)
        return true;
    if(type == TypeOfData::Integer && value.index()==1)
        return true;
    if(type == TypeOfData::Double && value.index()==2)
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

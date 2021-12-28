
#ifndef TKOM_21Z_DANIEL_FUNCTIONCALLCONTEXT_H
#define TKOM_21Z_DANIEL_FUNCTIONCALLCONTEXT_H
#include "Scope.h"
class FunctionCallContext{
private:
    std::vector<Scope> funConVec;
public:
    bool addToScope(std::string name, TypeOfData token_type, variantTypes value){
        if(funConVec.empty())
            return false;
        auto last_scope = funConVec.back();
        if(!last_scope.exists(name)){
            last_scope.addToScope(name,token_type,value);
            return true;
        }
        return false;
    }
    void addScope(){
        funConVec.emplace_back();
    }
    void popScope(){
        funConVec.pop_back();
    }
    bool existsInLastScope(std::string name){
        if(funConVec.empty())
            return false;
        auto last_scope = funConVec.back();
        if(last_scope.exists(name))
            return true;
        return false;
    }
};
#endif //TKOM_21Z_DANIEL_FUNCTIONCALLCONTEXT_H

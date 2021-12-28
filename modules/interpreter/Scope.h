

#ifndef TKOM_21Z_DANIEL_SCOPE_H
#define TKOM_21Z_DANIEL_SCOPE_H

using variantTypes = std::variant<std::monostate,int,double,bool,std::string,Date,TimeDiff>;

class Scope{
private:
    std::unordered_map<std::string, TypeOfData> types;
    std::unordered_map<std::string, variantTypes> values;
public:
    bool addToScope(std::string name, TypeOfData token_type, variantTypes value){
        auto search = types.find(name);
        if(search != types.end())
            return false;
        types.insert({name,token_type});
        values.insert({name,value});
        return true;
    }
    TypeOfData getTokenType(std::string name){
        auto search = types.find(name);
        return search->second;
    }
    variantTypes getValue(std::string name){
        auto search = values.find(name);
        return search->second;
    }
    bool exists(std::string name){
        auto search = types.find(name);
        if(search == types.end())
            return false;
        return true;
    }
};
#endif //TKOM_21Z_DANIEL_SCOPE_H

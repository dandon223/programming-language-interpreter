

#ifndef TKOM_21Z_DANIEL_INTERPRETER_H
#define TKOM_21Z_DANIEL_INTERPRETER_H

#include "../errorHandler/ErrorHandler.h"
#include "../parser/Parser.h"
#include "../parser/Program.h"
#include "FunctionCallContext.h"




class Interpreter : public Visitor{
private:
    std::variant<std::monostate,int> result;
    int times_minus = 0;
    //int times_negation = 0;
    std::unordered_map<std::string, std::variant<std::monostate,int>> global_scope;
    std::vector<FunctionCallContext> functions_scopes;
    std::unordered_map<std::string, Function> functions;
    bool properType(TypeOfData type ,std::variant<std::monostate,int> value);
public:
    std::string debug = "";
    void visit(Declaration &element) override{
        if(element.assignable != nullptr)
            element.assignable.operator*().accept(*this);
        element.var.accept(*this);
    };
    void visit(VariableDeclr &element) override
    {
        if(properType(element.typeOfData,result)){
            global_scope.insert({element.id,result});
            result = std::monostate{};
        }else
            ErrorHandler::printInterpreterError("wrong type in variable declaration");

    };
    void visit(Program &element) override
    {
        debug += "Entering interpreter\n";
        for(auto &declaration : element.declarations){
            if(declaration != nullptr)
                visit(declaration.operator*());
        }
        debug += "----------------------------------------\n";
        for(auto &function : element.functions){
            if(function != nullptr)
                visit(function.operator*());
        }
    };
    void visit(AdvExpression &element) override{
        // TODO
    };
    void visit(Expression &element) override{
        // TODO
    };
    void visit(ParenthesisExpression &element) override{
    };
    void visit(BasicExpression &element) override{
        if(element.wasNegation)
            ErrorHandler::printInterpreterError("Negation used in basicExpression");
        if(element.wasMinus)
            times_minus = times_minus -1;

        if(element.basic != nullptr)
            element.basic->accept(*this);

        if(element.wasMinus)
            times_minus = times_minus +1;
    };
    void visit(Variable &element) override{
    };
    void visit(VariableAccess &element) override{
    };
    void visit(FunCall &element) override{
    };
    void visit(Function &element) override{
    };
    void visit(Body &element) override{
    };
    void visit(AssignStatement &element) override{
    }
    void visit(Int &element) override{
        int is_minus = times_minus % 2 ==0 ? 1 : -1;
        //std::cout << is_minus;
        result = element.value * is_minus;
    };
    void visit(Double &element) override{
    };
    void visit(Bool &element) override{
    };
    void visit(String &element) override{
    };
    void visit(Date &element) override{
    };
    void visit(TimeDiff &element) override{
    };
    void visit(Return &element) override{
    }
    void visit(While &element) override{
    };
    void visit(If &element) override{
    };
    void visit(Else &element) override{
    };
    void visit(Condition &element) override{
    }
    void visit(RelationalCondition &element) override{
    }
};


#endif //TKOM_21Z_DANIEL_INTERPRETER_H

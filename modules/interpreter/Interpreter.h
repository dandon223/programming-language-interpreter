

#ifndef TKOM_21Z_DANIEL_INTERPRETER_H
#define TKOM_21Z_DANIEL_INTERPRETER_H

#include "../errorHandler/ErrorHandler.h"
#include "../parser/Parser.h"
#include "../parser/Program.h"
#include "FunctionCallContext.h"




class Interpreter : public Visitor{
private:
    INode result;
    std::unordered_map<std::string, INode> global_scope;
    std::vector<FunctionCallContext> functions_scopes;
    std::unordered_map<std::string, Function> functions;
public:
    std::string debug;
    void visit(Declaration &element) override{
        element.assignable.operator*().accept(*this);
    };
    void visit(VariableDeclr &element) override
    {
        // TODO
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
        // TODO
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

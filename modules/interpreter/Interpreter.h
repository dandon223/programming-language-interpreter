

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
    void visit(Declaration &element,int indentation) override{
        element.assignable.operator*().accept(*this,indentation);
    };
    void visit(VariableDeclr &element,int indentation) override
    {
        // TODO
    };
    void visit(Program &element,int indentation) override
    {
        debug += "Entering interpreter\n";
        for(auto &declaration : element.declarations){
            if(declaration != nullptr)
                visit(declaration.operator*(),indentation);
        }
        debug += "----------------------------------------\n";
        for(auto &function : element.functions){
            if(function != nullptr)
                visit(function.operator*(),indentation);
        }
    };
    void visit(AdvExpression &element,int indentation) override{
        // TODO
    };
    void visit(Expression &element,int indentation) override{
        // TODO
    };
    void visit(ParenthesisExpression &element,int indentation) override{
    };
    void visit(BasicExpression &element ,int indentation) override{
        // TODO
    };
    void visit(Variable &element,int indentation) override{
    };
    void visit(VariableAccess &element,int indentation) override{
    };
    void visit(FunCall &element,int indentation) override{
    };
    void visit(Function &element,int indentation) override{
    };
    void visit(Body &element,int indentation) override{
    };
    void visit(AssignStatement &element,int indentation) override{
    }
    void visit(Int &element,int indentation) override{
    };
    void visit(Double &element,int indentation) override{
    };
    void visit(Bool &element,int indentation) override{
    };
    void visit(String &element,int indentation) override{
    };
    void visit(Date &element,int indentation) override{
    };
    void visit(TimeDiff &element,int indentation) override{
    };
    void visit(Return &element,int indentation) override{
    }
    void visit(While &element,int indentation) override{
    };
    void visit(If &element,int indentation) override{
    };
    void visit(Else &element,int indentation) override{
    };
    void visit(Condition &element,int indentation) override{
    }
    void visit(RelationalCondition &element,int indentation) override{
    }
};


#endif //TKOM_21Z_DANIEL_INTERPRETER_H

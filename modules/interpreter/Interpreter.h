

#ifndef TKOM_21Z_DANIEL_INTERPRETER_H
#define TKOM_21Z_DANIEL_INTERPRETER_H

#include "../errorHandler/ErrorHandler.h"
#include "../parser/Parser.h"
#include "../parser/Program.h"
#include "FunctionCallContext.h"
#include <variant>


class Interpreter : public Visitor{
public:
    std::vector<variantTypes> results;
    int times_minus = 0;
    int times_negation = 0;
    bool to_continue = true;
    bool last_was_if = false;
    bool last_if_was_true = false;
    Scope global_scope;
    std::vector<FunctionCallContext> functions_scopes;
    std::unordered_map<std::string, std::unique_ptr<Function>> functions;
    void TryToMultiply();
    void TryToDivide();
    void TryToAdd();
    void TryToSubstract();
    bool properType(TypeOfData type ,variantTypes value);
    void TryToCompareValues(std::string a);
    void TryToAndOr(std::string a);
//public:
    std::string debug = "";
    void visit(Declaration &element) override{
        last_was_if = false;
        if(element.assignable != nullptr)
            element.assignable.operator*().accept(*this);
        else
            results.emplace_back(std::monostate{});
        element.var.accept(*this);
    };
    void visit(VariableDeclr &element) override
    {
        if(properType(element.typeOfData,results.back())){

            if(!functions_scopes.empty() && !functions_scopes.back().existsInLastScope(element.id))
                functions_scopes.back().addToScope(element.id,element.typeOfData,results.back());
            else if(!global_scope.exists(element.id))
                global_scope.addToScope(element.id,element.typeOfData,results.back());
            else
                ErrorHandler::printInterpreterError("variable already declared");

            results.pop_back();
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
            if(function != nullptr){
                if(functions.find(function->name) == functions.end())
                    functions.insert({function->name,std::move(function)});
                else
                    ErrorHandler::printInterpreterError("function already defined");
            }
        }
        functions_scopes.emplace_back(FunctionCallContext());
        functions_scopes.back().addScope();
        auto main = functions.find("main");
        if(main != functions.end())
            main->second->accept(*this);
        else
            ErrorHandler::printInterpreterError("did not find main function");
    };
    void visit(AdvExpression &element) override{
        element.left.operator*().accept(*this);
        element.right.operator*().accept(*this);
        if(element.operationType == "*")
            TryToMultiply();
        else
            TryToDivide();
    };
    void visit(Expression &element) override{
        element.left.operator*().accept(*this);
        element.right.operator*().accept(*this);
        if(element.operationType == "+")
            TryToAdd();
        else
            TryToSubstract();
    };
    void visit(ParenthesisExpression &element) override{
        if(element.wasMinus)
            times_minus = times_minus -1;
        if(element.wasNegation)
            times_negation = times_negation - 1;

        element.child.operator*().accept(*this);
        if(element.wasNegation && results.back().index() !=3)
            ErrorHandler::printInterpreterError("negation not for bool type");
        if(element.wasNegation && results.back().index() ==3)
            results.back() = !std::get<bool>(results.back());

        if(element.wasMinus)
            times_minus = times_minus +1;
        if(element.wasNegation)
            times_negation = times_negation +1;
    };
    void visit(BasicExpression &element) override{
        if(element.wasNegation)
            times_negation = times_negation -1;
        if(element.wasMinus)
            times_minus = times_minus -1;

        if(element.basic != nullptr)
            element.basic->accept(*this);

        if(element.wasNegation)
            times_negation = times_negation +1;
        if(element.wasMinus)
            times_minus = times_minus +1;
    };
    void visit(Variable &element) override{
    };
    void visit(VariableAccess &element) override{
        if(functions_scopes.back().existsInScope(element.id))
            results.emplace_back(functions_scopes.back().getValue(element.id));
        else
            ErrorHandler::printInterpreterError("no variable in scope "+element.id);
    };
    void visit(FunCall &element) override{
        last_was_if = false;
    };
    void visit(Function &element) override{

        for(long long unsigned int i =0 ; i < element.parameters.size();i++){
            results.emplace_back(std::monostate{});
            element.parameters[i].accept(*this);
        }
        element.body.operator*().accept(*this);
        if(results.empty())
            ErrorHandler::printInterpreterError("function did not return anything");
        if(!properType(element.dataType,results.back()))
            ErrorHandler::printInterpreterError("wrong return type in function "+element.name);

    };
    void visit(Body &element) override{
        for(const auto& statement : element.statements){
            statement->accept(*this);
            if(!to_continue){
                break;
            }
        }
    };
    void visit(AssignStatement &element) override{
        last_was_if = false;
        element.assignable.operator*().accept(*this);

        if(functions_scopes.back().existsInScope(element.var->id)){
            functions_scopes.back().changeInScope(element.var->id,results.back());
            results.pop_back();
        }
        else
            ErrorHandler::printInterpreterError("no variable in scope "+element.var->id);
    }
    void visit(Int &element) override{
        int is_minus = times_minus % 2 ==0 ? 1 : -1;
        results.emplace_back(element.value * is_minus);
    };
    void visit(Double &element) override{
        int is_minus = times_minus % 2 ==0 ? 1 : -1;
        results.emplace_back(element.value * is_minus);
    };
    void visit(Bool &element) override{
        int is_minus = times_minus % 2 ==0 ? 1 : -1;
        if(is_minus == -1)
            ErrorHandler::printInterpreterError("Bool can not have minus sign");
        int is_negation = times_negation % 2 ==0 ? 1:-1;
        if(is_negation == -1)
            results.emplace_back(!element.value);
        else
            results.emplace_back(element.value);
    };
    void visit(String &element) override{
        int is_minus = times_minus % 2 ==0 ? 1 : -1;
        if(is_minus == -1)
            ErrorHandler::printInterpreterError("String can not have minus sign");
        results.emplace_back(element.value);
    };
    void visit(Date &element) override{
        int is_minus = times_minus % 2 ==0 ? 1 : -1;
        if(is_minus == -1)
            ErrorHandler::printInterpreterError("Date can not have minus sign");
        results.emplace_back(element);
    };
    void visit(TimeDiff &element) override{
        int is_minus = times_minus % 2 ==0 ? 1 : -1;
        if(is_minus == -1)
            ErrorHandler::printInterpreterError("TimeDiff can not have minus sign");
        results.emplace_back(element);
    };
    void visit(Return &element) override{
        last_was_if = false;
        if(element.expression != nullptr)
            element.expression.operator*().accept(*this);

        to_continue = false;
    }
    void visit(While &element) override{
        last_was_if = false;
        functions_scopes.back().addScope();
        element.condition.operator*().accept(*this);
        while(std::get<bool>(results.back())){
            results.pop_back();
            element.body.operator*().accept(*this);
            element.condition.operator*().accept(*this);
        }
        results.pop_back();
        functions_scopes.back().popScope();
    };
    void visit(If &element) override{

        element.condition.operator*().accept(*this);
        if(std::get<bool>(results.back())){
            results.pop_back();
            functions_scopes.back().addScope();
            element.body.operator*().accept(*this);
            functions_scopes.back().popScope();
            last_if_was_true = true;
        }else{
            results.pop_back();
            last_if_was_true = false;
        }
        last_was_if = true;
    };
    void visit(Else &element) override{
        if(!last_was_if)
            ErrorHandler::printInterpreterError("no if before else");
        if(!last_if_was_true){
            element.body.operator*().accept(*this);
        }
        last_was_if = false;
    };
    void visit(Condition &element) override{
        element.left.operator*().accept(*this);
        element.right.operator*().accept(*this);
        TryToAndOr(element.operationType);
    }
    void visit(RelationalCondition &element) override{
        element.left.operator*().accept(*this);
        element.right.operator*().accept(*this);
        TryToCompareValues(element.operationType);

    }
};


#endif //TKOM_21Z_DANIEL_INTERPRETER_H



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
    bool variable_access_from_fun_call = false;
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
    void addPrintFunction();
    void checkDate();
    std::string toString(variantTypes type);
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
        // sprawdza czy typ wyniku jest zgodny z zadeklarowanym typem zmiennej
        if(properType(element.typeOfData,results.back())){
            // sprawdza czy nie ma juz tej zmiennej w zakresie funkcji lub globalnym
            if(!functions_scopes.empty() && !functions_scopes.back().existsInLastScope(element.id))
                functions_scopes.back().addToScope(element.id,element.typeOfData,results.back());
            else if(functions_scopes.empty() && !global_scope.exists(element.id))
                global_scope.addToScope(element.id,element.typeOfData,results.back());
            else
                ErrorHandler::printInterpreterError("variable already declared",element.line,element.column);

            results.pop_back();
        }else
            ErrorHandler::printInterpreterError("wrong type in variable declaration",element.line,element.column);
    };
    // początek interpretacji drzewa
    void visit(Program &element) override
    {
        debug += "\nEntering interpreter\n";
        for(auto &declaration : element.declarations){
            if(declaration != nullptr)
                visit(declaration.operator*());
        }
        debug += "----------------------------------------\n";
        addPrintFunction();
        for(auto &function : element.functions){
            if(function != nullptr){
                if(functions.find(function->name) == functions.end())
                    functions.insert({function->name,std::move(function)});
                else
                    ErrorHandler::printInterpreterError("function "+function->name+" already defined",function->line,function->column);
            }
        }
        functions_scopes.emplace_back(FunctionCallContext());
        functions_scopes.back().addScope();
        auto main = functions.find("main");
        if(main != functions.end()){
            if(!main->second->parameters.empty())
                ErrorHandler::printInterpreterError("main function cannot have parameters",main->second->line,main->second->column);
            main->second->accept(*this);
        }
        else
            ErrorHandler::printInterpreterError("did not find main function");
        debug= debug +"Returned value: "+toString(results.back());
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
            ErrorHandler::printInterpreterError("negation not for bool type",element.line,element.column);
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
        // jeżeli jest to wywolanie funckji to trzeba  wartosci podanych argumentów szukać w konekście poprzedniej funkcji
        // wywolywane przez wizytator w funcall
        if(variable_access_from_fun_call){
            auto last_scope = functions_scopes.rbegin();
            last_scope++;
            if(last_scope.operator*().existsInScope(element.id) && last_scope->getValue(element.id).index() != 0) // 0 to std::monostate{}
                results.emplace_back(last_scope->getValue(element.id));
            else if(global_scope.exists(element.id) && global_scope.getValue(element.id).index() != 0) // 0 to std::monostate{}
                results.emplace_back(global_scope.getValue(element.id));
            else
                ErrorHandler::printInterpreterError("no variable in scope or value not assigned "+element.id,element.line,element.column);

        }else{
            if(functions_scopes.back().existsInScope(element.id) && functions_scopes.back().getValue(element.id).index() != 0) // 0 to std::monostate{}
                results.emplace_back(functions_scopes.back().getValue(element.id));
            else if(global_scope.exists(element.id) && global_scope.getValue(element.id).index() != 0) // 0 to std::monostate{}
                results.emplace_back(global_scope.getValue(element.id));
            else
                ErrorHandler::printInterpreterError("no variable in scope or value not assigned "+element.id,element.line,element.column);
        }
    };
    void visit(FunCall &element) override{
        // przy kazdym wywolaniu funkcji tworzony jest dla niej nowy kontekst wraz z pierwszym zakresem zmiennych
        last_was_if = false;
        functions_scopes.push_back(FunctionCallContext());
        functions_scopes.back().addScope();
        auto function = functions.find(element.id);
        if(function == functions.end())
            ErrorHandler::printInterpreterError("no such function defined "+element.id,element.line,element.column);
        if(element.arguments.size() != function->second->parameters.size())
            ErrorHandler::printInterpreterError("wrong number of arguments in function call " + element.id,element.line,element.column);

        variable_access_from_fun_call = true;
        for(auto argument = element.arguments.rbegin() ; argument != element.arguments.rend() ; argument++)
            argument.operator*()->accept(*this);
        variable_access_from_fun_call = false;

        function->second.operator*().accept(*this);
        functions_scopes.pop_back();
        to_continue = true;
    };
    void visit(Function &element) override{

        for(long long unsigned int i =0 ; i < element.parameters.size();i++){
            element.parameters[i].accept(*this);
        }
        if( element.name == "print"){
            std::cout<< std::get<std::string>(functions_scopes.back().getValue("to_print"));
            return;
        }

        element.body.operator*().accept(*this);
        if(results.empty() || to_continue)
            ErrorHandler::printInterpreterError("function "+element.name+" did not return anything",element.line,element.column);
        if(!properType(element.dataType,results.back()))
            ErrorHandler::printInterpreterError("wrong return type in function "+element.name,element.line,element.column);

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

        if(functions_scopes.back().existsInScope(element.var->id) && properType(functions_scopes.back().getType(element.var->id),results.back())){
            functions_scopes.back().changeInScope(element.var->id,results.back());
            results.pop_back();
        }
        else if(global_scope.exists(element.var->id) && properType(global_scope.getTokenType(element.var->id),results.back())){
            global_scope.changeValue(element.var->id,results.back());
            results.pop_back();
        }
        else
            ErrorHandler::printInterpreterError("no variable in scope or attempt to assign wrong type to variable "+element.var->id, element.line,element.column);
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
        int is_negation = times_negation % 2 ==0 ? 1:-1;
        if(is_negation == -1)
            results.emplace_back(!element.value);
        else
            results.emplace_back(element.value);
    };
    void visit(String &element) override{
        results.emplace_back(element.value);
    };
    void visit(Date &element) override{
        results.emplace_back(element);
    };
    void visit(TimeDiff &element) override{
        results.emplace_back(element);
    };
    void visit(Return &element) override{
        last_was_if = false;
        if(element.expression != nullptr)
            element.expression.operator*().accept(*this);

        // wychodzimy z petli w body
        to_continue = false;
    }
    void visit(While &element) override{
        last_was_if = false;
        functions_scopes.back().addScope();
        element.condition.operator*().accept(*this);
        if(results.back().index()!=3)
            ErrorHandler::printInterpreterError("cannot evaluate 'while' condition to bool",element.line,element.column);
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
        if(results.back().index()!=3)
            ErrorHandler::printInterpreterError("cannot evaluate 'if' condition to bool",element.line,element.column);
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
        // ustawiane dla przypadku gdy bedzie cześć else, aby wiedziec czy byla ona bezposrednio po if
        last_was_if = true;
    };
    void visit(Else &element) override{
        if(!last_was_if)
            ErrorHandler::printInterpreterError("no if statement before else statement",element.line,element.column);
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

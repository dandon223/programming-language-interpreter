

#ifndef TKOM_21Z_DANIEL_PROGRAM_H
#define TKOM_21Z_DANIEL_PROGRAM_H

#include <variant>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <memory>
class Declaration;
class VariableDeclr;
class Expression;
class IExpression;
class AdvExpression;
class BasicExpression;
class Variable;
class VariableAccess;
class Program;
class FunCall;
//class Function;
//class Body;
class Int;
class Double;
class String;
class Date;
class TimeDiff;
enum class TypeOfData
{
    Integer,
    Double,
    Message,
    Date,
    TimeDiff
};
const std::unordered_map<int, std::string> TypeOfDataToString = {

        {0, "Integer"},
        {1, "Double"},
        {2,"Message" },
        {3,"Date" },
        {4,"TimeDiff" }
};
class Visitor
{
public:
    virtual void visit(Declaration &element,int indentation) = 0;
    virtual void visit(FunCall &element,int indentation) = 0;
    virtual void visit(VariableAccess &element,int indentation) = 0;
    virtual void visit(Variable &element,int indentation) = 0;
    virtual void visit(VariableDeclr &element,int indentation) = 0;
    virtual void visit(Expression &element,int indentation) = 0;
    virtual void visit(AdvExpression &element,int indentation) = 0;
    virtual void visit(BasicExpression &element, int indentation) = 0;
    virtual void visit(Program &element,int indentation) = 0;
    //virtual void visit(Function &element,int indentation) = 0;
    //virtual void visit(Body &element,int indentation) = 0;
    virtual void visit(Int &element,int indentation) = 0;
    virtual void visit(Double &element,int indentation) = 0;
    virtual void visit(String &element,int indentation) = 0;
    virtual void visit(Date &element,int indentation) = 0;
    virtual void visit(TimeDiff &element,int indentation) = 0;
};

class INode
{
public:
    virtual void accept(Visitor &v,int indentation){};
};
class IExpression : public INode
{
public:
    bool wasMinus;
    virtual void accept(Visitor &v,int indentation){};
};
class Int : public INode
{
public:
    int value;
    Int(int i):value(i){};
    void accept(Visitor &visitor,int indentation) override
    {
        visitor.visit(*this,indentation);
    };
};
class Double : public INode
{
public:
    double value;
    Double(double i):value(i){};
    void accept(Visitor &visitor,int indentation) override
    {
        visitor.visit(*this,indentation);
    };
};
class String : public INode
{
public:
    std::string value;
    String(std::string i):value(i){};
    void accept(Visitor &visitor,int indentation) override
    {
        visitor.visit(*this,indentation);
    };
};
class Date : public INode{
private:
    int year = 1970;
    int month = 1;
    int day = 1;
public:
    Date(int y, int m, int d) :year(y),month(m),day(d){}
    std::string toString() {
        std::string year_date = std::to_string(year);
        std::string month_date = std::to_string(month);
        std::string day_date = std::to_string(day);
        if(year <10)
            year_date = "000" +year_date;
        else if(year <100)
            year_date = "00" +year_date;
        else if(year <1000)
            year_date = "0" +year_date;

        if(month < 10)
            month_date = "0" + month_date;

        if(day <10)
            day_date = "0" + day_date;
        return year_date+":"+month_date+":"+day_date;
    }
    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }

};
class TimeDiff : public INode{
private:
    int year=0;
    int month=0;
    int day =0;
public:
    TimeDiff(int y, int m, int d) :year(y),month(m),day(d){}

    std::string toString() {
        return std::to_string(year)+"y"+std::to_string(month)+"m"+std::to_string(day)+"d";
    }
    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }

};

class VariableDeclr : public INode
{
public:
    std::string id;
    TypeOfData typeOfData;
    void accept(Visitor &visitor,int indentation) override
    {
        visitor.visit(*this,indentation);
    };
    VariableDeclr(){};
    VariableDeclr(std::string _id, TypeOfData _typeOfData) : id(_id), typeOfData(_typeOfData){};
    std::string getId(){return id;}
    TypeOfData getTypeOfData(){return typeOfData;}
};
class AdvExpression :public IExpression
{
public:
    std::string operationType;
    std::unique_ptr<IExpression> left;
    std::unique_ptr<IExpression> right;
    AdvExpression(){};
    AdvExpression(std::string _operationType, std::unique_ptr<IExpression> _left, std::unique_ptr<IExpression> _right) : operationType(_operationType), left(std::move(_left)), right(std::move(_right)){};
    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }
};
class Expression : public IExpression
{
public:
    std::string operationType;
    std::unique_ptr<IExpression> left;
    std::unique_ptr<IExpression> right;
    Expression(){};
    Expression(std::string _operationType, std::unique_ptr<IExpression> _left) : operationType(_operationType), left(std::move(_left)){};
    Expression(std::string _operationType, std::unique_ptr<IExpression> _left, std::unique_ptr<IExpression> _right) : operationType(_operationType), left(std::move(_left)), right(std::move(_right)){};
    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }
};
class Declaration : public INode
{
public:
    VariableDeclr var;
    std::unique_ptr<IExpression> assignable;
    Declaration(){};
    Declaration(VariableDeclr _var, std::unique_ptr<IExpression> _assignable) : var(_var), assignable(std::move(_assignable)) {}
    void accept(Visitor &visitor,int indentation) override{};

};

class VariableAccess : public INode
{
public:
    std::string id;
    VariableAccess(std::string _id) : id(_id){};
void accept(Visitor &visitor,int indentation) override{
    visitor.visit(*this,indentation);
};
};
class Variable : public INode
{
public:
    std::string id;
    TypeOfData typeOfData;
    std::variant<int,double,std::string,Date,TimeDiff> value;
    Variable(){};
    Variable(std::string _id, TypeOfData _typeOfData) : id(_id), typeOfData(_typeOfData){};
    void accept(Visitor &visitor,int indentation) override{
        visitor.visit(*this,indentation);
    };
};
class FunCall : public INode
{
public:
    std::string id;
    std::vector<std::shared_ptr<IExpression>> arguments;
    FunCall(std::string _id, std::vector<std::shared_ptr<IExpression>> _arguments) : id(_id), arguments(std::move(_arguments)){
    };
    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }
};
struct BasicGet {
    std::string operator()(int value) { return "Int "+std::to_string(value); }
    std::string operator()(double value) { return "Double "+std::to_string(value); }
    std::string operator()(std::string value) { return "String \""+value+"\"";}
    std::string operator()(Date value) { return "Date "+value.toString();}
    std::string operator()(TimeDiff value) { return "TimeDiff "+value.toString();}
    std::string operator()(VariableAccess value) { return "VariableId " +value.id;}
    std::string operator()(FunCall value) { return "FunCall";}

};
class BasicExpression :public IExpression
{
public:
    std::unique_ptr<INode> basic;
    BasicExpression(){};
    BasicExpression(std::unique_ptr<INode> _basic, bool _wasMinus) : basic(std::move(_basic)){wasMinus = _wasMinus;};
    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }
};
//class Body : INode
//{
//public:
//    std::vector<std::variant< Declaration, FunCall>> statements;
//
//    Body(std::vector<std::variant< Declaration, FunCall>>_statements) : statements(_statements){};
//    Body(){};
//
//    void accept(Visitor &v,int indentation) override {
//        v.visit(*this,indentation);
//    }
//};
//class Function: public INode
//{
//public:
//    explicit Function(std::string name, TypeOfData dataType,
//                      std::vector<VariableDeclr> parameters, Body block) : _name(name),
//                                                                            _dataType(dataType), _parameters(parameters), _block(block){};
//    Function(){};
//    std::string _name;
//    TypeOfData _dataType;
//    std::vector<VariableDeclr> _parameters;
//    Body _block;
//    void accept(Visitor &visitor,int indentation) override
//    {
//        visitor.visit(*this,indentation);
//    };
//};
class Program : public INode
{
public:
    std::vector<std::unique_ptr<Declaration>> declarations;
    Program(){};
    Program(std::vector<std::unique_ptr<Declaration>> _declarations) : declarations(std::move(_declarations)){};
    void accept(Visitor &visitor,int indentation) override
    {
        visitor.visit(*this,indentation);
    };
    std::vector<std::unique_ptr<Declaration>> getDeclarations(){return std::move(declarations);}
};

class PrintVisitor : public Visitor{
public:
    std::string debug;
    void visit(VariableDeclr &element,int indentation) override
    {
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered VariableDeclr [";
        debug += "Type of data: " + TypeOfDataToString.at(static_cast<const int>(element.getTypeOfData())) + "," ;
        debug += "Id: " + element.getId()+"]\n";

    };
    void visit(Program &element,int indentation) override
    {
        for(auto &declaration : element.declarations)
            visit(declaration.operator*(),indentation);
    };

    void visit(AdvExpression &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered AdvExpression\n";
        element.left.operator*().accept(*this,indentation+2);
        debug += spaces+"Operator: " +element.operationType+"\n";
        element.right.operator*().accept(*this,indentation+2);
    };
    void visit(Expression &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered Expression[";
        if(element.wasMinus)
            debug += " Is Minus = true]\n";
        else
            debug += "]\n";
        element.left.operator*().accept(*this,indentation+2);
        debug += spaces+"Operator: " +element.operationType+"\n";
        element.right.operator*().accept(*this,indentation+2);
        debug += spaces+"exited Expression\n";
    };
    void visit(BasicExpression &element ,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered BasicExpression [";
        if(element.wasMinus)
            debug +="Is Minus = true, ";
        else
            debug +="Is Minus = false, ";
        element.basic->accept(*this,indentation+2);
    };
    void visit(Declaration &element,int indentation) override{
        debug += "entered Declaration\n";
        visit(element.var,indentation+2);
        element.assignable.operator*().accept(*this,indentation+2);
        debug += "exited Declaration\n";

    };
    void visit(Variable &element,int indentation) override{

    };
    void visit(VariableAccess &element,int indentation) override{
        debug+="Type = VariableAccess, Value = " + element.id +"]\n";
    };
    void visit(FunCall &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += "entered FunCall [ name = "+element.id+",args:\n";
        for(long long unsigned int i = 0 ; i < element.arguments.size() ; i++){
            debug += spaces;
            element.arguments[i]->accept(*this,indentation+2);
        }

        debug += spaces+"]\n";
    };
//    void visit(Function &element,int indentation) override{
//
//    };
//    void visit(Body &element,int indentation) override{
//
//    };
    void visit(Int &element,int indentation) override{
        debug+="Type = Int, Value = " + std::to_string(element.value) +"]\n";
    };
    void visit(Double &element,int indentation) override{
        debug+="Type = Double, Value = " + std::to_string(element.value) +"]\n";
    };
    void visit(String &element,int indentation) override{
        debug+="Type = String, Value = " + element.value +"]\n";
    };
    void visit(Date &element,int indentation) override{
        debug+="Type = Date, Value = " + element.toString() +"]\n";
    };
    void visit(TimeDiff &element,int indentation) override{
        debug+="Type = TimeDiff, Value = " + element.toString() +"]\n";
    };


};
#endif //TKOM_21Z_DANIEL_PROGRAM_H

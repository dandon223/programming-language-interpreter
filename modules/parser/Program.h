

#ifndef TKOM_21Z_DANIEL_PROGRAM_H
#define TKOM_21Z_DANIEL_PROGRAM_H

#include <variant>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
class Declaration;
class VariableDeclr;
class Expression;
class AdvExpression;
class BasicExpression;
class Program;
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
    virtual void visit(Declaration *element) = 0;
    virtual void visit(VariableDeclr *element) = 0;
    virtual void visit(Expression *element) = 0;
    virtual void visit(AdvExpression *element) = 0;
    virtual void visit(BasicExpression *element) = 0;
    virtual void visit(Program *element) = 0;
};

class INode
{
public:
    virtual void accept(Visitor *v) = 0;
};
class VariableDeclr : INode
{
public:
    std::string id;
    TypeOfData typeOfData;
    void accept(Visitor *visitor) override
    {
        visitor->visit(this);
    };
    VariableDeclr(){};
    VariableDeclr(std::string _id, TypeOfData _typeOfData) : id(_id), typeOfData(_typeOfData){};
    std::string getId(){return id;}
    TypeOfData getTypeOfData(){return typeOfData;}
};
class Expression : INode
{
public:
    std::vector<std::string> operationType;
    std::vector<AdvExpression> left;
    std::vector<AdvExpression> right;
    Expression(){};
    Expression(std::vector<std::string> _operationType, std::vector<AdvExpression> _left,
               std::vector<AdvExpression> _right) : operationType(_operationType), left(_left), right(_right){};
    void accept(Visitor *visitor) override
    {
        visitor->visit(this);
    };
};
class Declaration : INode
{
public:
    VariableDeclr var;
    std::optional<Expression> assignable;
    Declaration(){};
    Declaration(VariableDeclr _var, std::optional<Expression> _assignable) : var(_var), assignable(_assignable) {}
    void accept(Visitor *visitor) override
    {
        visitor->visit(this);
    };

};

class AdvExpression : INode
{
public:
    std::vector<std::string> operationType;
    std::vector<BasicExpression> left;
    std::vector<BasicExpression> right;
    AdvExpression(){};
    AdvExpression(std::vector<std::string> _operationType, std::vector<BasicExpression> _left,
                  std::vector<BasicExpression> _right) : operationType(_operationType), left(_left), right(_right){};
    void accept(Visitor *visitor) override
    {
        visitor->visit(this);
    };
};
class BasicExpression : INode
{
public:
    std::variant<int> basic;
    bool wasMinus;
    BasicExpression(){};
    BasicExpression(std::variant<int> _basic, bool _wasMinus) : basic(_basic), wasMinus(_wasMinus){};
    void accept(Visitor *visitor) override
    {
        visitor->visit(this);
    };
};

class Program : INode
{
public:
    std::vector<Declaration> declarations;
    Program(){};
    Program(std::vector<Declaration> _declarations) : declarations(_declarations){};
    void accept(Visitor *visitor) override
    {
        visitor->visit(this);
    };
    std::vector<Declaration> getDeclarations(){return declarations;}
};

class PrintVisitor : public Visitor{
public:
    std::string debug;
    void visit(Declaration *element) override
    {
        debug += " entered Declaration\n";
        visit(&(element->var));
    };
    void visit(VariableDeclr *element) override
    {
        debug += " entered VariableDeclr\n";
        debug += "\tType of data: " + TypeOfDataToString.at(static_cast<const int>(element->getTypeOfData())) + "\n" ;
        debug += "\tId: " + element->getId()+"\n";

    };
    void visit(Program *element) override
    {
        for(auto declaration : element->declarations)
            visit(&declaration);
    };
    void visit(Expression *element) override{

    };
    void visit(AdvExpression *element) override{

    } ;
    void visit(BasicExpression *element) override{

    };


};
#endif //TKOM_21Z_DANIEL_PROGRAM_H

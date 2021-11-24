

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
    virtual void visit(Declaration *element,int indentation) = 0;
    virtual void visit(std::shared_ptr<Declaration> element,int indentation) = 0;
    virtual void visit(VariableDeclr *element,int indentation) = 0;
    virtual void visit(Expression *element) = 0;
    virtual void visit(std::shared_ptr<Expression> element,int indentation) = 0;
    virtual void visit(AdvExpression *element) = 0;
    virtual void visit(std::shared_ptr<AdvExpression> element,int indentation) = 0;
    virtual void visit(BasicExpression *element) = 0;
    virtual void visit(std::shared_ptr<BasicExpression> element, int indentation) = 0;
    virtual void visit(Program *element,int indentation) = 0;
};

class INode
{
public:
    virtual void accept(Visitor *v,int indentation) = 0;
};
class IExpression
{
public:
    virtual void accept(Visitor &v,int indentation){};
};

class VariableDeclr : public INode
{
public:
    std::string id;
    TypeOfData typeOfData;
    void accept(Visitor *visitor,int indentation) override
    {
        visitor->visit(this,indentation);
    };
    VariableDeclr(){};
    VariableDeclr(std::string _id, TypeOfData _typeOfData) : id(_id), typeOfData(_typeOfData){};
    std::string getId(){return id;}
    TypeOfData getTypeOfData(){return typeOfData;}
};
class AdvExpression :public IExpression, public std::enable_shared_from_this<AdvExpression>
{
public:
    std::string operationType;
    std::shared_ptr<IExpression> left;
    std::shared_ptr<IExpression> right;
    AdvExpression(){};
    AdvExpression(std::string _operationType, std::shared_ptr<IExpression> _left, std::shared_ptr<IExpression> _right) : operationType(_operationType), left(std::move(_left)), right(std::move(_right)){};
    void accept(Visitor &v,int indentation) override {
        std::shared_ptr<AdvExpression> p{shared_from_this()};
        v.visit(p,indentation);
    }
};
class Expression : public IExpression, public std::enable_shared_from_this<Expression>
{
public:
    std::string operationType;
    std::shared_ptr<IExpression> left;
    std::shared_ptr<IExpression> right;
    Expression(){};
    Expression(std::string _operationType, std::shared_ptr<IExpression> _left) : operationType(_operationType), left(std::move(_left)){};
    Expression(std::string _operationType, std::shared_ptr<IExpression> _left, std::shared_ptr<IExpression> _right) : operationType(_operationType), left(std::move(_left)), right(std::move(_right)){};
    void accept(Visitor &v,int indentation) override {
        std::shared_ptr<Expression> p{shared_from_this()};
        v.visit(p,indentation);
    }
};
class Declaration : public INode
{
public:
    VariableDeclr var;
    std::shared_ptr<IExpression> assignable;
    Declaration(){};
    Declaration(VariableDeclr _var, std::shared_ptr<IExpression> _assignable) : var(_var), assignable(std::move(_assignable)) {}
    void accept(Visitor *visitor,int indentation) override
    {
        visitor->visit(this,indentation);
    };

};

struct BasicGet {
    std::string operator()(int value) { return std::to_string(value); }
    std::string operator()(double value) { return std::to_string(value); }
    std::string operator()(std::string value) { return value;}

};
class BasicExpression :public IExpression, public std::enable_shared_from_this<BasicExpression>
{
public:
    std::variant<int,double,std::string> basic;
    bool wasMinus;
    BasicExpression(){};
    BasicExpression(std::variant<int,double,std::string> _basic, bool _wasMinus) : basic(_basic), wasMinus(_wasMinus){};
    void accept(Visitor &v,int indentation) override {
        std::shared_ptr<BasicExpression> p{shared_from_this()};
        v.visit(p,indentation);
    }
    std::string ValueToString(){
        return std::visit(BasicGet{}, basic);
    }
};

class Program : public INode
{
public:
    std::vector<std::shared_ptr<Declaration>> declarations;
    Program(){};
    Program(std::vector<std::shared_ptr<Declaration>> _declarations) : declarations(std::move(_declarations)){};
    void accept(Visitor *visitor,int indentation) override
    {
        visitor->visit(this,indentation);
    };
    std::vector<std::shared_ptr<Declaration>> getDeclarations(){return declarations;}
};

class PrintVisitor : public Visitor{
public:
    std::string debug;
    void visit(Declaration *element,int indentation) override
    {
        debug += "entered Declaration\n";
        visit(&(element->var),indentation+2);
    };
    void visit(VariableDeclr *element,int indentation) override
    {
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered VariableDeclr [";
        debug += "Type of data: " + TypeOfDataToString.at(static_cast<const int>(element->getTypeOfData())) + "," ;
        debug += "Id: " + element->getId()+"]\n";

    };
    void visit(Program *element,int indentation) override
    {
        for(auto &declaration : element->declarations)
            visit(declaration,indentation);
    };
    void visit(Expression *element) override{

    };
    void visit(AdvExpression *element) override{

    } ;
    void visit(BasicExpression* element) override{

    };

    void visit(std::shared_ptr<AdvExpression> element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered AdvExpression\n";
        auto sq = std::dynamic_pointer_cast<BasicExpression>(element->left);
        if(sq)
            visit(sq,indentation+2);
        else if(auto s = std::dynamic_pointer_cast<AdvExpression>(element->left))
            visit(s,indentation+2);
        else if(auto x = std::dynamic_pointer_cast<Expression>(element->left))
            visit(x,indentation+2);
        debug += spaces+"Operator: " +element->operationType+"\n";
        auto a = std::dynamic_pointer_cast<BasicExpression>(element->right);
        if(a)
            visit(a,indentation+2);
        else if(auto s = std::dynamic_pointer_cast<AdvExpression>(element->right))
            visit(s,indentation+2);
        else if(auto x = std::dynamic_pointer_cast<Expression>(element->right))
            visit(x,indentation+2);
    };
    void visit(std::shared_ptr<Expression> element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered Expression\n";
        auto sq = std::dynamic_pointer_cast<BasicExpression>(element->left);
        if(sq)
            visit(sq,indentation+2);
        else if(auto s = std::dynamic_pointer_cast<AdvExpression>(element->left))
            visit(s,indentation+2);
        else if(auto x = std::dynamic_pointer_cast<Expression>(element->left))
            visit(x,indentation+2);
        debug += spaces+"Operator: " +element->operationType+"\n";
        auto a = std::dynamic_pointer_cast<BasicExpression>(element->right);
        if(a)
            visit(a,indentation+2);
        else if(auto s = std::dynamic_pointer_cast<AdvExpression>(element->right))
            visit(s,indentation+2);
        else if(auto x = std::dynamic_pointer_cast<Expression>(element->right))
            visit(x,indentation+2);
        debug += spaces+"exited Expression\n";
    };
    void visit(std::shared_ptr<BasicExpression> element ,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered BasicExpression [";
        if(element->wasMinus)
            debug +="Is Minus = true";
        else
            debug +="Is Minus = false";
        debug += ",Value:"+ element->ValueToString()+"]\n";
    };
    void visit(std::shared_ptr<Declaration> element,int indentation) override{
        debug += "entered Declaration\n";
        visit(&(element->var),indentation+2);
        auto sq = std::dynamic_pointer_cast<BasicExpression>(element->assignable);
        if(sq)
            visit(sq,indentation+2);
        else if(auto s = std::dynamic_pointer_cast<AdvExpression>(element->assignable))
            visit(s,indentation+2);
        else if(auto x = std::dynamic_pointer_cast<Expression>(element->assignable))
            visit(x,indentation+2);
        debug += "exited Declaration\n";

    };


};
#endif //TKOM_21Z_DANIEL_PROGRAM_H

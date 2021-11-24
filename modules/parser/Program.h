

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
    virtual void visit(Declaration *element) = 0;
    virtual void visit(std::shared_ptr<Declaration> element) = 0;
    virtual void visit(VariableDeclr *element) = 0;
    virtual void visit(Expression *element) = 0;
    virtual void visit(std::shared_ptr<Expression> element) = 0;
    virtual void visit(AdvExpression *element) = 0;
    virtual void visit(std::shared_ptr<AdvExpression> element) = 0;
    virtual void visit(BasicExpression *element) = 0;
    virtual void visit(std::shared_ptr<BasicExpression> element) = 0;
    virtual void visit(Program *element) = 0;
};

class INode
{
public:
    virtual void accept(Visitor *v) = 0;
};
class IExpression
{
public:
    virtual void accept(Visitor &v){};
};

class VariableDeclr : public INode
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
class AdvExpression :public IExpression, public std::enable_shared_from_this<AdvExpression>
{
public:
    std::string operationType;
    std::shared_ptr<IExpression> left;
    std::shared_ptr<IExpression> right;
    AdvExpression(){};
    AdvExpression(std::string _operationType, std::shared_ptr<IExpression> _left, std::shared_ptr<IExpression> _right) : operationType(_operationType), left(std::move(_left)), right(std::move(_right)){};
    void accept(Visitor &v) override {
        std::shared_ptr<AdvExpression> p{shared_from_this()};
        v.visit(p);
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
    void accept(Visitor &v) override {
        std::shared_ptr<Expression> p{shared_from_this()};
        v.visit(p);
    }
};
class Declaration : public INode
{
public:
    VariableDeclr var;
    std::shared_ptr<IExpression> assignable;
    Declaration(){};
    Declaration(VariableDeclr _var, std::shared_ptr<IExpression> _assignable) : var(_var), assignable(std::move(_assignable)) {}
    void accept(Visitor *visitor) override
    {
        visitor->visit(this);
    };

};

struct BasicGet {
    std::string operator()(int value) { return std::to_string(value); }
    std::string operator()(std::string value) { return value;}
};
class BasicExpression :public IExpression, public std::enable_shared_from_this<BasicExpression>
{
public:
    std::variant<int,std::string> basic;
    bool wasMinus;
    BasicExpression(){};
    BasicExpression(std::variant<int,std::string> _basic, bool _wasMinus) : basic(_basic), wasMinus(_wasMinus){};
    void accept(Visitor &v) override {
        std::shared_ptr<BasicExpression> p{shared_from_this()};
        v.visit(p);
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
    void accept(Visitor *visitor) override
    {
        visitor->visit(this);
    };
    std::vector<std::shared_ptr<Declaration>> getDeclarations(){return declarations;}
};

class PrintVisitor : public Visitor{
public:
    std::string debug;
    void visit(Declaration *element) override
    {
        debug += "entered Declaration\n";
        visit(&(element->var));
    };
    void visit(VariableDeclr *element) override
    {
        debug += "entered VariableDeclr\n";
        debug += "\tType of data: " + TypeOfDataToString.at(static_cast<const int>(element->getTypeOfData())) + "\n" ;
        debug += "\tId: " + element->getId()+"\n";
        debug += "exited VariableDeclr\n";

    };
    void visit(Program *element) override
    {
        for(auto &declaration : element->declarations)
            visit(declaration);
    };
    void visit(Expression *element) override{

    };
    void visit(AdvExpression *element) override{

    } ;
    void visit(BasicExpression* element) override{

    };

    void visit(std::shared_ptr<AdvExpression> element) override{
        debug += "entered AdvExpression\n";
    };
    void visit(std::shared_ptr<Expression> element) override{
        debug += "entered Expression\n";
        auto sq = std::dynamic_pointer_cast<BasicExpression>(element->left);
        if(sq)
            visit(sq);
        else if(auto s = std::dynamic_pointer_cast<AdvExpression>(element->left))
            visit(s);
        else if(auto x = std::dynamic_pointer_cast<Expression>(element->left))
            visit(x);
        debug += "Operator: " +element->operationType+"\n";
        auto a = std::dynamic_pointer_cast<BasicExpression>(element->right);
        if(a)
            visit(a);
        else if(auto s = std::dynamic_pointer_cast<AdvExpression>(element->right))
            visit(s);
        else if(auto x = std::dynamic_pointer_cast<Expression>(element->right))
            visit(x);
        debug += "exited Expression\n";
    };
    void visit(std::shared_ptr<BasicExpression> element) override{
        debug += "entered BasicExpression\n";
        if(element->wasMinus)
            debug += "\tIs Minus = true\n";
        else
            debug += "\tIs Minus = false\n";
        debug += "\tValue:"+ element->ValueToString()+"\n";
        debug += "exited BasicExpression\n";

    };
    void visit(std::shared_ptr<Declaration> element) override{
        debug += "entered Declaration\n";
        visit(&(element->var));
        auto sq = std::dynamic_pointer_cast<BasicExpression>(element->assignable);
        if(sq)
            visit(sq);
        else if(auto s = std::dynamic_pointer_cast<AdvExpression>(element->assignable))
            visit(s);
        else if(auto x = std::dynamic_pointer_cast<Expression>(element->assignable))
            visit(x);
        debug += "exited Declaration\n";

    };
//    void visit(std::shared_ptr<IExpression> element) override{
//        debug += "Smuteczel\n";
//    };


};
#endif //TKOM_21Z_DANIEL_PROGRAM_H

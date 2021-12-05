

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
class AssignStatement;
class AdvExpression;
class BasicExpression;
class Variable;
class VariableAccess;
class Program;
class FunCall;
class Return;
class Function;
class Body;
class If;
class Else;
class While;
class Condition;
class RelationalCondition;
class Int;
class Double;
class String;
class Date;
class TimeDiff;
class INode;
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
        {1, "Float"},
        {2,"Message" },
        {3,"Date" },
        {4,"TimeDiff" }
};
class Visitor
{
public:
    virtual void visit(Declaration &element,int indentation) = 0;
    virtual void visit(Return &element,int indentation) = 0;
    virtual void visit(While &element,int indentation) = 0;
    virtual void visit(If &element,int indentation) = 0;
    virtual void visit(Else &element,int indentation) = 0;
    virtual void visit(Condition &element,int indentation) = 0;
    virtual void visit(RelationalCondition &element,int indentation) = 0;
    virtual void visit(AssignStatement &element,int indentation) = 0;
    virtual void visit(FunCall &element,int indentation) = 0;
    virtual void visit(VariableAccess &element,int indentation) = 0;
    virtual void visit(Variable &element,int indentation) = 0;
    virtual void visit(VariableDeclr &element,int indentation) = 0;
    virtual void visit(Expression &element,int indentation) = 0;
    virtual void visit(AdvExpression &element,int indentation) = 0;
    virtual void visit(BasicExpression &element, int indentation) = 0;
    virtual void visit(Program &element,int indentation) = 0;
    virtual void visit(Function &element,int indentation) = 0;
    virtual void visit(Body &element,int indentation) = 0;
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
    bool wasMinus = false;
    bool wasNegation = false;
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
class VariableAccess : public INode
{
public:
    std::string id;
    VariableAccess(std::string _id) : id(_id){};
    void accept(Visitor &visitor,int indentation) override{
        visitor.visit(*this,indentation);
    };
};


class AssignStatement : public INode
{
public:
    std::unique_ptr<VariableAccess> var;
    std::unique_ptr<IExpression> assignable;
    AssignStatement(){};
    AssignStatement(std::unique_ptr<VariableAccess> _var, std::unique_ptr<IExpression> _assignable) : var(std::move(_var)), assignable(std::move(_assignable)){};
    void accept(Visitor &visitor,int indentation) override
    {
        visitor.visit(*this,indentation);
    };
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
class Return : public INode
{
public:
    std::unique_ptr<IExpression> expression;
    Return(){};
    Return(std::unique_ptr<IExpression> _expression) : expression(std::move(_expression)){};
    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }
};
class Condition : public IExpression
{
public:
    std::string operationType;
    std::unique_ptr<IExpression> left;
    std::unique_ptr<IExpression> right;
    Condition(){};
    Condition(std::string _operationType, std::unique_ptr<IExpression> _left,
              std::unique_ptr<IExpression> _right) : operationType(_operationType), left(std::move(_left)), right(std::move(_right)){};
    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }
};
class RelationalCondition :public IExpression
{
public:
    std::string operationType;
    std::unique_ptr<IExpression> left;
    std::unique_ptr<IExpression> right;
    RelationalCondition(){};
    RelationalCondition(std::string _operationType, std::unique_ptr<IExpression> _left, std::unique_ptr<IExpression> _right) : operationType(_operationType), left(std::move(_left)), right(std::move(_right)){};
    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }
};
class If : public INode
{
public:
    std::unique_ptr<IExpression> condition;
    std::unique_ptr<Body> body;
    If(){};
    If(std::unique_ptr<IExpression> _condition, std::unique_ptr<Body> _body) : condition(std::move(_condition)), body(std::move(_body)){};
    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }
};
class Else : public INode
{
public:
    std::unique_ptr<Body> body;
    Else(){};
    Else(std::unique_ptr<Body> _body) : body(std::move(_body)){};
    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }
};
class While : public INode
{
public:
    std::unique_ptr<IExpression> condition;
    std::unique_ptr<Body> body;
    While(){};
    While(std::unique_ptr<IExpression> _condition, std::unique_ptr<Body> _body) : condition(std::move(_condition)), body(std::move(_body)){};
    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }
};
class Body : public INode
{
public:
    std::vector<std::shared_ptr<INode>> statements; // declaration, assignStatement, FunCall, Return, While,

    Body(std::vector<std::shared_ptr<INode>>_statements) : statements(std::move(_statements)){};
    Body(){};

    void accept(Visitor &v,int indentation) override {
        v.visit(*this,indentation);
    }
};
class Function: public INode
{
public:
    explicit Function(std::string _name, TypeOfData _dataType,
                      std::vector<VariableDeclr> _parameters, std::unique_ptr<Body> _body) : name(_name),
                                                                            dataType(_dataType), parameters(std::move(_parameters)), body(std::move(_body)){};
    explicit Function(std::string _name, TypeOfData _dataType,
                      std::vector<VariableDeclr> _parameters) : name(_name),
                                                                          dataType(_dataType), parameters(std::move(_parameters)){};
    Function(){};
    std::string name;
    TypeOfData dataType;
    std::vector<VariableDeclr> parameters;
    std::unique_ptr<Body> body;
    void accept(Visitor &visitor,int indentation) override
    {
        visitor.visit(*this,indentation);
    };
};
class Program : public INode
{
public:
    std::vector<std::unique_ptr<Declaration>> declarations;
    std::vector<std::unique_ptr<Function>> functions;
    Program(){};
    Program(std::vector<std::unique_ptr<Declaration>> _declarations,std::vector<std::unique_ptr<Function>> _functions) : declarations(std::move(_declarations)),functions(std::move(_functions)){};
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
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered AdvExpression [";
        if(element.wasNegation)
            debug += " Negation";
        if(element.wasMinus)
            debug +=", Minus";
        debug+="]\n";
        if(element.left != nullptr)
            element.left.operator*().accept(*this,indentation+2);
        debug += spaces+"Operator: " +element.operationType+"\n";
        if(element.right != nullptr)
            element.right.operator*().accept(*this,indentation+2);
    };
    void visit(Expression &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered Expression[";
        if(element.wasNegation)
            debug += " Negation";
        if(element.wasMinus)
            debug +=", Minus";
        debug+="]\n";
        if(element.left != nullptr)
            element.left.operator*().accept(*this,indentation+2);
        debug += spaces+"Operator: " +element.operationType+"\n";
        if(element.right != nullptr)
            element.right.operator*().accept(*this,indentation+2);
        debug += spaces+"exited Expression\n";
    };
    void visit(BasicExpression &element ,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered BasicExpression [";
        if(element.wasNegation)
            debug += " Negation, ";
        if(element.wasMinus)
            debug +="Minus, ";
        if(element.basic != nullptr)
            element.basic->accept(*this,indentation+2);
    };
    void visit(Declaration &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered Declaration\n";
        visit(element.var,indentation+2);
        if(element.assignable != nullptr)
            element.assignable.operator*().accept(*this,indentation+2);
        debug += spaces+"exited Declaration\n";

    };
    void visit(Variable &element,int indentation) override{
        debug += "entered Variable\n";
    };
    void visit(VariableAccess &element,int indentation) override{
        debug+="Type = VariableAccess, Value = " + element.id +"]\n";
    };
    void visit(FunCall &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered FunCall [ name = "+element.id+",args:\n";
        for(long long unsigned int i = 0 ; i < element.arguments.size() ; i++){
            debug += spaces;
            if(element.arguments[i] != nullptr)
                element.arguments[i]->accept(*this,indentation+2);
        }
        debug += spaces+"]\n";
    };
    void visit(Function &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug+=spaces + "Function [ name = "+element.name+", returnType = "+TypeOfDataToString.at(static_cast<const int>(element.dataType))+", parameters:\n";
        for(auto a : element.parameters)
            a.accept(*this,indentation+2);
        if(element.body != nullptr)
            element.body->accept(*this,indentation+2);
    };
    void visit(Body &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug+=spaces+",Body [\n";
        for(long long unsigned int i = 0 ; i < element.statements.size() ; i++){
            if(element.statements[i] != nullptr)
                element.statements[i]->accept(*this,indentation+2);
        }
    };
    void visit(AssignStatement &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug+= spaces+"entered AssignStatement [ ";
        if(element.var != nullptr)
            element.var->accept(*this,indentation+2);
        if(element.assignable != nullptr)
            element.assignable->accept(*this,indentation+2);
    }
    void visit(Int &element,int indentation) override{
        debug+="Type = Int, Value = " + std::to_string(element.value) +"]\n";
    };
    void visit(Double &element,int indentation) override{
        debug+="Type = Float, Value = " + std::to_string(element.value) +"]\n";
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
    void visit(Return &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered Return\n";
        if(element.expression != nullptr)
            element.expression->accept(*this,indentation+2);
    }
    void visit(While &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered While\n";
        if(element.condition != nullptr)
            element.condition->accept(*this,indentation+2);
        if(element.body != nullptr)
            element.body->accept(*this,indentation+2);
    };
    void visit(If &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered If\n";
        if(element.condition != nullptr)
            element.condition->accept(*this,indentation+2);
        if(element.body != nullptr)
            element.body->accept(*this,indentation+2);
    };
    void visit(Else &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered Else\n";
        if(element.body != nullptr)
            element.body->accept(*this,indentation+2);
    };
    void visit(Condition &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered Condition [";
        if(element.wasNegation)
            debug +=" Negation";
        if(element.wasMinus)
            debug +=", Minus";
        debug += "]\n";
        if(element.left != nullptr)
            element.left.operator*().accept(*this,indentation+2);
        debug += spaces+"Operator: " +element.operationType+"\n";
        if(element.right != nullptr)
            element.right.operator*().accept(*this,indentation+2);
        debug += spaces+"Exited Condition\n";
    }
    void visit(RelationalCondition &element,int indentation) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"entered RelationalCondition [";
        if(element.wasNegation)
            debug += " Negation";
        if(element.wasMinus)
            debug +=", Minus";
        debug+="]\n";
        if(element.left != nullptr)
            element.left.operator*().accept(*this,indentation+2);
        debug += spaces+"Operator: " +element.operationType+"\n";
        if(element.right != nullptr)
            element.right.operator*().accept(*this,indentation+2);
    }


};
#endif //TKOM_21Z_DANIEL_PROGRAM_H

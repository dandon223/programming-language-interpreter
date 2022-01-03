

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
class ParenthesisExpression;
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
class Bool;
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
    TimeDiff,
    Bool
};
const std::unordered_map<int, std::string> TypeOfDataToString = {

        {0, "Integer"},
        {1, "Float"},
        {2,"Message" },
        {3,"Date" },
        {4,"TimeDiff" },
        {5,"Bool" }

};
class Visitor
{
public:
    virtual void visit(Bool &element) = 0;
    virtual void visit(Declaration &element) = 0;
    virtual void visit(Return &element) = 0;
    virtual void visit(While &element) = 0;
    virtual void visit(If &element) = 0;
    virtual void visit(Else &element) = 0;
    virtual void visit(Condition &element) = 0;
    virtual void visit(RelationalCondition &element) = 0;
    virtual void visit(AssignStatement &element) = 0;
    virtual void visit(FunCall &element) = 0;
    virtual void visit(VariableAccess &element) = 0;
    virtual void visit(Variable &element) = 0;
    virtual void visit(VariableDeclr &element) = 0;
    virtual void visit(Expression &element) = 0;
    virtual void visit(ParenthesisExpression &element) = 0;
    virtual void visit(AdvExpression &element) = 0;
    virtual void visit(BasicExpression &element) = 0;
    virtual void visit(Program &element) = 0;
    virtual void visit(Function &element) = 0;
    virtual void visit(Body &element) = 0;
    virtual void visit(Int &element) = 0;
    virtual void visit(Double &element) = 0;
    virtual void visit(String &element) = 0;
    virtual void visit(Date &element) = 0;
    virtual void visit(TimeDiff &element) = 0;
};

class INode
{
public:
    virtual void accept(Visitor &v){};
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
    void accept(Visitor &visitor) override
    {
        visitor.visit(*this);
    };

};
class Bool : public INode
{
public:
    bool value;
    Bool( bool i):value(i){};
    void accept(Visitor &visitor) override
    {
        visitor.visit(*this);
    };
};
class Double : public INode
{
public:
    double value;
    Double(double i):value(i){};
    void accept(Visitor &visitor) override
    {
        visitor.visit(*this);
    };
};
class String : public INode
{
public:
    std::string value;
    String(std::string i):value(i){};
    void accept(Visitor &visitor) override
    {
        visitor.visit(*this);
    };
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
    void accept(Visitor &v) override {
        v.visit(*this);
    }
    bool operator ==( const TimeDiff & v){
        return (this->year == v.year && this->month == v.month && this->day == v.day);
    };
    bool operator !=( const TimeDiff & v){
        return (this->year != v.year || this->month != v.month || this->day != v.day);
    };
    bool operator <=( const TimeDiff & v){
        bool same =  (this->year == v.year && this->month == v.month && this->day == v.day);
        if(same)
            return true;
        return this->year < v.year || (this->year==v.year && this->month<v.month) || (this->year == v.year && this->month == v.month && this->day < v.day);
    };
    bool operator <( const TimeDiff & v){
        return this->year < v.year || (this->year==v.year && this->month<v.month) || (this->year == v.year && this->month == v.month && this->day < v.day);
    };

};
class Date : public INode{
private:
    int year = 1970;
    int month = 1;
    int day = 1;
public:
    Date(int y, int m, int d) :year(y),month(m),day(d){}
    bool operator ==( const Date & v){
        return (this->year == v.year && this->month == v.month && this->day == v.day);
    };
    bool operator !=( const Date & v){
        return (this->year != v.year || this->month != v.month || this->day != v.day);
    };
    bool operator <=( const Date & v){
        bool same =  (this->year == v.year && this->month == v.month && this->day == v.day);
        if(same)
            return true;
        return this->year < v.year || (this->year==v.year && this->month<v.month) || (this->year == v.year && this->month == v.month && this->day < v.day);
    };
    bool operator <( const Date & v){
        return this->year < v.year || (this->year==v.year && this->month<v.month) || (this->year == v.year && this->month == v.month && this->day < v.day);
    };
    TimeDiff operator -( const Date & v){
        int day2 = v.day;
        int day1 = this->day;
        int mon2 = v.month;
        int mon1 = this->month;
        int year2 = v.year;
        int year1 = this->year;
        if(day2 < day1)
        {
            // borrow days from february
            if (mon2 == 3)
            {
                //  check whether year is a leap year
                if ((year2 % 4 == 0 && year2 % 100 != 0) || (year2 % 400 == 0))
                    day2 += 29;
                else
                    day2 += 28;
            }
                // borrow days from April or June or September or November
            else if (mon2 == 5 || mon2 == 7 || mon2 == 10 || mon2 == 12)
                day2 += 30;
                // borrow days from Jan or Mar or May or July or Aug or Oct or Dec
            else
                day2 += 31;

            mon2 = mon2 - 1;
        }
        if (mon2 < mon1)
        {
            mon2 += 12;
            year2 -= 1;
        }
        return TimeDiff(year2-year1,mon2-mon1,day2-day1);
    };
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
    void accept(Visitor &v) override {
        v.visit(*this);
    }

};


class VariableAccess : public INode
{
public:
    std::string id;
    VariableAccess(std::string _id) : id(_id){};
    void accept(Visitor &visitor) override{
        visitor.visit(*this);
    };
};


class AssignStatement : public INode
{
public:
    std::unique_ptr<VariableAccess> var;
    std::unique_ptr<IExpression> assignable;
    AssignStatement(){};
    AssignStatement(std::unique_ptr<VariableAccess> _var, std::unique_ptr<IExpression> _assignable) : var(std::move(_var)), assignable(std::move(_assignable)){};
    void accept(Visitor &visitor) override
    {
        visitor.visit(*this);
    };
};
class VariableDeclr : public INode
{
public:
    std::string id;
    TypeOfData typeOfData;
    void accept(Visitor &visitor) override
    {
        visitor.visit(*this);
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
    void accept(Visitor &v) override {
        v.visit(*this);
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
    void accept(Visitor &v) override {
        v.visit(*this);
    }
};
class ParenthesisExpression : public IExpression
{
public:
    std::unique_ptr<IExpression> child;
    ParenthesisExpression(){};
    ParenthesisExpression(std::unique_ptr<IExpression> _child) : child(std::move(_child)){};
    void accept(Visitor &v) override {
        v.visit(*this);
    }
};
class Declaration : public INode
{
public:
    VariableDeclr var;
    std::unique_ptr<IExpression> assignable;
    Declaration(){};
    Declaration(VariableDeclr _var, std::unique_ptr<IExpression> _assignable) : var(_var), assignable(std::move(_assignable)) {}
    void accept(Visitor &visitor) override{
        visitor.visit(*this);
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
    void accept(Visitor &visitor) override{
        visitor.visit(*this);
    };
};
class FunCall : public INode
{
public:
    std::string id;
    std::vector<std::shared_ptr<IExpression>> arguments;
    FunCall(std::string _id, std::vector<std::shared_ptr<IExpression>> _arguments) : id(_id), arguments(std::move(_arguments)){
    };
    void accept(Visitor &v) override {
        v.visit(*this);
    }
};

class BasicExpression :public IExpression
{
public:
    std::unique_ptr<INode> basic;
    BasicExpression(){};
    BasicExpression(std::unique_ptr<INode> _basic, bool _wasMinus,bool _wasNegation) : basic(std::move(_basic)){wasMinus = _wasMinus;wasNegation = _wasNegation;};
    void accept(Visitor &v) override {
        v.visit(*this);
    }
};
class Return : public INode
{
public:
    std::unique_ptr<IExpression> expression;
    Return(){};
    Return(std::unique_ptr<IExpression> _expression) : expression(std::move(_expression)){};
    void accept(Visitor &v) override {
        v.visit(*this);
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
    void accept(Visitor &v) override {
        v.visit(*this);
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
    void accept(Visitor &v) override {
        v.visit(*this);
    }
};
class If : public INode
{
public:
    std::unique_ptr<IExpression> condition;
    std::unique_ptr<Body> body;
    If(){};
    If(std::unique_ptr<IExpression> _condition, std::unique_ptr<Body> _body) : condition(std::move(_condition)), body(std::move(_body)){};
    void accept(Visitor &v) override {
        v.visit(*this);
    }
};
class Else : public INode
{
public:
    std::unique_ptr<Body> body;
    Else(){};
    Else(std::unique_ptr<Body> _body) : body(std::move(_body)){};
    void accept(Visitor &v) override {
        v.visit(*this);
    }
};
class While : public INode
{
public:
    std::unique_ptr<IExpression> condition;
    std::unique_ptr<Body> body;
    While(){};
    While(std::unique_ptr<IExpression> _condition, std::unique_ptr<Body> _body) : condition(std::move(_condition)), body(std::move(_body)){};
    void accept(Visitor &v) override {
        v.visit(*this);
    }
};
class Body : public INode
{
public:
    std::vector<std::shared_ptr<INode>> statements; // declaration, assignStatement, FunCall, Return, While,

    Body(std::vector<std::shared_ptr<INode>>_statements) : statements(std::move(_statements)){};
    Body(){};

    void accept(Visitor &v) override {
        v.visit(*this);
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
    void accept(Visitor &visitor) override
    {
        visitor.visit(*this);
    };
};
class Program : public INode
{
public:
    std::vector<std::unique_ptr<Declaration>> declarations;
    std::vector<std::unique_ptr<Function>> functions;
    Program(){};
    Program(std::vector<std::unique_ptr<Declaration>> _declarations,std::vector<std::unique_ptr<Function>> _functions) : declarations(std::move(_declarations)),functions(std::move(_functions)){};
    void accept(Visitor &visitor) override
    {
        visitor.visit(*this);
    };
    std::vector<std::unique_ptr<Declaration>> getDeclarations(){return std::move(declarations);}
};

class PrintVisitor : public Visitor{
public:
    std::string debug;
    int indentation = 0;
    void visit(VariableDeclr &element) override
    {
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered VariableDeclr [";
        debug += "Type of data: " + TypeOfDataToString.at(static_cast<const int>(element.getTypeOfData())) + "," ;
        debug += "Id: " + element.getId()+"]\n";

    };
    void visit(Program &element) override
    {
        indentation = indentation +2;
        for(auto &declaration : element.declarations){
            if(declaration != nullptr)
                declaration.operator*().accept(*this);
        }
        debug += "----------------------------------------\n";
        for(auto &function : element.functions){
            if(function != nullptr)
                function.operator*().accept(*this);
        }
        indentation = indentation -2;
    };

    void visit(AdvExpression &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered AdvExpression [";
        if(element.wasNegation)
            debug += " Negation";
        if(element.wasMinus)
            debug +=", Minus";
        debug+="]\n";
        indentation = indentation +2;
        if(element.left != nullptr)
            element.left.operator*().accept(*this);
        debug += spaces+"Operator: " +element.operationType+"\n";
        if(element.right != nullptr)
            element.right.operator*().accept(*this);
        indentation = indentation -2;
    };
    void visit(Expression &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered Expression[";
        if(element.wasNegation)
            debug += " Negation";
        if(element.wasMinus)
            debug +=", Minus";
        debug+="]\n";
        indentation = indentation +2;
        if(element.left != nullptr)
            element.left.operator*().accept(*this);
        debug += spaces+"Operator: " +element.operationType+"\n";
        if(element.right != nullptr)
            element.right.operator*().accept(*this);
        indentation = indentation -2;
    };
    void visit(ParenthesisExpression &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered ParenthesisExpression[";
        if(element.wasNegation)
            debug += " Negation";
        if(element.wasMinus)
            debug +=", Minus";
        debug+="]\n";
        indentation = indentation +2;
        if(element.child != nullptr)
            element.child.operator*().accept(*this);
        indentation = indentation -2;
    };
    void visit(BasicExpression &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered BasicExpression [";
        if(element.wasNegation)
            debug += " Negation, ";
        if(element.wasMinus)
            debug +="Minus, ";
        indentation = indentation +2;
        if(element.basic != nullptr)
            element.basic->accept(*this);
        indentation = indentation -2;
    };
    void visit(Declaration &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered Declaration\n";
        indentation = indentation +2;
        visit(element.var);
        if(element.assignable != nullptr)
            element.assignable.operator*().accept(*this);
        indentation = indentation -2;
    };
    void visit(Variable &element) override{
        debug += "Entered Variable\n";
    };
    void visit(VariableAccess &element) override{
        debug+="Type = VariableAccess, Value = " + element.id +"]\n";
    };
    void visit(FunCall &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered FunCall [ name = "+element.id+",args:\n";
        indentation = indentation +2;
        for(long long unsigned int i = 0 ; i < element.arguments.size() ; i++){
            debug += spaces;
            if(element.arguments[i] != nullptr)
                element.arguments[i]->accept(*this);
        }
        debug += spaces+"]\n";
        indentation = indentation -2;
    };
    void visit(Function &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug+=spaces + "Function [ name = "+element.name+", returnType = "+TypeOfDataToString.at(static_cast<const int>(element.dataType))+", parameters:\n";
        indentation = indentation + 2;
        for(auto a : element.parameters)
            a.accept(*this);
        if(element.body != nullptr)
            element.body->accept(*this);
        indentation = indentation -2;
    };
    void visit(Body &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug+=spaces+",Body [\n";
        indentation = indentation +2;
        for(long long unsigned int i = 0 ; i < element.statements.size() ; i++){
            if(element.statements[i] != nullptr)
                element.statements[i]->accept(*this);
        }
        indentation = indentation -2;
    };
    void visit(AssignStatement &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug+= spaces+"Entered AssignStatement [ ";
        indentation = indentation +2;
        if(element.var != nullptr)
            element.var->accept(*this);
        if(element.assignable != nullptr)
            element.assignable->accept(*this);
        indentation = indentation -2;
    }
    void visit(Int &element) override{
        debug+="Type = Int, Value = " + std::to_string(element.value) +"]\n";
    };
    void visit(Double &element) override{
        debug+="Type = Float, Value = " + std::to_string(element.value) +"]\n";
    };
    void visit(Bool &element) override{
        std::string v = "";
        if(element.value)
            v = "True";
        else
            v = "False";
        debug+="Type = Bool, Value = " + v+"]\n";
    };
    void visit(String &element) override{
        debug+="Type = String, Value = " + element.value +"]\n";
    };
    void visit(Date &element) override{
        debug+="Type = Date, Value = " + element.toString() +"]\n";
    };
    void visit(TimeDiff &element) override{
        debug+="Type = TimeDiff, Value = " + element.toString() +"]\n";
    };
    void visit(Return &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered Return\n";
        indentation = indentation +2;
        if(element.expression != nullptr)
            element.expression->accept(*this);
        indentation = indentation -2;
    }
    void visit(While &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered While\n";
        indentation = indentation +2;
        if(element.condition != nullptr)
            element.condition->accept(*this);
        if(element.body != nullptr)
            element.body->accept(*this);
        indentation = indentation -2;
    };
    void visit(If &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered If\n";
        indentation = indentation +2;
        if(element.condition != nullptr)
            element.condition->accept(*this);
        if(element.body != nullptr)
            element.body->accept(*this);
        indentation = indentation -2;
    };
    void visit(Else &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered Else\n";
        indentation = indentation +2;
        if(element.body != nullptr)
            element.body->accept(*this);
        indentation = indentation -2;
    };
    void visit(Condition &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered Condition [";
        if(element.wasNegation)
            debug +=" Negation";
        if(element.wasMinus)
            debug +=", Minus";
        debug += "]\n";
        indentation = indentation +2;
        if(element.left != nullptr)
            element.left.operator*().accept(*this);
        debug += spaces+"Operator: " +element.operationType+"\n";
        if(element.right != nullptr)
            element.right.operator*().accept(*this);
        indentation = indentation -2;
    }
    void visit(RelationalCondition &element) override{
        std::string spaces;
        for(int i = 0; i < indentation; i++)
            spaces += " ";
        debug += spaces+"Entered RelationalCondition [";
        if(element.wasNegation)
            debug += " Negation";
        if(element.wasMinus)
            debug +=", Minus";
        debug+="]\n";
        indentation = indentation +2;
        if(element.left != nullptr)
            element.left.operator*().accept(*this);
        debug += spaces+"Operator: " +element.operationType+"\n";
        if(element.right != nullptr)
            element.right.operator*().accept(*this);
        indentation = indentation -2;
    }
};
#endif //TKOM_21Z_DANIEL_PROGRAM_H

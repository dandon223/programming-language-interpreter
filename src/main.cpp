#include <iostream>
#include <sstream>
#include "../modules/lexer/Lexer.h"
#include "../modules/parser/Parser.h"
#include "../modules/interpreter/Interpreter.h"

int main()
{
    std::filebuf ifst;
    ifst.open("../input.txt",std::ios::in);

    std::istream handle(&ifst);
    Lexer lexer = Lexer(handle);

    Parser parser = Parser(lexer);
    std::unique_ptr<Program> program = parser.TryToParseProgram();
    if(program){
        // printVisitor = PrintVisitor();
        //program->accept(printVisitor);
        //std::cout<< printVisitor.debug;

        Interpreter interpreter = Interpreter();
        program->accept(interpreter);
        std::cout<<interpreter.debug;
    }

    return 1;
}


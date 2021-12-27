#include <iostream>
#include <fstream>
#include <sstream>
#include "../modules/lexer/Lexer.h"
#include "../modules/parser/Parser.h"
#include "../modules/token/Token.h"
#include "../modules/interpreter/Interpreter.h"

//int main(int argc, char *argv[])
int main()
{
    std::filebuf ifst;
    ifst.open("../input.txt",std::ios::in);

    std::istream handle(&ifst);
    Lexer lexer = Lexer(handle);
//   while(!lexer.endOfFile())
//       std::cout << getStringToken(lexer.getNextToken()) <<"\n";
    Parser parser = Parser(lexer);
    std::unique_ptr<Program> program = parser.TryToParseProgram();
    if(program){
        PrintVisitor printVisitor = PrintVisitor();
        program->accept(printVisitor);
        std::cout<< printVisitor.debug;

        Interpreter interpreter = Interpreter();
        program->accept(interpreter);
        std::cout<<interpreter.debug;
    }

    return 1;
}


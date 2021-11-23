#include <iostream>
#include <fstream>
#include <sstream>
#include "../modules/lexer/Lexer.h"
#include "../modules/parser/Parser.h"
#include "../modules/token/Token.h"

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
    std::optional<Program> program = parser.TryToParseProgram();
    if(program){
        PrintVisitor *printVisitor = new PrintVisitor();
        program.value().accept(printVisitor);
        std::cout<< printVisitor->debug;
    }

    return 1;
}


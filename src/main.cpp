#include <iostream>
#include <fstream>
#include <sstream>
#include "../modules/lexer/Lexer.h"
#include "../modules/token/Token.h"

//int main(int argc, char *argv[])
int main()
{
    std::filebuf ifst;
    ifst.open("../input.txt",std::ios::in);

    std::istream handle(&ifst);
    Lexer lexer = Lexer(handle);
   while(!lexer.endOfFile())
       std::cout << getStringToken(lexer.getNextToken()) <<"\n";

    return 1;
}


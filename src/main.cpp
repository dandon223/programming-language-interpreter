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
//    while (handle.peek() != EOF)
//        std::cout << char(handle.get());
//    handle.seekg(std::ios_base::beg);
//    while (handle.peek() != EOF)
//        std::cout << char(handle.get());
//    ifst.close();
//
//    std::string text = "ABD\n\nDDD\n";
//    std::istringstream  handle2(text);
//
//    std::istream &h = handle2;
//    while (h.peek() != EOF)
//        std::cout << char(h.get());
//    h.seekg(std::ios_base::beg);
//    while (h.peek() != EOF)
//        std::cout << char(h.get());

    Lexer lexer = Lexer(handle);
    while(!lexer.endOfFile())
        std::cout << getStringToken(lexer.getNextToken()) <<"\n";
    return 1;
}




#include "Lexer.h"
Lexer::Lexer(std::istream &my_handle) : handle(my_handle){
}
void Lexer::getNextChar()
{
    char next_char = handle.get();
    if(next_char == '\n'){
        column = 0;
        line++;
        current_line_start = handle.tellg();
    }
    else if(next_char == '\t')
        column += 4;
    else
        column++;
    current_char = next_char;
}
char Lexer::peekNextChar(){
    return handle.peek();
}
void Lexer::printStream(){
    while(!handle.eof()){
        getNextChar();
        std::cout<<current_char<<" "<<"line: "<<line<<",column: "<<column<<"\n";
    }
}
bool Lexer::endOfFile()
{
    return handle.eof();
}
Token Lexer::getNextToken(){
    last_pos_after_good_token = handle.tellg();
    getNextChar();
    active_token.type = TokenType::Invalid;
    if(handle.peek()==EOF){
        active_token.type = TokenType::End_of_text;
        return active_token;
    }
    // nowa linia, musimy policzyć wcięcia
    if(current_char == '\n' || current_char == '\t' || current_char == ' '){
        if(getIndentation()){
            return active_token;
        }
    }
    return active_token;
}
bool Lexer::getIndentation(){
    active_token.type = TokenType::Invalid;
    int number_of_indentation = 0;
    if(current_char == '\t' ){
        column -= 4;
        handle.seekg(last_pos_after_good_token);
    }
    else if(current_char == ' '){
        column--;
        handle.seekg(last_pos_after_good_token);
    }
    while(peekNextChar() == ' ' || peekNextChar() == '\t'){

        // dla spacji potrzebujemy 4 pod rząd, inaczej błąd
        if(peekNextChar() == ' '){
            getNextChar();
            for(int i=0 ; i<3;i++){
                if(peekNextChar() != ' '){
                    std::cout << "Brak 4 spacji\n";
                    active_token.value =-1;
                    active_token.line_number = line;
                    active_token.column_number = column;
                    return false;
                }
                getNextChar();
            }
            number_of_indentation ++;
            // dla kazdego tab jedno wcięcie
        }else{
            number_of_indentation++;
            getNextChar();
        }
    }
    active_token.type = TokenType::Indentation;
    active_token.value = number_of_indentation;
    active_token.line_number = line;
    active_token.column_number = column;

    return true;
}
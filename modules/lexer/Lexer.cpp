

#include "Lexer.h"
Lexer::Lexer(std::istream &my_handle) : handle(my_handle){
    getNextChar();
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
    return end_of_file;
}
Token Lexer::getNextToken(){
    while(current_char == '\n'){
        last_pos_after_good_token = handle.tellg();
        getNextChar();
    }
    active_token.line_number = line;
    active_token.column_number = column;
    active_token.type = TokenType::Invalid;

    // nowa linia, musimy policzyć wcięcia
    if( current_char == '\t' ){
        if(getIndentation()){
            return active_token;
        }
    }
    if(current_char==EOF){
        active_token.type = TokenType::End_of_text;
        end_of_file= true;
        return active_token;
    }
    getNextChar();
    return active_token;
}
bool Lexer::getIndentation(){
    active_token.type = TokenType::Invalid;
    int number_of_indentation = 1;
    active_token.column_number = column -4;
    do{
        getNextChar();
        number_of_indentation ++;
    }while(current_char=='\t');

    active_token.type = TokenType::Indentation;
    active_token.value = number_of_indentation;
    return true;
}


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
    if(current_char == '\n' || !first_next_line){
        first_next_line = true;
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
    }
    ignore_comment();
    active_token.line_number = line;
    active_token.column_number = column;
    active_token.type = TokenType::Invalid;
    active_token.value = -1;


    if(current_char==EOF){
        active_token.type = TokenType::End_of_text;
        end_of_file= true;
        active_token.value = 0;
        return active_token;
    }
    if(getKeywordOrId())
        return active_token;
    getNextChar();
    return active_token;
}
void Lexer::ignore_comment(){
    if(current_char == '/' ){
        if(peekNextChar() == '/'){
            getNextChar();
            while(current_char != '\n' && current_char != EOF){
                getNextChar();
            }
            getNextChar();
        }
    }
}
bool Lexer::getKeywordOrId() {
    while(current_char ==' ')
        getNextChar();
    std::string word;
    if (!isalpha(current_char))
        return false;
    word += current_char;
    while (isalnum(peekNextChar()))
    {
        getNextChar();
        word += current_char;
    }
    if (map.count(word))
    {

        active_token.type = map.at(word);
        active_token.value = word;
        getNextChar();
        return true;
    }

    active_token.value = word;
    active_token.type = TokenType::Id;
    getNextChar();
    return true;
}
bool Lexer::getIndentation(){
    active_token.type = TokenType::Invalid;
    int number_of_indentation = 0;
    active_token.column_number = column -4;
    while(current_char=='\t'){
        getNextChar();
        number_of_indentation ++;
    }


    active_token.type = TokenType::Indentation;
    active_token.value = number_of_indentation;
    return true;
}
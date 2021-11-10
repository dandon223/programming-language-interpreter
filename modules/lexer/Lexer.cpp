

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
    std::cout<<current_char<<" "<<"line: "<<line<<",column: "<<column<<"\n";
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
    ignore_comment();

    active_token.type = TokenType::Invalid;
    active_token.value = "Invalid character.";
    active_token.line_number = line;
    active_token.column_number = column;

    if(getIndentation())
        return active_token;
    if(getKeywordOrId())
        return active_token;
    if(getOperators())
        return active_token;
    if(getString())
        return active_token;
    if(getNumber())
        return active_token;



    if(current_char==EOF){
        active_token.type = TokenType::End_of_text;
        end_of_file= true;
        active_token.value = 0;
        return active_token;
    }
    getNextChar();
    return active_token;
}
bool Lexer::ignore_comment(){
    if(current_char == '/' ){
        if(peekNextChar() == '/'){
            getNextChar();
            while(current_char != '\n' && current_char != EOF){
                getNextChar();
            }
            return true;
        }
    }
    return false;
}
bool Lexer::getDate(){
    //bool is_date = false;
   // bool is_timeDiff = false;
    if(current_char != '[')
        return false;
    //int first = 0;

    return true;
}
bool Lexer::getNumber() {
    active_token.type = TokenType::Invalid;
    std::string number;
    if(current_char =='.' && isdigit(peekNextChar())){
        getNextChar();
        while(isdigit(current_char))
            getNextChar();
        active_token.value = "No numbers before comma.";
        return true;
    }
    if (!isdigit(current_char))
        return false;
    number += current_char;
    getNextChar();
    if(number == "0" && isdigit(current_char)){
        active_token.value = "Leading zeros in number.";
        while(isdigit(current_char))
            getNextChar();
        if(current_char == '.'){
            getNextChar();
            while(isdigit(current_char))
                getNextChar();
        }
        return true;
    }
    while (isdigit(current_char)){
        number +=current_char;
        getNextChar();
    }
    if(current_char != '.'){
        try{
            int number_from_stoi = std::stoi(number);
            active_token.type = TokenType::Number;
            active_token.value = number_from_stoi;
            return true;
        }catch (const std::exception& out_of_range){
            active_token.value = "Value overflow.";
            return true;
        }
    }else{
        number +='.';
        getNextChar();
        while(isdigit(current_char)){
            number +=current_char;
            getNextChar();
        }
        try{
            double number_from_stod = std::stod(number);
            active_token.type = TokenType::Number;
            active_token.value = number_from_stod;
            return true;
        }catch (const std::exception& out_of_range){
            active_token.value = "Value overflow.";
            return true;
        }
    }
}
bool Lexer::getString() {
    std::string word;

    if(current_char == '\"'){
        getNextChar();
        while( current_char != EOF && ( current_char != '\"' || ( current_char=='\"' && word.back() == '\\' ) ) ){
            word +=current_char;
            getNextChar();
        }
        if(current_char == EOF){
            active_token.type = TokenType::Invalid;
            active_token.value = "No ending \" for string.";
            getNextChar();
            return true;
        }
        active_token.type = TokenType::String;
        active_token.value = word;
        getNextChar();
        return true;
    }
    return false;
}
bool Lexer::getKeywordOrId() {
    while(current_char ==' ' || current_char=='\t')
        getNextChar();
    active_token.column_number = column;
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
        while(current_char ==' ' || current_char =='\t')
            getNextChar();
        return true;
    }


    active_token.value = word;
    active_token.type = TokenType::Id;
    getNextChar();
    while(current_char ==' ' || current_char =='\t')
        getNextChar();
    return true;
}
bool Lexer::getOperators() {
    while (current_char == ' ' || current_char == '\t')
        getNextChar();
    std::string word;
    if ((current_char == '=' || current_char == '!' || current_char == '<' || current_char == '>') &&
        peekNextChar() == '=') {
        word += current_char;
        getNextChar();
        word += current_char;
        active_token.type = map.at(word);
        active_token.value = word;
        getNextChar();
        return true;
    }
    if (map.find(std::string(1,current_char)) == map.end())
        return false;

    active_token.type = map.at(std::string(1,current_char));
    active_token.value = std::string(1,current_char);
    getNextChar();
    return true;
}
bool Lexer::getIndentation(){

    if(current_char != '\n' && !first_line){
        return false;
    }
    while(current_char == '\n'){
        getNextChar();
        active_token.line_number = line;
    }

    while(ignore_comment()){
        getNextChar();
    }
    first_line = false;
    if(current_char == '\t')
        active_token.column_number = 0;
    while(current_char == '\n'){
        getNextChar();
        ignore_comment();
    }

    int number_of_indentation = 0;
    while(current_char=='\t'){
        getNextChar();
        number_of_indentation ++;
    }
    if(current_char == ' '){
        active_token.line_number = line;
        active_token.column_number = column;
        while(current_char == ' ')
            getNextChar();

        active_token.type = TokenType::Invalid;
        active_token.value = "spacebar after indentation";
        return true;
    }


    active_token.type = TokenType::Indentation;
    active_token.value = number_of_indentation;
    return true;
}


#include "Lexer.h"
#include "../structures/Date.h"
#include "../errorHandler/ErrorHandler.h"
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
    active_token.start_pos = handle.tellg();

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
    if(getDate())
        return active_token;
    if(getTimeDiff())
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
bool Lexer::getTimeDiff() {
    if(current_char !='{')
        return false;
    int year = 0;
    int month = 0;
    int day = 0;
    getNextChar();
    if(!isdigit(current_char)){
        active_token.value ="No digit after {";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    if(current_char == '0' && isdigit(peekNextChar())){
        active_token.value ="Leading zeros in number.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    for(int i=0 ; i <4 ;i++){
        if(isdigit(current_char)){
            year = year * 10 + current_char - '0';
            getNextChar();
        }
    }
    if(isdigit(current_char)){
        active_token.value ="Only up to 4 digits allowed for years.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    if(current_char != 'y'){
        active_token.value ="No 'y' after int.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    getNextChar();

    for(int i=0 ; i <2 ;i++){
        if(isdigit(current_char)){
            month = month * 10 + current_char - '0';
            getNextChar();
        }
    }
    if(isdigit(current_char)){
        active_token.value ="Only up to 2 digits allowed for months.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    if(current_char != 'm'){
        active_token.value ="No 'm' after int.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    getNextChar();

    for(int i=0 ; i <2 ;i++){
        if(isdigit(current_char)){
            day = day * 10 + current_char - '0';
            getNextChar();
        }
    }
    if(isdigit(current_char)){
        active_token.value ="Only up to 2 digits allowed for days.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    if(current_char != 'd'){
        active_token.value ="No 'd' after int.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    getNextChar();
    if(current_char != '}'){
        active_token.value ="No '}' after 'd'.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    getNextChar();

    TimeDiff timeDiff = TimeDiff(year,month,day);
    active_token.type = TokenType::TimeDiffValue;
    active_token.value = timeDiff;
    return true;
}
bool Lexer::getDate(){
    if(current_char != '[')
        return false;
    int year = 0;
    int month = 0;
    int day = 0;
    getNextChar();
    //while(current_char ==' ' || current_char =='\t'){
     //   getNextChar();
    //}
    if(!isdigit(current_char)){
        active_token.value ="No digit after [";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    for(int i=0 ; i <4 ;i++){
        if(!isdigit(current_char)){
            active_token.value ="Need 4 digits for year";
            ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
            getNextChar();
            return true;
        }
        year = year * 10 + current_char - '0';
        getNextChar();
    }
    if(isdigit(current_char)){
        active_token.value ="Only 4 digits for year";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        getNextChar();
        return true;
    }

    if(current_char!=':'){
        active_token.value ="No : after year.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        getNextChar();
        return true;
    }
    getNextChar();
    for(int i=0 ; i <2 ;i++){
        if(!isdigit(current_char)){
            active_token.value ="Need 2 digits for month";
            ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
            getNextChar();
            return true;
        }
        month = month * 10 + current_char - '0';
        getNextChar();
    }
    if(isdigit(current_char)){
        active_token.value ="Only 2 digits for month";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        getNextChar();
        return true;
    }
    if(current_char!=':'){
        active_token.value ="No : after month.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        getNextChar();
        return true;
    }
    getNextChar();
    for(int i=0 ; i <2 ;i++){
        if(!isdigit(current_char)){
            active_token.value ="Need 2 digits for day";
            ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
            getNextChar();
            return true;
        }
        day = day * 10 + current_char - '0';
        getNextChar();
    }
    if(isdigit(current_char)){
        active_token.value ="Only 2 digits for day.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        getNextChar();
        return true;
    }
    if(current_char!= ']'){
        active_token.value ="No ] after day";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        getNextChar();
        return true;
    }
    getNextChar();
    Date date = Date(year,month,day);
    active_token.type = TokenType::DateValue;
    active_token.value = date;
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
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    if (!isdigit(current_char))
        return false;
    number += current_char;
    getNextChar();
    if(number == "0" && isdigit(current_char)){
        active_token.value = "Leading zeros in number.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
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
            ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
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
            ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
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
            ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
            getNextChar();
            return true;
        }
        active_token.type = TokenType::StringValue;
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
        active_token.line_number = line;
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
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }


    active_token.type = TokenType::Indentation;
    active_token.value = number_of_indentation;
    return true;
}
std::string Lexer::getWholeLine(std::streampos &getFrom)
{
    const std::streampos curr_pos = handle.tellg();
    std::string line_local;
    handle.seekg(getFrom);
    std::getline(handle, line_local);
    handle.seekg(curr_pos);
    return line_local;
}
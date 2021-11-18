
#include <cmath>
#include "Lexer.h"
#include "../structures/Date.h"
#include "../errorHandler/ErrorHandler.h"
Lexer::Lexer(std::istream &my_handle) : handle(my_handle){
    active_token.start_pos = handle.tellg();
    getNextChar();
}
void Lexer::getNextChar()
{
    char next_char = handle.get();
    if(next_char == '\n'){
        column = 0;
        line++;
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
bool Lexer::endOfFile(){
    return end_of_file;
}
Token Lexer::getNextToken(){
    ignore_comment();

    active_token.type = TokenType::Invalid;
    active_token.value = "Invalid character.";
    active_token.line_number = line;
    active_token.column_number = column;
    active_token.length = 0;
    if(!first_line)
        active_token.start_pos = handle.tellg();

    if(parseIndentation())
        return active_token;
    if(parseKeywordOrId())
        return active_token;
    if(parseOperators())
        return active_token;
    if(parseString())
        return active_token;
    if(parseNumber())
        return active_token;
    if(parseDate())
        return active_token;
    if(parseTimeDiff())
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
bool Lexer::parseTimeDiff() {
    int length = 0;
    if(current_char !='{')
        return false;
    length++;
    int year = 0;
    int month = 0;
    int day = 0;
    getNextChar();
    length++;
    if(!isdigit(current_char)){
        active_token.value ="No digit after {";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    if(current_char == '0' && isdigit(peekNextChar())){
        active_token.value ="Leading zeros in number after {.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        while(isdigit(current_char))
            getNextChar();
        return true;
    }
    for(int i=0 ; i <4 ;i++){
        if(isdigit(current_char)){
            length++;
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
    length++;
    getNextChar();
    if(current_char == '0' && isdigit(peekNextChar())){
        active_token.value ="Leading zeros in number after y.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        while(isdigit(current_char))
            getNextChar();
        return true;
    }
    for(int i=0 ; i <2 ;i++){
        if(isdigit(current_char)){
            length++;
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
    length++;
    if(current_char == '0' && isdigit(peekNextChar())){
        active_token.value ="Leading zeros in number after m.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        while(isdigit(current_char))
            getNextChar();
        return true;
    }
    for(int i=0 ; i <2 ;i++){
        if(isdigit(current_char)){
            length++;
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
    length++;
    if(current_char != '}'){
        active_token.value ="No '}' after 'd'.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    getNextChar();

    TimeDiff timeDiff = TimeDiff(year,month,day);
    active_token.type = TokenType::TimeDiffValue;
    active_token.value = timeDiff;
    active_token.length = length;
    return true;
}
bool Lexer::parseDate(){
    int length = 0;
    if(current_char != '[')
        return false;
    length++;
    int year = 0;
    int month = 0;
    int day = 0;
    getNextChar();
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
        length++;
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
    length++;
    getNextChar();
    for(int i=0 ; i <2 ;i++){
        if(!isdigit(current_char)){
            active_token.value ="Need 2 digits for month";
            ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
            getNextChar();
            return true;
        }
        length++;
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
    length++;
    getNextChar();
    for(int i=0 ; i <2 ;i++){
        if(!isdigit(current_char)){
            active_token.value ="Need 2 digits for day";
            ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
            getNextChar();
            return true;
        }
        length++;
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
    length++;
    getNextChar();
    Date date = Date(year,month,day);
    active_token.type = TokenType::DateValue;
    active_token.value = date;
    active_token.length = length;
    return true;
}
bool Lexer::parseNumber() {
    int length = 0;
    active_token.type = TokenType::Invalid;
    double tempNumber = 0;
    if(current_char =='.' && isdigit(peekNextChar())){
        active_token.value = "No numbers before comma.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        getNextChar();
        return true;
    }
    if (!isdigit(current_char))
        return false;
    tempNumber = current_char - '0';
    length++;
    getNextChar();
    if(tempNumber == 0 && isdigit(current_char)){
        active_token.value = "Leading zeros in number.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        getNextChar();
        return true;
    }

    while(isdigit(current_char)){
        if (tempNumber * 10 + 1 > std::numeric_limits<double>::max() / 10)
        {
            active_token.value = "Value overflow.";
            ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
            return true;
        }
        length++;
        tempNumber = tempNumber * 10 + current_char - '0';
        getNextChar();
    }
    if(isalpha(current_char)){
        active_token.value = "Char after number.";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        return true;
    }
    if(current_char != '.'){
        if (tempNumber > std::numeric_limits<int>::max())
        {
            active_token.value = "Int value overflow.";
            ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
            return true;
        }
        active_token.type = TokenType::Number;
        active_token.value = (int) tempNumber;
        active_token.length = length;
        return true;
    }else{
        int positions_after_comma = 0;
        getNextChar();
        length++;
        while(isdigit(current_char)){
            if (tempNumber * 10 + 1 > std::numeric_limits<double>::max() / 10)
            {
                active_token.value = "Double value overflow.";
                ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
                return true;
            }
            length++;
            positions_after_comma++;
            tempNumber = tempNumber * 10 + current_char - '0';
            getNextChar();
        }
        tempNumber = tempNumber / std::pow(10,positions_after_comma);
        active_token.type = TokenType::Number;
        active_token.length = length;
        active_token.value =tempNumber;
        return true;
    }
}
bool Lexer::parseString() {
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
        active_token.length = word.length();
        getNextChar();
        return true;
    }
    return false;
}
bool Lexer::parseKeywordOrId() {
    while(current_char ==' ' || current_char=='\t'){
        active_token.start_pos = handle.tellg();
        getNextChar();
    }
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
        active_token.length = word.size();
        getNextChar();
        while(current_char ==' ' || current_char =='\t')
            getNextChar();
        return true;
    }

    active_token.value = word;
    active_token.type = TokenType::Id;
    active_token.length = word.size();
    getNextChar();
    while(current_char ==' ' || current_char =='\t'){
        active_token.start_pos = handle.tellg();
        getNextChar();
    }

    return true;
}
bool Lexer::parseOperators() {
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
        active_token.length = 2;
        getNextChar();
        return true;
    }
    if (map.find(std::string(1,current_char)) == map.end())
        return false;

    active_token.type = map.at(std::string(1,current_char));
    active_token.value = std::string(1,current_char);
    active_token.length = 1;
    getNextChar();
    return true;
}
bool Lexer::parseIndentation(){

    if(current_char != '\n' && !first_indentation){
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
    if(line >1)
        first_line = false;
    if(current_char == '\t')
        active_token.column_number = 0;
    while(current_char == '\n'){
        getNextChar();
        ignore_comment();
    }
    if(current_char == EOF)
        return false;

    int number_of_indentation = 0;
    while(current_char=='\t'){
        active_token.start_pos = handle.tellg();
        getNextChar();
        number_of_indentation ++;
    }
    if(current_char == ' '){
        active_token.line_number = line;
        active_token.column_number = column;
        while(current_char == ' ' || current_char == '\t'){
            getNextChar();
        }

        active_token.type = TokenType::Invalid;
        active_token.value = "spacebar after indentation";
        ErrorHandler::printLexerError(active_token,getWholeLine(active_token.start_pos));
        first_indentation = false;
        return true;
    }

    active_token.type = TokenType::Indentation;
    active_token.value = number_of_indentation;
    active_token.length = number_of_indentation*4;
    first_indentation = false;
    return true;
}
std::string Lexer::getWholeLine(std::streampos &getFrom)
{
    const std::streampos curr_pos = handle.tellg();
    std::string line_local;
    handle.seekg(getFrom);
    std::getline(handle, line_local);
    handle.seekg(curr_pos );
    return line_local;
}
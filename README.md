# Implementation of an interpreter for a general-purpose programming language with a built-in data type representing a date.

## Language description
<p align="justify">
This language draws inspiration from both Python and C++. Variable scopes and the association of statements with blocks and functions are determined by indentation, as is typical in languages like Python. This eliminates the need for unnecessary braces. On the other hand, variables must be explicitly typed upon definition to ensure greater code readability.
</p>

## Language Behavior Rules
* Supports integers and floating-point numbers
  * Supports mathematical operations with varying execution precedence
  * Supports logical operations and comparisons with varying precedence
* Supports the character (string) type
  * Allows string concatenation only with other strings
  * Strings may contain any characters, including the delimiter (" ") itself
* Supports the bool type
* Supports comparison operators for two dates
* Introduces a timeDiff type, resulting from subtracting a smaller date from a larger one
* Allows writing comments in the language
* Supports variable creation, assignment, and reading
  * Static typing
  * Weak typing for int and float
  * Variables are mutable
  * Variables have local scopes
* Supports conditional loops with if else
* Supports basic while loop
* Allows defining and calling custom functions with local variables, where arguments are passed by value
* Supports recursive function calls
* Built-in print function that accepts a single string-type argument

## Project structure

* The project is written in C++
* It will be a graphical application where a script can be provided for interpretation
* Testing will be done using unit tests with the Boost library
* Modules:
  * Lexical Analyzer Module, Reads a stream of characters and produces tokens as requested by the syntax analyzer; Detects invalid tokens and signals them to the error handling module
  * Syntax Analyzer Module, Requests tokens from the lexical analyzer and constructs parse trees, Detects syntax errors and signals them to the error handling module
  * Semantic Analyzer Module, Checks the constructed parse trees to ensure they make semantic sense, Detects errors such as using the + operator with incompatible types, Executes the validated program
  * Error Handling Module, When parsing fails, provides the user with a list of detected errors
  * Identifier Table Module,  Stores and manages all identifiers, including those reserved by the language; Used by the lexical, syntax, and semantic analyzer modules

## Grammar

```
program             ::= ( functionDefinition | declaration )*
functionDefinition  ::= ’fun’ type id ’(’ parameters ’)’ ’:’ body
parameters          ::= empty
                      | type id (’,’ type id)*
body                ::= statement+
statement           ::= assignment
                      | if
                      | while
                      | declaration
                      | return
                      | functionCall
assignment          ::= id ’=’ expression
return              ::= ’return’ expression?
declaration         ::= type id
                      | type id ’=’ expression
if                  ::= ’if’ condition ’:’body (’else:’ body)?
while               ::= ’while’ condition ’:’ body
condition           ::= relationalCondition (logicalOperator, relationalCondition )*
relationalCondition ::= basicCondition, (relationalOperator, basicCondition )*
basicCondition      ::= negCondition
                      | ’(’ condition’)’
                      | expression
negCondition        ::= ’!’ ( ’(’ condition’)’ | expression)
expression          ::= advancedExpression ([+-] advancedExpression)*
advancedExpression  ::= basicExpression ([*/] advancedExpression)*
basicExpression     ::= ’"’ allchar* ’"’
                      | timeDiff
                      | date
                      | ’-’? number
                      | bool
                      | ’-’? id
                      | ’-’? ’(’ expression ’)’
                      | ’-’? functionCall
functionCall        ::= id ’(’ arguments ’)’
arguments           ::= empty
                      | expression (’,’ expression)*
id                  ::= letter (digit | letter)*
number              ::= int
                      | float
bool                ::= ’True’
                      | ’False’
int                 ::= nonzeroDigit digit*
                      | ’0’
float               ::= int ’.’ digit+
digit               ::= nonzeroDigit
                      | ’0’
nonzeroDigit        ::= [1-9]
relationOperator    ::= ’==’
                      | ’!=’
                      | ’<’
                      | ’>’
                      | ’>=’
                      | ’<=’
logicalOperator     ::= ’and’
                      | ’or’
letter              ::= [A-Za-z_]
timeDiff            ::= {int’y’int’m’int’d’}
type                ::= ’int’
                      | ’float’
                      | ’date’
                      | ’timeDiff’
                      | ’string’
                      | ’bool’
allchar             ::= ?all characters?
date                ::= ’[’ [0-9]{4}’:’ [0-9]{2} ’:’ [0-9]{2} ’]’
empty               ::= ’’

```

## Code example
```
1)
fun int start():
  string napis = "A"
  if napis =="A":
    print("Tak")
  else:
  int zmienna = 1
  while(zmienna <5):
    print(i)
  zmienna = zmienna +1
  return 0

2)
fun int eatNumber(int x):
  int result
  if(x >0):
    result = eatNumber(x-1)
  return result+1

fun int start():
  return eatNumber(5)

3)
fun int start():
  date a = (1999:05:05)
  if [2000:01:01] - a > {0y1m0d}:
    print("Większa")
  else:
    print("Mniejsza")
  return 1
```

## Project compilation

### Kompilacja 
<p align="justify">
To compile the project, run the make.sh script located in the root directory of the project.
</p>

### Running

<p align="justify">
To run the previously built project, navigate to the build subdirectory and launch the generated program from the terminal. The input file is input.txt, located one directory level above.
</p>

### Testing

<p align="justify">
The tests were written using the Boost library. To run them, use the build script make.sh located in the tests subfolder. Then, in the build folder, you will find the test executable. Run it to execute the tests.
</p>

## Instruction to use

### Variables 

<p align="justify">
Variables can be declared both globally and within functions. Variable declaration is as follows. <type> <name> = <arithemtic operation> Available types are : int, float, string, bool, date, timeDiff. When declaring variables, you don't have to provide a value immediately.  (without = <arithemtic operation> part).
Examples:
</p>
  
* int a =1
* float b = (1.1 + 2)*3
* string c = ”napis”
* bool g = True
* date d = [2020:12:12]
* timeDiff e = {1y1m1d}
  
<p align="justify">
The character ” can also appear in the string by preceding it with the character \, such as ”przy\”example”. When specifying a time difference, the number of years, months, and days should always be given, even if it is 0.
</p>
  
### arithmetic expressions
  
<p align="justify">
  Basic operations can be performed on variables of type int and float: addition, subtraction, multiplication, division. The correct order of operations is maintained, i.e. first multiplication and division, then addition and subtraction. The order of operations can be changed using parentheses. A string variable can be concatenated with any other variable, e.g. by adding a data variable to it.

</p>
  
```
string a = ”now is: ”+[2022:01:16]
```
  
<p align="justify">
You can subtract the date from date to get the time difference, but the date you subtract must be the smaller date. You can also subtract the time difference from the date to get the smaller date.

</p>
 
### comparisons and logical expressions

<p align="justify">
The language has basic comparison operators ”==” - equal, ”!=” - different, ”<” - less, ”>” - greater, ”<=” - not greater, ”>=” - not less. The language provides classic logical operators: negation ”!”, conjunction ”and”, and disjunction ”or”. They can only be used when the left and right sides are of type bool.
</p>
  
### if else statement

<p align="justify">
  The if statement here is a typical statement of this type for programming languages.
</p>

```
if True:
  return 1
```

<p align="justify">
The else statement is optional and must appear immediately after the completed if statement block.
</p>

```
if True:
  return 1
else:
  return 2
```

<p align="justify">
The condition in the if statement can take on a more complex form. You can use both logical expressions, comparisons, and arithmetic expressions. For example:
</p>

```
if (1 - 2)*2
```

### While loop

<p align="justify">
  Like the if statement, this is a typical while loop construct in programming languages.
</p>

```
int a = 5
while a > 1:
  a = a - 1
```

### Functions

<p align="justify">
 The user can create their own functions. They must start with the word "fun" and have zero indentation
in the document. The "main" function cannot have parameters. Each function must return a declared type
of variable. Function parameters are given after a comma together with their type, e.g.: int a.
Functions accept arguments as copies of values, so they are not overwritten outside the function to which they were
given.
</p>

```
fun <type> <name> (<parameters>) :
  function body
```
  
### built-in function

<p align="justify">
  The built-in function "print" is available to the user. It takes one argument of type string, which
prints to the screen. In order to output variables of other types than string, you must force a change of type,
by concatenating with an empty string.
</p>

```
int a = 1
print("" +a)
```

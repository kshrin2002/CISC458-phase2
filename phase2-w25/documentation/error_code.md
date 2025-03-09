Parser overview

The parser processes input code and converts it into an Abstract Syntax Tree (AST), following correct syntax and structure. It supports variable declarations, assignments, mathematical expressions, control statements and function calls. It also implements error handling for syntax errors.


Features

Factorial Function Support 
• Enables function calls specifically for calculating the factorial of a number.
• Function calls follow the syntax: function name followed by parentheses containing the argument.
• The 'factorial' function call must be written as: factorial (expression);
• Eg: factorial (5);
Block Scoping - 
• Supports grouping of multiple statements within a block.
• Blocks are defined using curly braces { and }.
• Variables declared within a block are scoped to that block.
• Eg:
{
int x;
x = 42;
}
Enhanced Runtime Error Detection -
• Improves error reporting by including both line and column numbers.
• Lexer errors now report the exact column where an error occurs.
• Parser errors display precise locations for unexpected tokens and missing syntax elements.
• Eg: “Parse Error at line 2, column 16: Unexpected token '!'”



Language Syntax Supported

Grammar rules

Variable declarations 
Starts with the keyword int, followed by a variable name and ends with a semicolon
Eg: int x;  int y;

Assignments
A variable is assigned a value using =
The value can be a number, another variable or an expression
Eg: x = 5;  y = x + 10;

Expressions 
Expressions support +, -, *, /
Parentheses () can be used for grouping
Eg: z = 10 + 3 * 2; a = (10 + 3) * 2;

Conditional statements (if)
Starts with if
Condition is placed inside parentheses()
Body in {}
Eg: 
	if (x > 0) {
			print x;
		}
Loops

While loop
Starts with while
Condition inside parentheses()
Body in {}
Eg:
	while (x < 10) {
		x = x + 1;
	}

Repeat - Until loop
Starts with repeat 
Body in {}
Ends with until (condition)
Eg: 
repeat {
	x = x - 1;
} until (x == 0);

Function calls
Function name followed by ()
Arguments go inside parentheses
Ends with a semicolon
Eg: 
factorial (5);
print factorial (3);

Error handling strategy

Lexer errors

ERROR_INVALID_CHAR
Detection: Unrecognized character
Handling strategy: Report error and continue

ERROR_CONSECUTIVE OPERATORS
Detection: Two consecutive operators (eg. ++, - -)
Handling strategy: Report error and skip second operator

ERROR_UNEXPECTED TOKEN
Detection: Unexpected token encountered
Handling strategy: Report token and continue

ERROR_INVALID_NUMBER
Detection: Invalid number format
Handling strategy: Report error 

ERROR_INVALID_IDENTIFIER
Detection: Invalid identifier used
Handling strategy: Report error and continue


Parser errors

PARSE_ERROR_UNEXPECTED_TOKEN
Detection: Encountered an unexpected token
Handling strategy: Report error and stop parsing

PARSE_ERROR_MISSING_SEMICOLON
Detection: Missing semicolon (;)
Handling strategy: Report error and mention where the semicolon is missing

PARSE_ERROR_MISSING_IDENTIFIER
Detection: Declaration without identifier
Handling strategy: Report error

PARSE_ERROR_MISSING_EQUALS
Detection: Missing =  in assignment
Handling strategy: Report error

PARSE_ERROR_INVALID_EXPRESSION
Detection: Invalid expression syntax
Handling strategy: Report error

PARSE_ERROR_MISSING PARENTHESES
Detection: Unmatched ( or )
Handling strategy: Report missing parentheses

PARSE_ERROR_MISSING_CONDITION
Detection: If/while/repeat statements missing condition
Handling strategy: Report error

PARSE_ERROR_INVALID_OPERATOR
Detection: Invalid use of operators
Handling strategy: Report error

PARSE_ERROR_FUNCTION_CALL_NO_ARGUMENTS
Detection: Function call missing arguments 
Handling strategy: Report error

PARSE_ERROR_FUNCTION_CALL_INVALID ARGUMENT
Detection: Function call with too many arguments
Handling strategy: Report error

PARSE_ERROR_FUNCTION_TOO_MANY_ARGUMENTS
Detection: Function call with too many arguments
Handling strategy: Report error

PARSE_ERROR_FUNCTION_UNDEFINED
Detection: Undefined function call
Handling strategy: Report error


AST structure

Node Type
Description
AST_PROGRAM
Root node of the AST
AST_VARDECL
Variable declaration (int x;)
AST_ASSIGN
Assignment (x = 5;)
AST_NUMBER
Number (42)
AST_IDENTIFIER
Variable name (x,y)
AST_IF
If statement (if (x > 0) { … })
AST_WHILE
While loop (while (x < 10) { … })
AST_REPEAT
Repeat-until loop
AST_BLOCK
Block of statements ({ statement1; statement2; })
AST_FUNCTIONCALL
Function call (factorial (x))
AST_BINOP
Binary operation (x + y, x * y)


Test cases and expected outputs

Valid cases

Case 1:
int x;
int y;

Case 2:
x = 42;
y = x + 5;

Case 3:
z = 10 + 3 * 2;
a = (10 + 3) * 2;

Case 4:
if (x > 0) {
    print x;
}

Case 5:
while (y < 100){
    y = y * 2;
}

Case 6:
repeat {
    z = z - 1;
} until (z == 0);

Case 7:
factorial (5);
print factorial(3);

Invalid cases

Case 1:
int x
x = 5

Case 2:
x 5 = 3;

Case 3:
x = (5 + 3;

Case 4:
if (x > ){
    print x;
}

Case 5:
while x > 0{
    print x;
}

Case 6:
int ;

Case 7:
y = 3 ++ 2;

Case 8: 
factorial 5;



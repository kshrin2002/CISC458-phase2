/* parser.c */
#include <stdio.h>
#include <stdlib.h>
#include "../../include/parser.h"
#include "../../include/lexer.h"
#include "../../include/tokens.h"
#include "../../src/lexer/lexer.c"

// TODO 1: Add more parsing function declarations for:
// - if statements: if (condition) { ... }
// - while loops: while (condition) { ... }
// - repeat-until: repeat { ... } until (condition)
// - print statements: print x;
// - blocks: { statement1; statement2; }
// - factorial function: factorial(x)

//Added by Shrinidhi
static ASTNode* parse_if_statement(void);
static ASTNode* parse_while_statement(void);
static ASTNode* parse_repeat_statement(void);
static ASTNode* parse_print_statement(void);
static ASTNode* parse_block(void);
static ASTNode* parse_factorial(void);

static ASTNode *parse_statement(void);
static ASTNode *parse_expression(void);
// End of added

// Current token being processed
static Token current_token;
static int position = 0;
static const char *source;


static void parse_error(ParseError error, Token token) {
    // TODO 2: Add more error types for: - done
    // - Missing parentheses
    // - Missing condition
    // - Missing block braces
    // - Invalid operator
    // - Function call errors

    printf("Parse Error at line %d: ", token.line);
    switch (error) {
        case PARSE_ERROR_UNEXPECTED_TOKEN:
            printf("Unexpected token '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_MISSING_SEMICOLON:
            printf("Missing semicolon after '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_MISSING_IDENTIFIER:
            printf("Expected identifier after '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_MISSING_EQUALS:
            printf("Expected '=' after '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_INVALID_EXPRESSION:
            printf("Invalid expression after '%s'\n", token.lexeme);
            break;

        // Part of TODO 2: -dharsan (expand)
        /*
        You must improve or build upon these error messages!
        */
        case PARSE_ERROR_MISSING_L_PARENTHESIS:
            printf("Missing opening parenthesis after '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_MISSING_R_PARENTHESIS:
            printf("Missing closing parenthesis for expression starting with '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_MISSING_CONDITION:
            printf("Missing condition after '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_MISSING_L_BRACE:
            printf("Missing opening brace '{' after '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_MISSING_R_BRACE:
            printf("Missing closing brace '}' for block starting with '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_INVALID_OPERATOR:
            printf("Invalid operator '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_FUNCTION_CALL_NO_ARGUMENTS:
            printf("Function '%s' called with no arguments but requires some\n", token.lexeme);
            break;
        case PARSE_ERROR_FUNCTION_CALL_INVALID_ARGUMENT:
            printf("Invalid argument in call to function '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_FUNCTION_CALL_TOO_MANY_ARGUMENTS:
            printf("Too many arguments in call to function '%s'\n", token.lexeme);
            break;
        case PARSE_ERROR_FUNCTION_UNDEFINED:
            printf("Call to undefined function '%s'\n", token.lexeme);
            break;
        default:
            printf("Unknown error\n");
    }
}

// Get next token
static void advance(void) {

    printf("%s\n", current_token.lexeme);
    current_token = get_next_token(source, &position);
}

// Create a new AST node
static ASTNode *create_node(ASTNodeType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node) {
        node->type = type;
        node->token = current_token;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

// Match current token with expected type
static int match(TokenType type) {
    return current_token.type == type;
}

// Expect a token type or error
static void expect(TokenType type) {
    if (match(type)) {
        advance();
    } else {
        parse_error(PARSE_ERROR_UNEXPECTED_TOKEN, current_token);
        exit(1); // Or implement error recovery
    }
}


// Added by Shrinidhi
// TODO 3: Add parsing functions for each new statement type

// If statement parsing: if (condition) statement
static ASTNode* parse_if_statement(void) { 
    ASTNode *node = create_node(AST_IF);
    advance();
    expect(TOKEN_LPAREN); // expect left parantheses
    node -> left = parse_expression(); // condition 
    expect(TOKEN_RPAREN); // closing right parantheses
    node -> right = parse_statement(); // statement after if
    return node;

}

// While loop parsing: while (condition) statement
static ASTNode* parse_while_statement(void) { 
    ASTNode *node = create_node(AST_WHILE);
    advance();
    expect(TOKEN_LPAREN);
    node -> left = parse_expression(); // condition 
    expect(TOKEN_RPAREN);
    node -> right = parse_statement(); // loop body 
    return node;

}

// Repeat-until loop parsing: repeat statement until (condition)
static ASTNode* parse_repeat_statement(void) { 
    ASTNode *node = create_node(AST_REPEAT);
    advance();
    node -> left = parse_statement(); 
    if (!match(TOKEN_UNTIL)){
        parse_error(PARSE_ERROR_UNEXPECTED_TOKEN, current_token);
        exit(1);
    }
    advance();
    expect(TOKEN_LPAREN);
    node -> right = parse_expression(); // condition
    expect(TOKEN_RPAREN);
    return node;

}

// Print statement parsing: print expression
static ASTNode* parse_print_statement(void) { 
    ASTNode *node = create_node(AST_PRINT);
    advance();
    node -> left = parse_expression(); 
    if (!match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_MISSING_SEMICOLON,current_token);
        exit(1);
    }
    advance();
    return node;
}

// Block parsing
static ASTNode* parse_block(void) { 
    expect(TOKEN_LBRACE); //left opening brace
    ASTNode *block_node = create_node(AST_BLOCK); // create new ast node for the block

    ASTNode *current = NULL; // helpful for chaining statements
    // now, we parse the statements until we hit the closing brace.
    while (!match(TOKEN_RBRACE) && current_token.type != TOKEN_EOF) {
        // single statement parsing
        ASTNode *statement = parse_statement();
        // now if we are looking at the first statement in the block, attatch it as the left child.
        if (block_node -> left == NULL) {
            block_node -> left = statement;
        } else {
            // if we are not looking at the first statement, chain the statements together.
            current -> right = statement;
        }
        // update the last statement to the current statement
        current = statement;
    }
    // eat the closing brace
    expect(TOKEN_RBRACE);
    return block_node;
}

// Factorial function call parsing
static ASTNode* parse_factorial(void) { 
     ASTNode *node = create_node(AST_FUNCTIONCALL);
     node -> token = current_token;
     advance();
     expect(TOKEN_LPAREN);
     node -> left = parse_expression();
     expect(TOKEN_RPAREN);
     return node;

}


// Parse variable declaration: int x;
static ASTNode *parse_declaration(void) {
    ASTNode *node = create_node(AST_VARDECL);
    advance(); // consume 'int'

    if (!match(TOKEN_IDENTIFIER)) {
        parse_error(PARSE_ERROR_MISSING_IDENTIFIER, current_token);
        exit(1);
    }

    node->token = current_token;
    advance();

    if (!match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
        exit(1);
    }
    advance();
    return node;
}

// Parse assignment: x = 5;
static ASTNode *parse_assignment(void) {
    ASTNode *node = create_node(AST_ASSIGN);
    node->left = create_node(AST_IDENTIFIER);
    node->left->token = current_token;
    advance();

    if (!match(TOKEN_EQUALS)) {
        parse_error(PARSE_ERROR_MISSING_EQUALS, current_token);
        exit(1);
    }
    advance();

    node->right = parse_expression();

    if (!match(TOKEN_SEMICOLON)) {
        parse_error(PARSE_ERROR_MISSING_SEMICOLON, current_token);
        exit(1);
    }
    advance();
    return node;
}

// Parse statement
static ASTNode *parse_statement(void) {
    if (match(TOKEN_INT)) {
        return parse_declaration();
    } else if (match(TOKEN_IDENTIFIER)) {
        return parse_assignment();
    } else if (match(TOKEN_LBRACE)) { // new case for left brace - yash
        return parse_block();
    }

    // TODO 4: Add cases for new statement types
    // else if (match(TOKEN_IF)) return parse_if_statement();
    // else if (match(TOKEN_WHILE)) return parse_while_statement();
    // else if (match(TOKEN_REPEAT)) return parse_repeat_statement();
    // else if (match(TOKEN_PRINT)) return parse_print_statement();
    // ...

    printf("Syntax Error: Unexpected token\n");
    exit(1);
}

// Parse expression (currently only handles numbers and identifiers)

// TODO 5: Implement expression parsing
// Current expression parsing is basic. Need to implement:
// - Binary operations (+-*/)
// - Comparison operators (<, >, ==, etc.)
// - Operator precedence
// - Parentheses grouping
// - Function calls

// static ASTNode *parse_expression(void) {
//     ASTNode *node;
//
//     if (match(TOKEN_NUMBER)) { // Numbers
//         node = create_node(AST_NUMBER);
//         advance();
//     } else if (match(TOKEN_IDENTIFIER)) { // Identifiers
//         node = create_node(AST_IDENTIFIER);
//         advance();
//     } else if (match(TOKEN_LPAREN)) { // Parentheses grouping
//         advance();
//         node = parse_expression();
//         expect(TOKEN_RPAREN);
//     } else if (match(TOKEN_OPERATOR)) { // Binary operations (+-*/)
//         node = create_node(AST_BINOP);
//         node->token = current_token;
//         advance();
//         node->left = parse_expression();
//         node->right = parse_expression();
//     } else {
//         printf("Syntax Error: Expected expression\n");
//         exit(1);
//     }
//
//     return node;
// }

static ASTNode *parse_expression_prec(int);

static ASTNode *parse_primary(void) {
    if (match(TOKEN_NUMBER)) { // If the current token is a number
        ASTNode *node = create_node(AST_NUMBER);
        advance();
    } else if (match(TOKEN_IDENTIFIER)) {
        // check if the identifier is a 'factorial' function call
        if (strcmp(current_token.lexeme,"factorial") == 0) {
            // call factorial function parsing
            return parse_factorial();
            //node = parse_factorial();
        } else {
            // parse identifier
            ASTNode *node = create_node(AST_IDENTIFIER);
            return node;
        }
    } else if (match(TOKEN_IDENTIFIER)) { // If the token is an identifier
        ASTNode *node = create_node(AST_IDENTIFIER);
        advance();

        if (match(TOKEN_LPAREN)) { // If the identifier is followed by '(', it's a function call
            node->type = AST_FUNCTIONCALL;
            advance();
            node->args = parse_expression(); // Parse function arguments
            expect(TOKEN_RPAREN);
        }

        return node;
    } else if (match(TOKEN_LPAREN)) { // If the token is '(', parse a parenthesized expression
        advance();
        ASTNode *node = parse_expression_prec(0); // Start parsing with the lowest precedence
        expect(TOKEN_RPAREN);
        return node;
    }

    printf("Syntax Error: Expected primary expression\n");
    exit(1);
}

#include <string.h> // for strcmp

static int get_precedence(Token op) {
    if (strcmp(op.lexeme, "+") == 0 || strcmp(op.lexeme, "-") == 0) {
        return 1;  // Addition and subtraction (low precedence)
    } else if (strcmp(op.lexeme, "*") == 0 || strcmp(op.lexeme, "/") == 0) {
        return 2;  // Multiplication and division (higher precedence)
    } else if (strcmp(op.lexeme, "<") == 0 || strcmp(op.lexeme, ">") == 0 || strcmp(op.lexeme, "==") == 0) {
        return 0; // Comparison operators (lowest precedence)
    } else {
        return -1; // Unknown operator
    }
}

static ASTNode *parse_expression_prec(int min_precedence) {
    ASTNode *lhs = parse_primary();

    while (match(TOKEN_OPERATOR) && get_precedence(current_token) >= min_precedence) {
        Token op = current_token; // Store the current operator
        int precedence = get_precedence(op);
        // ASTNode *rhs = parse_expression_prec(precedence); // Parse the right-hand side (RHS) with correct precedence
        advance();

        // Fix: Correct precedence handling
        ASTNode *rhs = parse_expression_prec(precedence + 1); // Enforce correct precedence

        ASTNode *node = create_node(AST_OPERATOR);
        node->token = op;
        node->left = lhs;
        node->right = rhs;
        lhs = node;
    }

    return lhs;
}

/* Parses an expression starting with the lowest precedence */
static ASTNode *parse_expression(void) {
    return parse_expression_prec(0);
}


// Parse program (multiple statements)
static ASTNode *parse_program(void) {
    ASTNode *program = create_node(AST_PROGRAM);
    ASTNode *current = program;

    while (!match(TOKEN_EOF)) {
        current->left = parse_statement();
        if (!match(TOKEN_EOF)) {
            current->right = create_node(AST_PROGRAM);
            current = current->right;
        }
    }

    return program;
}

// Initialize parser
void parser_init(const char *input) {
    source = input;
    position = 0;
    advance(); // Get first token
}

// Main parse function
ASTNode *parse(void) {
    return parse_program();
}

// Print AST (for debugging)
void print_ast(ASTNode *node, int level) {
    if (!node) return;

    // Indent based on level
    for (int i = 0; i < level; i++) printf("  ");

    // Print node info
    switch (node->type) {
        case AST_PROGRAM:
            printf("Program\n");
            break;
        case AST_VARDECL:
            printf("VarDecl: %s\n", node->token.lexeme);
            break;
        case AST_ASSIGN:
            printf("Assign\n");
            break;
        case AST_NUMBER:
            printf("Number: %s\n", node->token.lexeme);
            break;
        case AST_IDENTIFIER:
            printf("Identifier: %s\n", node->token.lexeme);
            break;

        //yashcases
        case AST_FUNCTIONCALL:
            printf("FunctionCall: %s\n", node->token.lexeme);
            break;

        // TODO 6: Add cases for new node types
        // case AST_IF: printf("If\n"); break;
        // case AST_WHILE: printf("While\n"); break;
        // case AST_REPEAT: printf("Repeat-Until\n"); break;
        // case AST_BLOCK: printf("Block\n"); break;
        case AST_BINOP:
            printf("BinaryOp: %s\n", node->token.lexeme);
            break;
        case AST_COMP:
            printf("Comparison: %s\n", node->token.lexeme);
            break;
        case AST_FUNCTIONCALL:
            printf("FunctionCall: %s\n", node->token.lexeme);
        break;
        case AST_OPERATOR:
            printf("Operator: %s\n", node->token.lexeme);
        break;
        default:
            printf("Unknown node type\n");
    }

    // Print children
    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);
}

// Free AST memory
void free_ast(ASTNode *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free(node);
}

// Main function for testing
int main() {
    // // Test with both valid and invalid inputs
    // const char *input = "int x;\n" // Valid declaration
    //         "x = 42;\n"; // Valid assignment;
    // // TODO 8: Add more test cases and read from a file:
    // const char *invalid_input = "int x;\n"
    //                             "x = 42;\n"
    //                             "int ;";


    //printf("Parsing input:\n%s\n", invalid_input);
    //parser_init(invalid_input);


    // Test invalid inputs
    char input[4096];
    ASTNode *ast;
    FILE *file;
    file = fopen("tests/input_invalid.txt", "r");
    if (file) {
        printf("\nParsing invalid test cases...\n");
        fread(input, sizeof(char), sizeof(input) - 1, file);
        fclose(file);

        parser_init(input);
        ast = parse();
        printf("\nAbstract Syntax Tree:\n");
        print_ast(ast, 0);
        free_ast(ast);
    }

    printf("\nAbstract Syntax Tree:\n");
    print_ast(ast, 0);

    free_ast(ast);


    // Test valid inputs
    file = fopen("tests/input_valid.txt", "r");
    if (file) {
        printf("Parsing valid test cases...\n");
        fread(input, sizeof(char), sizeof(input) - 1, file);
        fclose(file);

        parser_init(input);
        ASTNode *ast = parse();
        printf("\nAbstract Syntax Tree:\n");
        print_ast(ast, 0);
        free_ast(ast);
    return 0;


}
}
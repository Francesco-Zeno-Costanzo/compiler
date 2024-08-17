#ifndef LEXER_H
#define LEXER_H

/* Type that lexer can recognize */
typedef enum {
    TOKEN_BEGINNING,   // Start of the file
    TOKEN_IDENTIFIER,  // Identifier e.g. name of variable
    TOKEN_SEPARATOR,   // Parentesis or semicolon
    TOKEN_NUMBER,      // An integer number
    TOKEN_OPERATOR,    // Operartor (+, -, *, /, =)
    TOKEN_COMP,        // Comparison (like fortran neq, eq, less, greater)
    TOKEN_STRING,      // String
    TOKEN_KEYWORD,     // Key word like while, print and so on
    TOKEN_EOF,         // End of source file
} TokenType;

/* Struct of the token */
typedef struct {
    TokenType type;   // Type of token    
    char *value;      // Relative vaule
    size_t line_num;  // At which line
} Token;


/* Functions */
void print_token(Token token);
Token *generate_number(char *current, int *current_index);
Token *generate_string(char *current, int *current_index);
Token *generate_keyword_or_identifier(char *current, int *current_index);
Token *generate_separator_or_operator(char *current, int *current_index, TokenType type);
Token *lexer(FILE *file);


#endif


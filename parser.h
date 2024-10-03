#ifndef PARSER_H
#define PARSER_H

/* Structure of a node of the AST */
typedef struct ASTNode {
    Token token;                 // Current node
    struct ASTNode *left;        // left node
    struct ASTNode *right;       // right node
} ASTNode;

ASTNode *parse_expression(Token **tokens);
ASTNode *parse_declaration_or_assignment(Token **tokens);
ASTNode *parse_while(Token **tokens);
ASTNode *parse_function_call(Token **tokens);
ASTNode *parse_block(Token **tokens);
ASTNode *parse(Token *tokens);
void print_ast(ASTNode *node, int indent);


#endif
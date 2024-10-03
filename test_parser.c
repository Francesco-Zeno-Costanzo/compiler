#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"  // Include the lexer
#include "parser.h" // Include the parser

int main() {

    // Open a file
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Errore nell'aprire il file.\n");
        return 1;
    }

    // Genera i token usando il lexer
    Token *tokens = lexer(file);

    // Esegui il parser sui token generati
    ASTNode *ast = parse(tokens);

    // Stampa l'albero sintattico (AST)
    printf("Albero Sintattico (AST):\n");
    print_ast(ast, 0);

    // Libera la memoria usata per i token e l'AST
    free(tokens);
    free(ast);

    return 0;
}

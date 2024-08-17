#include<stdio.h>
#include <stdlib.h>

#include "lexer.h" 

extern size_t line_number;  // External declaration for global variable

int main() {

    // Open a file
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Errore nell'aprire il file.\n");
        return 1;
    }

    // Call the lexer
    Token *tokens = lexer(file);

    // Print token
    size_t i = 0;
    while (tokens[i].type != TOKEN_EOF) {
        print_token(tokens[i]);
        i++;
    }
    
    // Free tokens
    free(tokens);
    
    return 0;
}

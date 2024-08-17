#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "lexer.h"


size_t line_number = 0;  // line of source file

void print_token(Token token){
  printf("TOKEN VALUE: ");
  printf("'");
  for(int i = 0; token.value[i] != '\0'; i++){
    printf("%c", token.value[i]);
  }
  printf("'");
  printf("\nline number: %zu", token.line_num);

  switch(token.type){
    case TOKEN_NUMBER:
      printf(" TOKEN TYPE: INT\n");
      break;
    case TOKEN_KEYWORD:
      printf(" TOKEN TYPE: KEYWORD\n");
      break;
    case TOKEN_SEPARATOR:
      printf(" TOKEN TYPE: SEPARATOR\n");
      break;
    case TOKEN_OPERATOR:
      printf(" TOKEN TYPE: OPERATOR\n");
      break;
    case TOKEN_IDENTIFIER:
      printf(" TOKEN TYPE: IDENTIFIER\n");
      break;
    case TOKEN_STRING:
      printf(" TOKEN TYPE: STRING\n");
      break;
    case TOKEN_COMP:
      printf(" TOKEN TYPE: COMPARATOR\n");
      break;
    case TOKEN_EOF:
      printf(" END OF TOKENS\n");
      break;
    case TOKEN_BEGINNING:
      printf("BEGINNING\n");
      break;
  }
}


Token *generate_number(char *current, int *current_index){
    // Function to generate the token for numbers
    
    // Initialization of the token 
    Token *token    = malloc(sizeof(Token));
    //token->line_num = malloc(sizeof(size_t));
    token->line_num = line_number;
    token->type     = TOKEN_NUMBER;
    char *value     = malloc(sizeof(char) * 8);
    int value_index = 0;

    // While we read a number
    while(isdigit(current[*current_index]) && current[*current_index] != '\0'){
        if(!isdigit(current[*current_index])){
        break;
        }
        value[value_index] = current[*current_index];
        value_index++;
        *current_index += 1;
    }
    value[value_index] = '\0';
    token->value = value;
    return token;
}


Token *generate_string(char *current, int *current_index){
    // Function to generate the token for strings

    // Initializzation
    Token *token    = malloc(sizeof(Token));
    //token->line_num = malloc(sizeof(size_t));
    token->line_num = line_number;
    char *value     = malloc(sizeof(char) * 64);
    int value_index = 0;
    *current_index += 1;

    // Untile the string is finished
    while(current[*current_index] != '"'){
        value[value_index] = current[*current_index];
        value_index++;
        *current_index += 1;
    }
    value[value_index] = '\0';
    token->type  = TOKEN_STRING;
    token->value = value;
    return token;
}

Token *generate_keyword_or_identifier(char *current, int *current_index){
    // Function to create the token for keyword and for identifier
    // if there are no keywords it means that I am defining the variable

    // Initializzation
    Token *token      = malloc(sizeof(Token));
    //token->line_num   = malloc(sizeof(size_t));
    token->line_num   = line_number;
    char *keyword     = malloc(sizeof(char) * 8);
    int keyword_index = 0;
    
    // while we read a word in the code
    while(isalpha(current[*current_index]) && current[*current_index] != '\0'){
        keyword[keyword_index] = current[*current_index];
        keyword_index++;
        *current_index += 1;
    }
    keyword[keyword_index] = '\0';

    // check for the right token
    if(strcmp(keyword, "exit") == 0){            // Stop the program
        token->type  = TOKEN_KEYWORD;
        token->value = "EXIT";
    } 
    else if(strcmp(keyword, "int") == 0){        // Declaration of integre variable
        token->type  = TOKEN_KEYWORD;
        token->value = "INT";
    } 
    else if(strcmp(keyword, "if") == 0){         // If statement
        token->type  = TOKEN_KEYWORD;
        token->value = "IF";
    } 
    else if(strcmp(keyword, "while") == 0){      // While loop
        token->type  = TOKEN_KEYWORD;
        token->value = "WHILE";
    } 
    else if(strcmp(keyword, "print") == 0){      // Print function
        token->type  = TOKEN_KEYWORD;
        token->value = "PRINT";
    } 
    else if(strcmp(keyword, "eq") == 0){         // Logic equal ==
        token->type  = TOKEN_COMP;
        token->value = "EQ";
    } 
    else if(strcmp(keyword, "ne") == 0){         // Logic not equal !=
        token->type  = TOKEN_COMP;
        token->value = "NEQ";
    } 
    else if(strcmp(keyword, "lt") == 0){         // Logic less than <
        token->type  = TOKEN_COMP;
        token->value = "LESS";
    } 
    else if(strcmp(keyword, "gt") == 0){         // Logic greater than >
        token->type  = TOKEN_COMP;
        token->value = "GREATER";
    } 
    else {
        token->type  = TOKEN_IDENTIFIER;         // Assign a variable
        token->value = keyword;
    }
    
    return token;
}


Token *generate_separator_or_operator(char *current, int *current_index, TokenType type){
    // Function to create the token for separator and operator
    // same function because all of the are one symbol (i.e. +, -, (, ecc... )
    // the symbol is the value of the token

    Token *token    = malloc(sizeof(Token));
    token->value    = malloc(sizeof(char) * 2);
    token->value[0] = current[*current_index];
    token->value[1] = '\0';
    //token->line_num = malloc(sizeof(size_t));
    token->line_num = line_number;
    token->type     = type;

    return token;
}

size_t tokens_index;


Token *lexer(FILE *file){
    int length;
    char *current = 0;

    fseek(file, 0, SEEK_END); // Move the pointer to the end of the file
    length = ftell(file);     // Compute the total length of the file
    fseek(file, 0, SEEK_SET); // Returns to the beginning of the file

    // Read all the file
    current = malloc(sizeof(char) * length);
    fread(current, 1, length, file);
    fclose(file);

    // Add a terminator
    current[length] = '\0';
    // Index of the file 
    int current_index = 0;

    int number_of_tokens = 12;  // Maximum number of token, will change dynamically
    int tokens_size      = 0;   // Number of tokens currently created
    Token *tokens        = malloc(sizeof(Token) * number_of_tokens);
    tokens_index         = 0;

    // Until End Of File
    while(current[current_index] != '\0'){
        Token *token = malloc(sizeof(Token));
        tokens_size++;

        // If I exceed the limit, I increase the size of the array containing the tokens
        if(tokens_size > number_of_tokens){
        number_of_tokens *= 1.5; // Increase of 50 %
        tokens = realloc(tokens, sizeof(Token) * number_of_tokens);
        }

        if(current[current_index] == ';'){
        token = generate_separator_or_operator(current, &current_index, TOKEN_SEPARATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        } 
        else if(current[current_index] == ','){
        token = generate_separator_or_operator(current, &current_index, TOKEN_SEPARATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        } 
        else if(current[current_index] == '('){
        token = generate_separator_or_operator(current, &current_index, TOKEN_SEPARATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        } 
        else if(current[current_index] == ')'){
        token = generate_separator_or_operator(current, &current_index, TOKEN_SEPARATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        } 
        else if(current[current_index] == '{'){
        token = generate_separator_or_operator(current, &current_index, TOKEN_SEPARATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        } 
        else if(current[current_index] == '}'){
        token = generate_separator_or_operator(current, &current_index, TOKEN_SEPARATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        } 
        else if(current[current_index] == '='){
        token = generate_separator_or_operator(current, &current_index, TOKEN_OPERATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        } 
        else if(current[current_index] == '+'){
        token = generate_separator_or_operator(current, &current_index, TOKEN_OPERATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        }
        else if(current[current_index] == '-'){
        token = generate_separator_or_operator(current, &current_index, TOKEN_OPERATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        } 
        else if(current[current_index] == '*'){
        token = generate_separator_or_operator(current, &current_index, TOKEN_OPERATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        } 
        else if(current[current_index] == '/'){
        token = generate_separator_or_operator(current, &current_index, TOKEN_OPERATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        } 
        else if(current[current_index] == '%'){
        token = generate_separator_or_operator(current, &current_index, TOKEN_OPERATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        } 
        else if(current[current_index] == '"'){
        token = generate_string(current, &current_index);
        tokens[tokens_index] = *token;
        tokens_index++;
        }
        else if(isdigit(current[current_index])){
        token = generate_number(current, &current_index); 
        tokens[tokens_index] = *token;
        tokens_index++;
        current_index--;
        }
        else if(isalpha(current[current_index])){
        token = generate_keyword_or_identifier(current, &current_index);
        tokens[tokens_index] = *token;
        tokens_index++;
        current_index--;
        }
        else if(current[current_index] == '\n'){
        line_number += 1;
        } 
        free(token);
        current_index++;
    }

    // End of file
    tokens[tokens_index].value = '\0';
    tokens[tokens_index].type  = TOKEN_EOF;
    
    return tokens;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "parser.h"


ASTNode *create_node(Token token){
    // Function to create a node of the tree
    ASTNode *node = malloc(sizeof(ASTNode));
    node->token   = token;
    node->left    = NULL;
    node->right   = NULL;
    return node;
}


ASTNode *parse_declaration_or_assignment(Token **tokens){
    // Function for parsing assignment or variable declaration

    printf("Parsing declaration or assignment with token: %s\n", (*tokens)->value);
    Token token = **tokens;
    (*tokens)++;  // go to the next token

    // Declaration of a variable
    if (token.type == TOKEN_KEYWORD && strcmp(token.value, "INT") == 0){
        // New variable
        if ((**tokens).type == TOKEN_IDENTIFIER){
            Token var_token = **tokens;
            printf("Variable identifier found: %s\n", var_token.value);
            (*tokens)++;  // go to the next token

            // Check for the =
            if ((**tokens).type == TOKEN_OPERATOR && strcmp((**tokens).value, "=") == 0){
                printf("Assignment operator '=' found\n");
                (*tokens)++;  // go to the next token

                // Parsing of the expression
                ASTNode *value_node = parse_expression(tokens);

                // Check for the ;
                if ((**tokens).type == TOKEN_SEPARATOR && strcmp((**tokens).value, ";") == 0) {
                    printf("Found ';' after assignment\n");
                    (*tokens)++;  // go to the next token
                }
                else{
                    printf("Error: expected ;\n");
                    exit(EXIT_FAILURE);
                }

                // Node for the variable
                ASTNode *var_node = create_node(var_token);
                var_node->right = value_node;
                return var_node;
            }
            else{
                printf("Error: expecting =\n");
                exit(EXIT_FAILURE);
            }
        }
        else{
            printf("Error: expected variable after 'int'\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (token.type == TOKEN_IDENTIFIER){
        // Handle an existing variable
        Token var_token = token;
        printf("Variable reuse found: %s\n", var_token.value);

        if ((**tokens).type == TOKEN_OPERATOR && strcmp((**tokens).value, "=") == 0){
            printf("Assignment operator '=' found\n");
            (*tokens)++;  // go to the next token

            // Parsing the expression
            ASTNode *value_node = parse_expression(tokens);

            // Check for the ;
            if ((**tokens).type == TOKEN_SEPARATOR && strcmp((**tokens).value, ";") == 0){
                printf("Found ';' after assignment\n");
                (*tokens)++; // go to the next token
            }
            else{
                printf("Errore: expected ;\n");
                exit(EXIT_FAILURE);
            }

            ASTNode *var_node = create_node(var_token);
            var_node->right = value_node;  // Assign the expression
            return var_node;
        }
        else{
            printf("Errore: expected =");
            exit(EXIT_FAILURE);
        }
    } 
    else{
        printf("Error: invalid declaration or assignement\n");
        exit(EXIT_FAILURE);
    }
}



ASTNode *parse_expression(Token **tokens){
    // Function for the parsing of the expression
    printf("Parsing expression starting with token: %s\n", (*tokens)->value);

    Token left_token = **tokens;
    (*tokens)++;  // go to the next token

    // Check in the next token is an operator
    if ((**tokens).type == TOKEN_OPERATOR) {
        Token operator_token = **tokens;
        printf("Operator found: %s\n", operator_token.value);
        (*tokens)++;  // go to the next token

        Token right_token = **tokens;
        printf("Right-hand expression token: %s\n", right_token.value);
        (*tokens)++;  // go to the next token

        // Creation of nodes for the entire expression
        ASTNode *left_node     = create_node(left_token);
        ASTNode *operator_node = create_node(operator_token);
        ASTNode *right_node    = create_node(right_token);

        // Creation of the tree
        operator_node->left  = left_node;
        operator_node->right = right_node;

        return operator_node;
    }
    else{
        // Simple case: assignemet
        return create_node(left_token);
    }
}


ASTNode *parse_function_call(Token **tokens){
    // Parsing for calling a function
    printf("Parsing function call with token: %s\n", (*tokens)->value);
    
    Token func_token = **tokens;
    (*tokens)++;  // go to the next token

    // Check for open parentesis, the argumente of the function
    if ((**tokens).type == TOKEN_SEPARATOR && strcmp((**tokens).value, "(") == 0) {
        (*tokens)++;  // go to the next token

        ASTNode *argument_node = create_node(**tokens);  // Token for the argument
        printf("Function argument: %s\n", (*tokens)->value);
        (*tokens)++;  // go to the next token

        // Check for close parentesis, end function argument
        if ((**tokens).type == TOKEN_SEPARATOR && strcmp((**tokens).value, ")") == 0) {
            (*tokens)++;  // go to the next token

            ASTNode *func_node = create_node(func_token);
            func_node->right = argument_node;  // Assign the argument at the function
            return func_node;
        }
        else{
            printf("Error: Expected ')' but found: %s\n", (*tokens)->value);
            exit(EXIT_FAILURE);
        }
    }
    else{
        printf("Error: Expected '(' but found: %s\n", (*tokens)->value);
        exit(EXIT_FAILURE);
    }
}


ASTNode *parse_while(Token **tokens){
    // Function for the parsing of the while loop

    (*tokens)++;  // The current token is while so we go to the next
    
    // Check for open parentesis for the logical condition
    if ((**tokens).type == TOKEN_SEPARATOR && strcmp((**tokens).value, "(") == 0){
        (*tokens)++;  // go to the next token
        
        // Parsing of the condition (es: x ge 0)
        // Node for the LHS
        ASTNode *condition_left = create_node(**tokens);
        printf("While condition left side: %s\n", (*tokens)->value);
        (*tokens)++;  // go to the next token
        
        // Now we check the several logical comparisons
        if ((**tokens).type == TOKEN_IDENTIFIER && (strcmp((**tokens).value, "ge") == 0 ||
                                                    strcmp((**tokens).value, "le") == 0 ||
                                                    strcmp((**tokens).value, "eq") == 0)){
            
            Token comp_op_token = **tokens;  // ge, le, eq
            printf("Comparison operator found: %s\n", comp_op_token.value);
            (*tokens)++;  // go to the next token
            
            // Node for the RHS of the condition
            ASTNode *condition_right = create_node(**tokens);
            printf("While condition right side: %s\n", (*tokens)->value);
            (*tokens)++;  // go to the next token
            
            // Node for the full condtion
            ASTNode *condition = create_node(comp_op_token);
            condition->left = condition_left;
            condition->right = condition_right;
            
            // Check for the close parentesis, end of the condition
            if ((**tokens).type == TOKEN_SEPARATOR && strcmp((**tokens).value, ")") == 0){
                (*tokens)++;  // go to the next token
                
                // Check for the { for the operations in while loop
                if ((**tokens).type == TOKEN_SEPARATOR && strcmp((**tokens).value, "{") == 0){
                    (*tokens)++;  // go to the next token
                    
                    // Parsing of the while's instructions
                    ASTNode *block = parse_block(tokens);

                    // Parsing the whole while loop
                    ASTNode *while_node = create_node((Token){.type = TOKEN_KEYWORD, .value = "WHILE"});
                    while_node->left = condition;
                    while_node->right = block;
                    return while_node;
                }
            }
        }
    }
    printf("Errore: invalid while loop\n");
    exit(EXIT_FAILURE);
}


ASTNode *parse_block(Token **tokens){
    // Parsing of code blocks (e.g. contained inside {})

    // Initializzation
    ASTNode *block = NULL;
    ASTNode *last_statement = NULL;
    
    // Looop until the end of the block: }
    while ((**tokens).type != TOKEN_SEPARATOR || strcmp((**tokens).value, "}") != 0){
        ASTNode *statement = NULL;
        
        // Check for the keywords while or functions
        if ((**tokens).type == TOKEN_KEYWORD){
            if (strcmp((**tokens).value, "WHILE") == 0){
                statement = parse_while(tokens);
            } 
            else if (strcmp((**tokens).value, "PRINT") == 0 || strcmp((**tokens).value, "EXIT") == 0){
                statement = parse_function_call(tokens);
            }
            else{
                printf("Error: unknown keyword \n");
                exit(EXIT_FAILURE);
            }
        }
        // otherwise chek for other keyword or assignament or identifier
        else if ((**tokens).type == TOKEN_IDENTIFIER || (**tokens).type == TOKEN_KEYWORD){
            statement = parse_declaration_or_assignment(tokens);
        }
        // If none of the above
        else{
            printf("Errore: invalid token \n");
            exit(EXIT_FAILURE);
        }

        // If statement is NULL, it means that no valid statement was recognized, so an error is generated
        if (!statement){
            printf("Errore: invalid instruction\n");
            exit(EXIT_FAILURE);
        }

        // if Null we set as first
        if (!block){
            block = statement;
        }
        // otherwise on the right like always
        else{
            last_statement->right = statement;
        }
        last_statement = statement;

        // Before looking for the ';', check if the next token is a '}'
        if ((**tokens).type == TOKEN_SEPARATOR && strcmp((**tokens).value, "}") == 0){
            break;  // If it is '}', we exit the loop without looking for the ';'
        }

        // Check for ;
        if ((**tokens).type == TOKEN_SEPARATOR && strcmp((**tokens).value, ";") == 0){
            (*tokens)++;  // go to the next token
        }
        else{
            printf("Errore: missing ;\n");
            exit(EXIT_FAILURE);
        }
    }

    // Check for }
    if ((**tokens).type == TOKEN_SEPARATOR && strcmp((**tokens).value, "}") == 0){
        (*tokens)++; // go to the next token
    }
    else{
        printf("Errore: missing }\n");
        exit(EXIT_FAILURE);
    }

    return block;
}


// FUnction to parse all source code
ASTNode *parse(Token *tokens){

    // Initializzation of the tree
    Token *current_token = tokens;
    ASTNode *root        = NULL;
    
    // Until End Of File
    while (current_token->type != TOKEN_EOF) {
        printf("Parsing token: %s (type: %d)\n", current_token->value, current_token->type);

        // Ignore the token for ; beacuse it is only a separator
        if (current_token->type == TOKEN_SEPARATOR && strcmp(current_token->value, ";") == 0){
            current_token++;  // go to the next token
            continue;
        }

        // Parse while statement
        if (current_token->type == TOKEN_KEYWORD && strcmp(current_token->value, "WHILE") == 0){
            // Call the function to parse the while loop
            ASTNode *while_node = parse_while(&current_token);

            // If the root is empty we set the first node
            if (root == NULL) {
                root = while_node;
            }
            // Otherwise we add the node on the right (each subsequent statement is linked to the right)
            else{
                ASTNode *temp = root;
                while (temp->right != NULL){
                    temp = temp->right;
                }
                temp->right = while_node;
            }
        }
        // Parse keywords for functions (only two for the moment)
        else if (current_token->type == TOKEN_KEYWORD && (strcmp(current_token->value, "PRINT") == 0 ||
                                                          strcmp(current_token->value, "EXIT") == 0)){
            
            // Call the function to parse the built-in functions
            ASTNode *func_node = parse_function_call(&current_token);

            // Then like before
            if (root == NULL){
                root = func_node;
            }
            else{
                ASTNode *temp = root;
                while (temp->right != NULL){
                    temp = temp->right;
                }
                temp->right = func_node;
            }
        } 
        // Parse other keyword or identifier (in our case will be assignaments or declarations)
        else if (current_token->type == TOKEN_KEYWORD || current_token->type == TOKEN_IDENTIFIER){
            
            // Call the function to parse assignaments or declarations
            ASTNode *declaration_or_assign = parse_declaration_or_assignment(&current_token);
            
            // As always
            if (root == NULL){
                root = declaration_or_assign;
            }
            else{
                ASTNode *temp = root;
                while (temp->right != NULL){
                    temp = temp->right;
                }
                temp->right = declaration_or_assign;
            }
        }
        else{
            printf("Error: unknown token %s\n", current_token->value);
            exit(EXIT_FAILURE);
        }
    }

    // Return all the tree
    return root;
}

// Function to print the tree
void print_ast(ASTNode *node, int indent){
    if (node == NULL) return;
    for (int i = 0; i < indent; i++) printf("  ");
    printf("%s\n", node->token.value);
    print_ast(node->left, indent + 1);
    print_ast(node->right, indent + 1);
}

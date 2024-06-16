/*****************************************************************
Simple code to make an interpreter.
Only the four operetion are implemented and only between integers.
Usage:
gcc easy_interpreter.c -o easyint -Wall -pedantic -O3
./easyint <executable_name> 
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_VARS 256

typedef struct {
    char name;
    int value;
} Variable;

Variable variables[MAX_VARS];
int var_count    = 0;
int current_line = 0;

/* Function to avoid text that starts with # (comment) */
void remove_comment(char *line){
    char *commentStart = strchr(line, '#');
    if (commentStart){
        *commentStart = '\0';  // Terminate the string at the start of the comment
    }
}

/* Function to set the vaule of a variable */
void set_variable(char name, int value){

    /* Search if the variable already exist */
    for (int i = 0; i < var_count; i++){
        if (variables[i].name == name){
            variables[i].value = value;
            return;
        }
    }

    /* Otherwise create the variable */
    variables[var_count].name = name;
    variables[var_count].value = value;
    var_count++;
}

/* Function that return the value of a variable */
int get_variable(char name) {
    for (int i = 0; i < var_count; i++) {
        if (variables[i].name == name) {
            return variables[i].value;
        }
    }
    printf("Error: Undefined variable %c at line %d \n", name, current_line);
    exit(1);
}

/* Function to evaluate the lines of source code*/
void evaluate_line(char *line) {
    
    char var, op, var1, var2;
    int value;

    /* Assignament */
    if (sscanf(line, " %c = %d", &var, &value) == 2){
        set_variable(var, value);
    }
    /* Operation */
    else if (sscanf(line, " %c = %c %c %c", &var, &var1, &op, &var2) == 4){
        int val1 = get_variable(var1);
        int val2 = get_variable(var2);

        switch (op){
            case '+': value = val1 + val2; break;
            case '-': value = val1 - val2; break;
            case '*': value = val1 * val2; break;
            case '/': value = val1 / val2; break;
            default:
                printf("Error: Unsupported operator %c at line %d \n", op, current_line);
                exit(1);
        }
        set_variable(var, value);
    }
    /* Print function */
    else if (sscanf(line, " print %c", &var) == 1){
        printf("%d\n", get_variable(var));
    }
    /* Error otherwise */
    else {
        printf("Error: Invalid syntax at line %d \n", current_line);
        exit(1);
    }
}

int main(int argc, char *argv[]){

    if (argc != 2){
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file){
        perror("Could not open source file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];

    /* Read line by line */
    while (fgets(line, sizeof(line), file)){

        current_line += 1;
        
        /* Remove comment */
        remove_comment(line);

        /* Ignore empty lines */
        if (line[0] == '\0' || line[0] == '\n' || line[0] == ' '){
            continue;
        }
        evaluate_line(line);
    }

    fclose(file);
    return 0;
}

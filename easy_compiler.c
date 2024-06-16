/*****************************************************************
Simple code to make a compilator.
Only the four operetion are implemented and only between integers.
Usage:
chmod +x build_c.sh     | To build the compiler
./build_c.sh
chmod +x build_e.sh     | To build the executable
./build_e.sh <file_name_souce_code> <executable_name>
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE_LENGTH 1024
#define MAX_LINE_LENGTH 128

/* Structure for variable assignament
   variable = value */
typedef struct {
    char variable;
    int value;
} Assignment;

/* Structure for binary operation
   result = item1 operator item2 */
typedef struct {
    char result;
    char item1;
    char operator;
    char item2;
} Operation;

/* Function to avoid text that starts with # (comment) */
void remove_comment(char *line){
    char *commentStart = strchr(line, '#');
    if (commentStart){
        *commentStart = '\0';  // Terminate the string at the start of the comment
    }
}

/* Function to analize lines where assignament is made*/
void parse_assignment(char *line, Assignment *assignment){
    sscanf(line, " %c = %d", &assignment->variable, &assignment->value);
}

/* Function to analize lines where operation is made*/
void parse_operation(char *line, Operation *op){
    sscanf(line, " %c = %c %c %c", &op->result, &op->item1, &op->operator, &op->item2);
}

/* Function to analize print statement*/
void parse_print(char *line, char *variable){
    sscanf(line, " print %c", variable);
}

/* Function to translate high level code in assembly code*/
void generate_assembly(Assignment *assignments, int assignment_count, Operation *ops, int op_count, char *prints, int print_count){

    printf("section .data\n");
    printf("format db \"%%d\", 10, 0\n"); // format for printf

    // Assignment of values ​​to variables
    for (int i = 0; i < assignment_count; i++){
        printf("%c dq %d\n", assignments[i].variable, assignments[i].value);
    }

    printf("section .bss\n");
    // Allocate space for result
    for (int i = 0; i < op_count; i++){
        printf("%c resq 1\n", ops[i].result);
    }

    printf("section .text\n");
    printf("extern printf\n"); // Declaration of printf
    printf("global _start\n"); // Start of the program

    printf("_start:\n");

    for (int i = 0; i < assignment_count; i++){
        printf("\tmov rax, %d\n", assignments[i].value);       // Load the value in rax
        printf("\tmov [%c], rax\n", assignments[i].variable);  // Store the value in the variable
    }

    for (int i = 0; i < op_count; i++){
        printf("\tmov rax, [%c]\n", ops[i].item1);
        switch (ops[i].operator){
            case '+':
                printf("\tadd rax, [%c]\n", ops[i].item2);    // Addition
                break;
            case '-':
                printf("\tsub rax, [%c]\n", ops[i].item2);    // Subtraction
                break;
            case '*':
                printf("\timul rax, [%c]\n", ops[i].item2);    // Multiplication
                break;
            case '/':
                printf("\tcqo\n");                             // Convert rax to rdx:rax for division
                printf("\tidiv qword [%c]\n", ops[i].item2);   // Division
                break;
            default:
                fprintf(stderr, "Unsupported operator: %c\n", ops[i].operator);
                exit(1);
        }
        printf("\tmov [%c], rax\n", ops[i].result);
    }

    for (int i = 0; i < print_count; i++){
        printf("\tmov rdi, format\n");                         // Load address of string in rdi format
        printf("\tmov rsi, [%c]\n", prints[i]);                // Load the value of result in rsi
        printf("\txor rax, rax\n");                            // Clean rax
        printf("\tcall printf\n");                             // Call printf
    }
    printf("\tmov rax, 60\n");                                 // Syscall for exit
    printf("\txor rdi, rdi\n");                                // Exit code 0
    printf("\tsyscall\n");                                     // Invokes the operating system to terminate the program
}

int main(int argc, char *argv[]){

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Could not open source file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    Assignment assignments[MAX_CODE_LENGTH / MAX_LINE_LENGTH];
    Operation ops[MAX_CODE_LENGTH / MAX_LINE_LENGTH];
    char prints[MAX_CODE_LENGTH / MAX_LINE_LENGTH];
    
    int assignment_count = 0;
    int op_count         = 0;
    int print_count      = 0;

    /* Read line by line*/
    while (fgets(line, sizeof(line), file)){
        
        /* Remove comment */
        remove_comment(line);

        /* Ignore empty lines */
        if (line[0] == '\0' || line[0] == '\n' || line[0] == ' '){
            continue;
        }
        if (strstr(line, "+") != NULL || strstr(line, "-") != NULL || strstr(line, "*") != NULL || strstr(line, "/") != NULL){
            parse_operation(line, &ops[op_count]);
            op_count++;
        }
        else if (strstr(line, "print") != NULL){
            parse_print(line, &prints[print_count]);
            print_count++;
        }
        else {
            parse_assignment(line, &assignments[assignment_count]);
            assignment_count++;
        }
    }

    fclose(file);

    generate_assembly(assignments, assignment_count, ops, op_count, prints, print_count);

    return 0;
}

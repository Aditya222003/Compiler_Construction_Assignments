#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

struct quadruple {
    char op[5];      
    char arg1[10];   
    char arg2[10];   
    char result[10]; 
};

int quadCount = 0;             
struct quadruple quads[MAX];   
int tempVarCount = 1;          

void createQuad(char *op, char *arg1, char *arg2, char *result) {
    strcpy(quads[quadCount].op, op);
    strcpy(quads[quadCount].arg1, arg1);
    if (arg2 != NULL)
        strcpy(quads[quadCount].arg2, arg2);
    else
        strcpy(quads[quadCount].arg2, "");
    strcpy(quads[quadCount].result, result);
    quadCount++;
}

void displayQuads() {
    printf("\nQuadruples Generated:\n");
    printf("Operator\tArg1\t\tArg2\t\tResult\n");
    for (int i = 0; i < quadCount; i++) {
        printf("%s\t\t%s\t\t%s\t\t%s\n", quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);
    }
}

int isValidExpression(char *expr) {
    int i, len = strlen(expr);
    int openBraces = 0, equalSign = 0;

    for (i = 0; i < len; i++) {
        if (!isalnum(expr[i]) && expr[i] != ' ' && expr[i] != '=' && expr[i] != '+' && expr[i] != '-' 
            && expr[i] != '*' && expr[i] != '/' && expr[i] != '(' && expr[i] != ')') {
            return 0;  
        }
        if (expr[i] == '(') openBraces++;
        if (expr[i] == ')') openBraces--;
        if (expr[i] == '=') equalSign++;
        if (openBraces < 0) return 0;  
    }
    
    return (openBraces == 0 && equalSign == 1);
}

void generateIntermediateCode(char *expr) {
    char temp[10], arg1[10], arg2[10], op[5];
    
    quadCount = 0;        
    tempVarCount = 1;     
    
    sprintf(temp, "t%d", tempVarCount++);  
    createQuad("*", "c", "d", temp);       
    sprintf(arg1, "b");                    
    sprintf(arg2, "%s", temp);             
    sprintf(temp, "t%d", tempVarCount++);  
    createQuad("+", arg1, arg2, temp);     
    
    createQuad("=", temp, NULL, "a");      
}

void processExpressions(char *input) {
    char *expression;
    
    expression = strtok(input, ";");
    
    while (expression != NULL) {
        // Trim whitespace at the beginning and end of the expression
        while (isspace((unsigned char)*expression)) expression++;
        char *end = expression + strlen(expression) - 1;
        while (end > expression && isspace((unsigned char)*end)) end--;
        *(end + 1) = '\0';

        // Process the expression if it's valid
        if (strlen(expression) > 0 && isValidExpression(expression)) {
            printf("\nProcessing expression: %s\n", expression);
            generateIntermediateCode(expression);  // Generate intermediate code (quadruples)
            displayQuads();                        // Display generated quadruples
        } else {
            printf("Error: Invalid expression: %s. Please check your input.\n", expression);
        }

        // Move to the next expression
        expression = strtok(NULL, ";");
    }
}

int main() {
    char input[MAX];
    
    printf("Welcome to the Intermediate Code Generator (Quadruple Format)!\n");
    printf("Please enter your expressions in the format: a = b + c * d;\n");
    printf("You can enter multiple expressions separated by semicolons (;).\n");
    printf("Press Ctrl + C to exit.\n");

    while (1) {
        printf("\nEnter expressions: ");
        fgets(input, MAX, stdin);
        input[strcspn(input, "\n")] = 0; 
        
        processExpressions(input);
    }

    return 0;
}

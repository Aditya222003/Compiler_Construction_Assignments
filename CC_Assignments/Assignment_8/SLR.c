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
        if (!isalnum(expr[i]) && expr[i] != ' ' && expr[i] != '=' && expr[i] != '+' && expr[i] != '*' && expr[i] != '(' && expr[i] != ')') {
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
    createQuad("*", "c", "d", temp);       // Create quad for c * d
    
    // Now process the addition (b + t1)
    sprintf(arg1, "b");                    // First operand is b
    sprintf(arg2, "%s", temp);             // Second operand is temp variable t1
    sprintf(temp, "t%d", tempVarCount++);  // Create temp variable t2
    createQuad("+", arg1, arg2, temp);     // Create quad for b + t1
    
    // Finally, assign the result to 'a'
    createQuad("=", temp, NULL, "a");      // Create quad for a = t2
}

int main() {
    char expr[MAX];
    
    printf("Welcome to the Intermediate Code Generator (Quadruple Format)!\n");
    printf("Please enter your expressions in the format: a = b + c * d\n");
    printf("Press Ctrl + C to exit.\n");

    while (1) {
        // Get user input
        printf("\nEnter an expression: ");
        fgets(expr, MAX, stdin);
        expr[strcspn(expr, "\n")] = 0;  // Remove newline character
        
        // Validate expression
        if (isValidExpression(expr)) {
            generateIntermediateCode(expr);  // Generate intermediate code (quadruples)
            displayQuads();                  // Display generated quadruples
        } else {
            printf("Error: Invalid expression. Please check your input (balanced parentheses, correct format, etc.)\n");
        }
    }

    return 0;
}

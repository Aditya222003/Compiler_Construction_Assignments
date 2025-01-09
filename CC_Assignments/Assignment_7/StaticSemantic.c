#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 100
#define MAX_FUNCTIONS 10

typedef enum { INT, FLOAT, STRING, UNDEFINED } DataType;

typedef struct {
    char name[32];
    DataType type;
} Symbol;

typedef struct {
    Symbol table[MAX_SYMBOLS];
    int count;
} SymbolTable;

typedef struct {
    char name[32];
    DataType returnType;
    int paramCount;
    DataType paramTypes[10];
} Function;

typedef struct {
    Function table[MAX_FUNCTIONS];
    int count;
} FunctionTable;

SymbolTable symbolTable;
FunctionTable functionTable;

void initTables() {
    symbolTable.count = 0;
    functionTable.count = 0;
}

// Add a variable to the symbol table
int addSymbol(const char *name, DataType type) {
    for (int i = 0; i < symbolTable.count; i++) {
        if (strcmp(symbolTable.table[i].name, name) == 0) {
            printf("Error: Variable '%s' already declared in the same scope.\n", name);
            return -1;
        }
    }
    strcpy(symbolTable.table[symbolTable.count].name, name);
    symbolTable.table[symbolTable.count].type = type;
    symbolTable.count++;
    return 0;
}

// Get the type of a symbol
DataType getSymbolType(const char *name) {
    for (int i = 0; i < symbolTable.count; i++) {
        if (strcmp(symbolTable.table[i].name, name) == 0) {
            return symbolTable.table[i].type;
        }
    }
    return UNDEFINED;
}

int addFunction(const char *name, DataType returnType, int paramCount, DataType paramTypes[]) {
    for (int i = 0; i < functionTable.count; i++) {
        if (strcmp(functionTable.table[i].name, name) == 0) {
            printf("Error: Function '%s' already declared.\n", name);
            return -1;
        }
    }
    strcpy(functionTable.table[functionTable.count].name, name);
    functionTable.table[functionTable.count].returnType = returnType;
    functionTable.table[functionTable.count].paramCount = paramCount;
    memcpy(functionTable.table[functionTable.count].paramTypes, paramTypes, paramCount * sizeof(DataType));
    functionTable.count++;
    return 0;
}

// Check for type mismatches in assignment
void checkAssignment(const char *varName, DataType exprType) {
    DataType varType = getSymbolType(varName);
    if (varType == UNDEFINED) {
        printf("Error: Variable '%s' not declared.\n", varName);
    } else if (varType != exprType) {
        printf("Error: Type mismatch in assignment to '%s'. Expected %d but got %d.\n", varName, varType, exprType);
    } else {
        printf("Assignment to '%s' is valid.\n", varName);
    }
}

// Check for invalid division operations
void checkDivision(const char *varName, DataType divisorType) {
    DataType varType = getSymbolType(varName);
    if (varType == STRING || divisorType == STRING) {
        printf("Error: Cannot divide a string.\n");
    } else {
        printf("Division is valid.\n");
    }
}

// Check for function call correctness
void checkFunctionCall(const char *name, int argCount, DataType argTypes[]) {
    for (int i = 0; i < functionTable.count; i++) {
        if (strcmp(functionTable.table[i].name, name) == 0) {
            if (functionTable.table[i].paramCount != argCount) {
                printf("Error: Function '%s' called with incorrect number of arguments. Expected %d but got %d.\n", name, functionTable.table[i].paramCount, argCount);
            } else {
                for (int j = 0; j < argCount; j++) {
                    if (functionTable.table[i].paramTypes[j] != argTypes[j]) {
                        printf("Error: Type mismatch in argument %d of function '%s'. Expected %d but got %d.\n", j + 1, name, functionTable.table[i].paramTypes[j], argTypes[j]);
                    }
                }
            }
            return;
        }
    }
    printf("Error: Function '%s' not declared.\n", name);
}

// Function to process user input
void processInput(char *input) {
    // Split the input to process symbols, functions, or assignments
    char command[50];
    sscanf(input, "%s", command);

    if (strcmp(command, "addVar") == 0) {
        char varName[32];
        char type[10];
        sscanf(input + strlen(command), "%s %s", varName, type);
        DataType dataType = UNDEFINED;

        if (strcmp(type, "int") == 0) dataType = INT;
        else if (strcmp(type, "float") == 0) dataType = FLOAT;
        else if (strcmp(type, "string") == 0) dataType = STRING;

        if (dataType != UNDEFINED) {
            addSymbol(varName, dataType);
        } else {
            printf("Error: Invalid data type '%s'.\n", type);
        }
    } else if (strcmp(command, "addFunc") == 0) {
        char funcName[32], returnType[10];
        int paramCount;
        DataType paramTypes[10];

        sscanf(input + strlen(command), "%s %s %d", funcName, returnType, &paramCount);

        DataType returnDataType = UNDEFINED;
        if (strcmp(returnType, "int") == 0) returnDataType = INT;
        else if (strcmp(returnType, "float") == 0) returnDataType = FLOAT;
        else if (strcmp(returnType, "string") == 0) returnDataType = STRING;

        for (int i = 0; i < paramCount; i++) {
            sscanf(input + strlen(command) + strlen(funcName) + strlen(returnType) + 3 * i, "%s", returnType);
            if (strcmp(returnType, "int") == 0) paramTypes[i] = INT;
            else if (strcmp(returnType, "float") == 0) paramTypes[i] = FLOAT;
            else if (strcmp(returnType, "string") == 0) paramTypes[i] = STRING;
            else paramTypes[i] = UNDEFINED;  // Handle undefined
        }

        if (returnDataType != UNDEFINED) {
            addFunction(funcName, returnDataType, paramCount, paramTypes);
        } else {
            printf("Error: Invalid return type '%s'.\n", returnType);
        }
    } else if (strcmp(command, "assign") == 0) {
        char varName[32], type[10];
        sscanf(input + strlen(command), "%s %s", varName, type);
        DataType exprType = UNDEFINED;

        if (strcmp(type, "int") == 0) exprType = INT;
        else if (strcmp(type, "float") == 0) exprType = FLOAT;
        else if (strcmp(type, "string") == 0) exprType = STRING;

        checkAssignment(varName, exprType);
    } else if (strcmp(command, "divide") == 0) {
        char varName[32], type[10];
        sscanf(input + strlen(command), "%s %s", varName, type);
        DataType divisorType = UNDEFINED;

        if (strcmp(type, "int") == 0) divisorType = INT;
        else if (strcmp(type, "float") == 0) divisorType = FLOAT;
        else if (strcmp(type, "string") == 0) divisorType = STRING;

        checkDivision(varName, divisorType);
    } else if (strcmp(command, "callFunc") == 0) {
        char funcName[32];
        int argCount;
        DataType argTypes[10];

        sscanf(input + strlen(command), "%s %d", funcName, &argCount);
        for (int i = 0; i < argCount; i++) {
            char argType[10];
            sscanf(input + strlen(command) + strlen(funcName) + 2 * i, "%s", argType);
            if (strcmp(argType, "int") == 0) argTypes[i] = INT;
            else if (strcmp(argType, "float") == 0) argTypes[i] = FLOAT;
            else if (strcmp(argType, "string") == 0) argTypes[i] = STRING;
        }

        checkFunctionCall(funcName, argCount, argTypes);
    } else {
        printf("Error: Unknown command '%s'.\n", command);
    }
}

int main() {
    initTables();
    char input[256];

    // Add some variables and functions initially for testing
    addSymbol("x", INT);
    addSymbol("y", FLOAT);
    addSymbol("str", STRING);
    DataType paramTypes[] = { INT, FLOAT };
    addFunction("myFunc", FLOAT, 2, paramTypes);

    printf("Static Semantics Analyzer\n");
    printf("Enter commands (addVar, addFunc, assign, divide, callFunc), or Ctrl + C to exit.\n");

    while (1) {
        printf("> ");
        fgets(input, sizeof(input), stdin);
        processInput(input);
    }

    return 0;
}

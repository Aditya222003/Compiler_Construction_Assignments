#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    KEYWORD, IDENTIFIER, NUMBER, OPERATOR, DELIMITER, STRING_LITERAL, CHAR_LITERAL, DATATYPE, UNKNOWN, END
} TokenType;

TokenType getToken(char *lexeme);
int isKeyword(char *lexeme);
int isDatatype(char *lexeme);

int main() {
    char c;
    char lexeme[100];
    int index = 0;

    while ((c = getchar()) != EOF) {
        if (isspace(c)) {
            continue;
        }

        // Handle identifiers, keywords, and datatypes
        if (isalpha(c) || c == '_') {
            lexeme[index++] = c;
            while ((c = getchar()) != EOF && (isalnum(c) || c == '_')) {
                lexeme[index++] = c;
            }
            lexeme[index] = '\0';
            ungetc(c, stdin); 
            
            if (isDatatype(lexeme)) {
                printf("Datatype: %s\n", lexeme);
            } else if (isKeyword(lexeme)) {
                printf("Keyword: %s\n", lexeme);
            } else {
                printf("Identifier: %s\n", lexeme);
            }
            index = 0;
        }
        // Handle numbers and floating-point numbers
        else if (isdigit(c)) {
            lexeme[index++] = c;
            int hasDot = 0;
            while ((c = getchar()) != EOF && (isdigit(c) || (c == '.' && !hasDot))) {
                if (c == '.') {
                    hasDot = 1;
                }
                lexeme[index++] = c;
            }
            lexeme[index] = '\0';
            ungetc(c, stdin); 
            printf("Number: %s\n", lexeme);
            index = 0;
        }
        // Handle string literals
        else if (c == '"') {
            lexeme[index++] = c;
            while ((c = getchar()) != EOF && c != '"') {
                lexeme[index++] = c;
            }
            if (c == '"') {
                lexeme[index++] = c;
                lexeme[index] = '\0';
                printf("String Literal: %s\n", lexeme);
            } else {
                printf("Lexical Error: Unclosed string literal\n");
            }
            index = 0;
        }
        // Handle character literals
        else if (c == '\'') {
            lexeme[index++] = c;
            c = getchar();
            if (c != EOF && c != '\'') {
                lexeme[index++] = c;
                c = getchar();
                if (c == '\'') {
                    lexeme[index++] = c;
                    lexeme[index] = '\0';
                    printf("Character Literal: %s\n", lexeme);
                } else {
                    printf("Lexical Error: Unclosed character literal\n");
                }
            } else {
                printf("Lexical Error: Invalid character literal\n");
            }
            index = 0;
        }
        // Handle operators
        else if (strchr("+-*/=", c)) {
            printf("Operator: %c\n", c);
        }
        // Handle delimiters
        else if (strchr("(){};", c)) {
            printf("Delimiter: %c\n", c);
        }
        // Handle unknown characters and report lexical errors
        else {
            printf("Lexical Error: Unknown character: %c\n", c);
        }
    }

    return 0;
}

// Check if the lexeme is a keyword
int isKeyword(char *lexeme) {
    const char *keywords[] = {"if", "else", "while", "return"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(lexeme, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if the lexeme is a data type
int isDatatype(char *lexeme) {
    const char *datatypes[] = {"int", "float"};
    for (int i = 0; i < 2; i++) {
        if (strcmp(lexeme, datatypes[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

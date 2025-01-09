%{
#include <stdio.h>
#include "y.tab.h"
%}

%%

int         { printf("Datatype: int\n"); }
float       { printf("Datatype: float\n"); }
if          { printf("Keyword: if\n"); }
else        { printf("Keyword: else\n"); }
while       { printf("Keyword: while\n"); }
return      { printf("Keyword: return\n"); }
printf      { printf("Identifier: printf\n"); }

[a-zA-Z_][a-zA-Z0-9_]*  { printf("Lexical Error: Invalid identifier or mistyped keyword: %s\n", yytext); }
[0-9]+(\.[0-9]+)?       { printf("Number: %s\n", yytext); }

"+"                     { printf("Operator: +\n"); }
"-"                     { printf("Operator: -\n"); }
"*"                     { printf("Operator: *\n"); }
"/"                     { printf("Operator: /\n"); }
"="                     { printf("Operator: =\n"); }

"("                     { printf("Delimiter: (\n"); }
")"                     { printf("Delimiter: )\n"); }
"{"                     { printf("Delimiter: {\n"); }
"}"                     { printf("Delimiter: }\n"); }
";"                     { printf("Delimiter: ;\n"); }

\"[^\"]*\"              { printf("String Literal: %s\n", yytext); }
\'[^\']\'               { printf("Character Literal: %s\n", yytext); }

[ \t\n]                 { /* Ignore whitespace */ }

.                       { printf("Unknown character: %c\n", yytext[0]); }

%%

int main() {
    yylex();
    return 0;
}

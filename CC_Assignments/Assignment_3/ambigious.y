%{
#include<ctype.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<alloca.h>
#include<stddef.h>
extern FILE *yyin;

double userDefinedVariables[1024];
int yylex(void);
void yyerror(char *s)			
{      
    printf("%s\n",s);
}
%}


%union{
    double p;
    char id;
}
%token OPEN_BR CLOSE_BR ASSIGN
%token ADD SUB MUL DIV MOD
%token <id> VARIABLE
%token <p> num
%type <p> expr
%type <p> line

%left ADD SUB
%left MUL DIV MOD
%right POW
%left OPEN_BR CLOSE_BR
%%

line : expr {printf("Result: %f\n", $1);}
    | VARIABLE { printf("%f\n", userDefinedVariables[$1]); }

expr: expr ADD expr {$$=$1 + $3;}
    | expr SUB expr {$$=$1 - $3;}
    | expr MUL expr {$$=$1 * $3;}
    | expr DIV expr {$$=$1 / $3;}
    | expr MOD expr {$$=$1 - (floor($1/$3) * $3);}
    | expr POW expr {$$=pow($1, $3);}
    | num 



%%
void main() {
    printf("Scientific Calculator based on LEX YACC\n");
    yyin = stdin; // specify the input stream
    while(1){
        printf("Enter Expression: ");
        yyparse();
    }
}
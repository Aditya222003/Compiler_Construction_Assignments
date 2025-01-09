%{
#include<math.h>
#include"y.tab.h"
%}

%%
-?[0-9]+(\.[0-9]+)? {yylval.p=atof(yytext); return num;}
"+" {return ADD;}
"-" {return SUB;}
"**" {return POW;}
"*" {return MUL;}
"/" {return DIV;}
"%" {return MOD;}
"^" {return POW;}
"=" {return ASSIGN;}
"(" {return OPEN_BR;}
")" {return CLOSE_BR;}
[a-zA-Z]+[a-zA-Z0-9]* { yylval.id = yytext[0]; return VARIABLE; }
[ \t] ;
\n {return 0;}
. {printf ("Lexical Error | Invalid characters found! '%s'\n",yytext); return yytext[0];}
%%
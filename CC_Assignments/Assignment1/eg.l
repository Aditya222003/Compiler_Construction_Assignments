%%
[a-z]+   { printf("Found word: %s\n", yytext); }
.        { /* Ignore all other characters */ }
%%
int main() {
    yylex();
    return 0;
}

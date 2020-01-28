%option noyywrap nodefault yylineno
%{
#include "frontend.h"
#include "ast.h"
#include "calculator.tab.h"
int yycolumn = 1;
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line; \
    yylloc.first_column = yylloc.last_column; \
    for(int i = 0; yytext[i] != '\0'; i++) { \
        if(yytext[i] == '\n') { \
            yylloc.last_line++; \
            yylloc.last_column = 0; \
        } \
        else { \
            yylloc.last_column++; \
        } \
    }
%}


%%
[ \t]                       ;
\n                          ;
";"                         { return FINAL; }

[0-9]+\.[0-9]+              { yylval.d = atof(yytext); return FLOAT; }
[0-9]+                      { yylval.d = atof(yytext); return INTEGER; }

"print"                     { return PRINT; }

[a-zA-Z_][a-zA-Z0-9]*       { yylval.name = strdup(yytext); return VARIABLE; }

"+"                         { return ADD; }
"-"                         { return SUB; }
"*"                         { return MUL; }
"/"                         { return DIV; }
"^"                         { return EXP; }
"="                         { return EQUAL; }
")"                         { return RIGHT; }
"("                         { return LEFT; }
.                           { printf("UNKNOWN CHAR %c\n", *yytext); }
%%
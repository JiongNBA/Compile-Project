%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include "../include/tree.h"
#define YYSTYPE treeNode*
#include "parser.tab.hpp"

extern FlexLexer* lexer;

using namespace std;

%}

unsign_float ([1-9][0-9]*|0)\.[0-9]*|\.[0-9]+
unsign_int [1-9][0-9]*|0 
literal \"("\\".|[^\"\\])*\"
char \'(\\[abfnrtv\\\"\'\?0]|[^\'\\])\'
whitespace ("\t"|" "|"\n"|(("\/\*""\/"*([^\*\/]|[^\*]"\/"|"\*"[^\/])*("\*")*"\*\/")|("\/\/"[^\n]*)))+
id [\_a-zA-Z][a-zA-Z0-9\_]*
%%

\+ {yylval = new treeNode("ADD"); return ADD;}
\- {yylval = new treeNode("SUB"); return SUB;}
\* {yylval = new treeNode("MUL"); return MUL;}
\/ {yylval = new treeNode("DIV"); return DIV;}
\= {yylval = new treeNode("ASSIGN"); return ASSIGN;}
\! {yylval = new treeNode("NOT"); return NOT;}
\& {yylval = new treeNode("ADDR"); return ADDR;}
\|\| {yylval = new treeNode("OR"); return OR;}
\&\& {yylval = new treeNode("AND"); return AND;}
\=\= {yylval = new treeNode("EQUAL"); return EQUAL;}
\!\= {yylval = new treeNode("UEQUAL"); return UEQUAL;}
\> {yylval = new treeNode("GR"); return GR;}
\>\= {yylval = new treeNode("GE"); return GE;}
\< {yylval = new treeNode("LS"); return LS;}
\<\= {yylval = new treeNode("LE"); return LE;}
\( {yylval = new treeNode("LEFT_PAR"); return LEFT_PAR;}
\) {yylval = new treeNode("RIGHT_PAR"); return RIGHT_PAR;}
\{ {yylval = new treeNode("LEFT_CURLY"); return LEFT_CURLY;}
\} {yylval = new treeNode("RIGHT_CURLY"); return RIGHT_CURLY;}
\[ {yylval = new treeNode("LEFT_SQUARE"); return LEFT_SQUARE;}
\] {yylval = new treeNode("RIGHT_SQUARE"); return RIGHT_SQUARE;}
\; {yylval = new treeNode("SEMICOLON"); return SEMICOLON;}
\, {yylval = new treeNode("COMMA"); return COMMA;}
if {yylval = new treeNode("IF"); return IF;}
else {yylval = new treeNode("ELSE"); return ELSE;}
for {yylval = new treeNode("FOR"); return FOR;}
while {yylval = new treeNode("WHILE");return WHILE;}
return {yylval = new treeNode("RETURN"); return RETURN;}
break {yylval = new treeNode("BREAK"); return BREAK;}
continue {yylval = new treeNode("CONTINUE"); return CONTINUE;}
int {yylval = new treeNode("INT"); return INT;}
float {yylval = new treeNode("FLOAT"); return FLOAT;}
char {yylval = new treeNode("CHAR"); return CHAR;}
void {yylval = new treeNode("VOID"); return VOID;}
{literal} {yylval = new treeNode("LITERAL"); return LITERAL;}
{char} {yylval = new treeNode("CH"); return CH;}
{unsign_float} {yylval = new treeNode("NUM"); return NUM;}
{unsign_int} {yylval = new treeNode("NUM"); return NUM;}
{id} {yylval = new treeNode("ID"); return ID;}
{whitespace} {}
. {fprintf(stderr, "[ERROR] unexpected token \'%s\'\n", yytext); }
%%
/*
int main(int argc, char** argv)
{
    // check parameters
    if(argc == 1) {
        fprintf(stderr, "running: %s\n", argv[0]);
        return 1;
    }
    // 
    ifstream* srcCode = new ifstream();
    srcCode->open(argv[1], ios::in);
    if(srcCode->fail()) {
        fprintf(stderr, "this file doesn't exist or cannot be accessed!!!\n");
        return 1;
    }
    // create lexer instance
    FlexLexer* lexer = new yyFlexLexer(srcCode);
    // lexical analysis
    while(lexer->yylex());
    // free obj    
    delete srcCode;
    delete lexer;

    return 0;
}
*/

int yylex(void)
{
    return lexer->yylex();
}

int yyFlexLexer::yywrap()
{
    return 1;
}
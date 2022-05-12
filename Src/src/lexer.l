%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include "../include/AST.h"
#define YYSTYPE BaseAST *
#include "parser.tab.hpp"

extern FlexLexer *lexer;

using namespace std;

BaseAST *lastId;
BaseAST *curAST;

char parseChar(string str);
string parseStr(string str);

%}

%option yylineno

unsign_float ([1-9][0-9]*|0)\.[0-9]*|\.[0-9]+
unsign_int [1-9][0-9]*|0 
literal \"("\\".|[^\"\\])*\"
char \'(\\[abfnrtv\\\"\'\?0]|[^\'\\])\'
whitespace ("\t"|" "|"\n"|(("\/\*""\/"*([^\*\/]|[^\*]"\/"|"\*"[^\/])*("\*")*"\*\/")|("\/\/"[^\n]*)))+
id [\_a-zA-Z][a-zA-Z0-9\_]*
%%

\+ {return ADD;}
\- {return SUB;}
\* {return MUL;}
\/ {return DIV;}
\= {return ASSIGN;}
\! {return NOT;}
\& {return ADDR;}
\|\| {return OR;}
\&\& {return AND;}
\=\= {return EQUAL;}
\!\= {return UEQUAL;}
\> {return GR;}
\>\= {return GE;}
\< {return LS;}
\<\= {return LE;}
\( {return LEFT_PAR;}
\) {return RIGHT_PAR;}
\{ {return LEFT_CURLY;}
\} {return RIGHT_CURLY;}
\[ {return LEFT_SQUARE;}
\] {return RIGHT_SQUARE;}
\; {return SEMICOLON;}
\, {return COMMA;}
if {return IF;}
else {return ELSE;}
for {return FOR;}
while {return WHILE;}
return {return RETURN;}
break {return BREAK;}
continue {return CONTINUE;}
int {return INT;}
float {return FLOAT;}
char {return CHAR;}
void {return VOID;}
{literal} {Val val = parseStr(string(lexer->YYText())); curAST = new TokenAST(val, "LITERAL"); return LITERAL;}
{char} {Val val = parseChar(string(lexer->YYText())); curAST = new TokenAST(val, "CHAR"); return CH;}
{unsign_float} {Val val = atof(lexer->YYText()); curAST = new TokenAST(val, "FLOAT"); return NUM;}
{unsign_int} {Val val = atoi(lexer->YYText()); curAST = new TokenAST(val, "INT"); return NUM;}
{id} {Val val = string(lexer->YYText()); lastId = new TokenAST(val); curAST = lastId; return ID;}
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

char parseChar(string str)
{
    string ch = str.substr(1, str.size()-2);
    if(ch[0] == '\\') {
        switch(ch[1]) {
            case 'a': return '\a';
            case 'b': return '\b';
            case 'f': return '\f';
            case 'n': return '\n';
            case 'r': return '\r';
            case 't': return '\t';
            case 'v': return '\v';
            case '\\': return '\\';
            case '\'': return '\'';
            case '\"': return '\"';
            case '\?': return '\?';
            case '0': return '\0';
        }
    }
    return ch[0];
}

string parseStr(string str) {
    // trim "
    str = str.substr(1, str.size()-2);
    // res str
    string res;
    // parse char if needed
    for(int i = 0; i < str.size(); i++) {
        if(str[i] == '\\')
            res.push_back(parseChar("\'"+str.substr(i++, 2)+"\'"));
        else res.push_back(str[i]);
    }
    return res;
}
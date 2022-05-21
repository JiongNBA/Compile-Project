/* ----------------------------------------------------------
 * file: lexer.l
 * author: Haofeng Yang 3190105301@zju.edu.cn
 *         Yuesong Wang 3190105303@zju.edu.cn
 *         Jiong   Li   3190105725@zju.edu.cn
 * copyright: © 2022 authors. All rights reserved.
 * ----------------------------------------------------------
 * function: lexer for flySCC
 * description: lex related tokens, assemble corresponding
 *              AST node and record for parser to use.
 * ----------------------------------------------------------
 * create date:   2022/04/23 0:04 am
 * last modified: 2022/05/19 2:45 am
 * ----------------------------------------------------------
 * history: 2022/04/23 00:04 am: create the file and create rough framework of lexer
 *          2022/04/30 11:44 pm: use TokenAST to transfer yytext 
 *          2022/05/03 03:08 am: add parseChar function to deal with escape characters
 *          2022/05/19 02:45 am: improve the code structure and add comments
 */
  
%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include "../include/AST.h"
#define YYSTYPE BaseAST *
#include "parser.tab.hpp"

extern FlexLexer *lexer; // global lexer ptr

using namespace std;

BaseAST *lastId; // last Id AST node
BaseAST *curAST; // current lexed AST node

char   parseChar(string str);
string parseStr(string str);

%}

%option yylineno

unsign_float ([1-9][0-9]*|0)\.[0-9]*|\.[0-9]+
unsign_int   [1-9][0-9]*|0 
literal      \"("\\".|[^\"\\])*\"
char         \'(\\[abfnrtv\\\"\'\?0]|[^\'\\])\'
whitespace   ("\t"|" "|"\n"|(("\/\*""\/"*([^\*\/]|[^\*]"\/"|"\*"[^\/])*("\*")*"\*\/")|("\/\/"[^\n]*)))+
id           [\_a-zA-Z][a-zA-Z0-9\_]*
%%

\+   {return ADD;}
\-   {return SUB;}
\*   {return MUL;}
\/   {return DIV;}
\=   {return ASSIGN;}
\!   {return NOT;}
\&   {return ADDR;}
\|\| {return OR;}
\&\& {return AND;}
\=\= {return EQUAL;}
\!\= {return UEQUAL;}
\>   {return GR;}
\>\= {return GE;}
\<   {return LS;}
\<\= {return LE;}
\(   {return LEFT_PAR;}
\)   {return RIGHT_PAR;}
\{   {return LEFT_CURLY;}
\}   {return RIGHT_CURLY;}
\[   {return LEFT_SQUARE;}
\]   {return RIGHT_SQUARE;}
\;   {return SEMICOLON;}
\,   {return COMMA;}
if       {return IF;}
else     {return ELSE;}
for      {return FOR;}
while    {return WHILE;}
return   {return RETURN;}
break    {return BREAK;}
continue {return CONTINUE;}
int      {return INT;}
float    {return FLOAT;}
char     {return CHAR;}
void     {return VOID;}
{literal}      {Val val = parseStr(string(lexer->YYText())); curAST = new TokenAST(val, "LITERAL"); return LITERAL;}
{char}         {Val val = parseChar(string(lexer->YYText())); curAST = new TokenAST(val, "CHAR"); return CH;}
{unsign_float} {Val val = atof(lexer->YYText()); curAST = new TokenAST(val, "FLOAT"); return NUM;}
{unsign_int}   {Val val = atoi(lexer->YYText()); curAST = new TokenAST(val, "INT"); return NUM;}
{id}           {Val val = string(lexer->YYText()); lastId = new TokenAST(val); curAST = lastId; return ID;}
{whitespace}   {}
. {fprintf(stderr, "[ERROR] unexpected token \'%s\'\n", yytext); }
%%

/**
 * @brief redefine yylex for parser
 */
int yylex(void)
{
    return lexer->yylex();
}

/**
 * @brief yywrap
 */
int yyFlexLexer::yywrap()
{
    return 1;
}

/**
 * @brief parse escape char
 */
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

/**
 * @brief parse escape chars in string
 */
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
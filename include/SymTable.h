/* ----------------------------------------------------------
 * file: SymTable.h
 * author: Haofeng Yang 3190105301@zju.edu.cn
 *         Yuesong Wang 3190105303@zju.edu.cn
 *         Jiong   Li   3190105725@zju.edu.cn
 * copyright: © 2022 authors. All rights reserved.
 * ----------------------------------------------------------
 * function: the symbol table used while creating AST.
 * description: header file for SymTable.cpp
 * ----------------------------------------------------------
 * create date:   2022/04/29 09:02 pm
 * last modified: 2022/05/19 02:45 am
 * ----------------------------------------------------------
 * history: 2022/04/29 09:02 pm: create the file and add function insert and checkID
 *          2022/04/30 11:44 pm: add function prtTable() and delTable()
 *          2022/05/19 02:45 am: improve the code structure and add comments
 */

#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <FlexLexer.h>

extern FlexLexer *lexer; // global lexer ptr

/**
 * @brief Kind of symbols
 * FUNC : function
 * ARG  : parameter of the function
 * VAR  : variables
 * ARR  : array
 */
enum Kind {FUNC = 0, ARG = 1, VAR = 2, ARR = 3};

/**
 * @brief Symbol table entry
 * include: symbol name, symbol kind, symbol data type,
 *          symbol nested level and line number in src code
 */
class Entry
{
public:
    std::string name; // eg. foo, m, n, top
    Kind kind;        // eg. func, arg, var
    std::string type; // data type;
    int level;        // nested level
    int lineNum;      // the line number that appear in the code
    Entry();
    Entry(std::string name, Kind kind, 
          std::string type, int level, int lineNum);
    ~Entry();
};

/**
 * @brief Symbol table
 * each entry of the table is the stack of Entry of certain Symbol
 * e.g. for name 'a', in the entry there can be:
 * top level: int a; next level: int a(); bottom level: char a[]
 */
class SymTable
{
public:
    std::unordered_map<std::string, std::stack<Entry> > table;
    SymTable();
    ~SymTable();
    // check for the existence of the same ID in the idLevel
    bool checkId(std::string str, int idLevel) ;
    // check for the existence of the ID in the symbol table
    bool isFind(std::string str) ;
    // insert the symbol to the symbol table
    void insert(std::string str, Entry entry) ;
    // print the information of the symbol table
    void prtTable();
    // pop the symbol in the level layer
    void delTable(int level);
};

#endif /*SymTable.h*/
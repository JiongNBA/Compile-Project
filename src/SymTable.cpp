/* ----------------------------------------------------------
 * file: SymTable.cpp
 * author: Haofeng Yang 3190105301@zju.edu.cn
 *         Yuesong Wang 3190105303@zju.edu.cn
 *         Jiong   Li   3190105725@zju.edu.cn
 * copyright: © 2022 authors. All rights reserved.
 * ----------------------------------------------------------
 * function: the symbol table used while creating AST.
 * description: insert symbols and their attributes into this
 *              symbol table.
 * ----------------------------------------------------------
 * create date:   2022/04/29 09:02 pm
 * last modified: 2022/05/19 02:45 am
 * ----------------------------------------------------------
 * history: 2022/04/29 09:02 pm: create the file and add function insert and checkID
 *          2022/04/30 11:44 pm: add function prtTable() and delTable()
 *          2022/05/19 02:45 am: improve the code structure and add comments
 */

#include "../include/SymTable.h"

using namespace std;

/* ---------------------------- Entry ---------------------------- */

Entry::Entry() {}

Entry::Entry(string name, Kind kind, 
             string type, int level, int lineNum)
{
    this->name = name;
    this->kind = kind;
    this->type = type;
    this->level = level;
    this->lineNum = lineNum;
}

Entry::~Entry() {}

/* ---------------------------- SymTable ---------------------------- */

SymTable::SymTable() {}

SymTable::~SymTable() {}

// check for the existence of the same ID in the idLevel
bool SymTable::checkId(string str, int idLevel) {
    if(this->table.count(str) > 0 && this->table[str].top().level == idLevel) {
        cerr << "[line: " << lexer->lineno() << "]" 
             << "Declaration of \'" << str
             << "\' conflicts with the one in [line: " 
             << this->table[str].top().lineNum <<  "]" << endl;
        return false;
    }
    return true;
}

// check for the existence of the ID in the symbol table
bool SymTable::isFind(string str) {
    return this->table.count(str);
}

// insert the symbol to the symbol table
void SymTable::insert(string str, Entry entry) {
    if(this->table.count(str) == 0) {
        stack<Entry> newEntry;
        newEntry.push(entry);
        this->table[str] = newEntry;
    }
    else {
        this->table[str].push(entry);
    }
}

// print the information of the symbol table
void SymTable::prtTable()
{
    for( auto subTable : this->table ) {
        cout << "str: " << subTable.first << endl;
        while( !subTable.second.empty() ) {
            cout << "  [" << subTable.second.top().name << ", " 
                 << subTable.second.top().kind    << ", "
                 << subTable.second.top().type    << ", "
                 << subTable.second.top().level   << ", "
                 << subTable.second.top().lineNum << ", "
                 << "]" << endl;
            subTable.second.pop();
        }
    }
}

// pop the symbol in the level layer
void SymTable::delTable(int level)
{
    for( auto iter = this->table.begin(); iter != this->table.end(); ) {
        if( !iter->second.empty() && iter->second.top().level==level ) {
            iter->second.pop();
        }
        if( iter->second.empty() ) {
            this->table.erase(iter++);
        }
        else iter++;
    }
}
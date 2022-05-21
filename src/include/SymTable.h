#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<unordered_map>

extern FlexLexer *lexer;

enum Kind {FUNC = 0, ARG = 1, VAR = 2, ARR = 3};

class Entry
{
public:
    std::string name; // eg. foo, m, n, top
    Kind kind; // eg. func, arg, var
    std::string type; // data type;
    int addr; // offset
    int level; // nested level
    int lineNum;
    int size;
    Entry() {}
    Entry(std::string name, Kind kind, 
          std::string type, int addr, int level, int lineNum, int size):
          name(name), kind(kind), type(type), addr(addr), 
          level(level), lineNum(lineNum), size(size) {}
    ~Entry() {}
};

class SymTable
{
public:
    std::unordered_map<std::string, std::stack<Entry> > table;
    SymTable() {}
    ~SymTable() {}
    bool checkId(std::string str, int idLevel) {
        if(this->table.count(str) > 0 && this->table[str].top().level == idLevel) {
            std::cerr << "[line: " << lexer->lineno() << "]" << 
                         "Declaration of \'" << str
                      << "\' conflicts with the one in [line: " 
                      << this->table[str].top().lineNum <<  "]" << std::endl;
            return false;
        }
        return true;
    }
    bool isFind(std::string str) {
        return this->table.count(str);
    }
    void insert(std::string str, Entry entry) {
        if(this->table.count(str) == 0) {
            std::stack<Entry> newEntry;
            newEntry.push(entry);
            this->table[str] = newEntry;
        }
        else {
            this->table[str].push(entry);
        }
    }
    void prtTable()
    {
        for( auto subTable : this->table ) {
            std::cout << "str: " << subTable.first << std::endl;
            while( !subTable.second.empty() ) {
                std::cout << "  [" << subTable.second.top().name << ", " 
                                << subTable.second.top().kind << ", "
                                << subTable.second.top().type << ", "
                                << subTable.second.top().addr << ", "
                                << subTable.second.top().level << ", "
                                << subTable.second.top().lineNum << ", "
                                << subTable.second.top().size << "]" << std::endl;
                subTable.second.pop();
            }
        }
    }
    void delTable(int level)
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
};


#endif
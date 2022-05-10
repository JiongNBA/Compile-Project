#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

#include <string>
#include <vector>
#include <utility>
#include <FlexLexer.h>
#include "AST.h"
#include "SymTable.h"

extern FlexLexer *lexer;
extern SymTable symTable;
extern int level;
extern std::vector<BaseAST *> parent;
extern std::vector<int> addrs;
extern BaseAST *root;

void prtErr(std::string str) {
    std::cerr << "ERROR: [line: " << lexer->lineno() << "]" << str << std::endl;
}

void prtWarning(std::string str) {
    std::cerr << "WARNING: [line: " << lexer->lineno() << "]" << str << std::endl;
}

int getSize(std::string type) {
    if(type == "INT") return 4;
    else if(type == "CHAR") return 1;
    else if(type == "FLOAT") return 8;
    else if(type == "VOID") return 0;
    return 0;
}


class Func
{
public:
    static BaseAST *func(std::string type, Val id) {
        // create FuncAST
        BaseAST *fa = new FuncAST(id, type);        
        // update level
        level = 0;
        std::string strId = std::get<std::string>(id);
        // check symbol { $$ = Func::func($1->dtype, ->val); }
        if(strId == "printf" || strId == "scanf" || strId == "getchar") {
            prtErr("the name \'" + strId + "\' conflicts that of build-in function!");
            return NULL;
        }
        if(symTable.checkId(std::get<std::string>(id), level)) {
            // FIXME: decide value of size
            int size = 0;
            // add entry
            Entry entry(std::get<std::string>(id), FUNC, type, 
                        addrs[0], level, lexer->lineno(), size);
            symTable.insert(std::get<std::string>(id), entry);
            // update addrs
            addrs[0] += size;
            // push into parent stack
            parent.push_back(fa);
        }
        // TODO: error recovery
        else {
            
        } 
        
        addrs[1] = 0;

        return fa;
    }
};

class RetStmt
{
public:
    static BaseAST *ret(BaseAST *exp) {
        BaseAST *ra = new RetAST("ret", exp->dtype);
        if(level >= 1) {
            // type check
            if(parent[0]->dtype != ra->dtype) {
                prtErr("wrong return type!");
                // wrong recovery
                ra->dtype = parent[0]->dtype;
            }
            parent[level-1]->addChild(ra);

            ra->addChild(exp);
        }
        // wrong place
        else {
            prtErr("return statement in global scope!");
        }

        return ra;
    }
};

class DefStmt
{
public:
    static BaseAST *def(std::string type, Val id, bool isArray, int level) {
        BaseAST *da = new DefAST(type, id, isArray, level);
        std::string strId = std::get<std::string>(id);
        if(symTable.checkId(strId, level)) {
            int size = getSize(type);
            // add entry
            Entry entry(strId, isArray? ARR: VAR, type, 
                        addrs[level], level, lexer->lineno(), size);
            symTable.insert(strId, entry);
            // update addrs
            addrs[level] += size;

            if(level >= 1) parent[level-1]->addChild(da);
            else root->addChild(da);
        }
        // TODO: error recovery
        else {

        }

        return da;
    }

    static BaseAST *def(std::string type, Val id, bool isArray, BaseAST *exp, int level) {
        BaseAST *da = new DefAST(type, id, isArray, exp, level);
        std::string strId = std::get<std::string>(id);
        if(symTable.checkId(strId, level)) {
            int size = getSize(type);
            // add entry
            Entry entry(strId, isArray? ARR: VAR, type, 
                        addrs[level], level, lexer->lineno(), size);
            symTable.insert(strId, entry);
            // update addrs
            addrs[level] += size;

            if(level >= 1) parent[level-1]->addChild(da);
            else root->addChild(da);
        }
        // TODO: error recovery
        else {

        }

        return da;
    }
};


class IfStmt
{
public:
    static BaseAST *ifStmt(BaseAST *exp) {
        Val val = "if";
        BaseAST *ia = new IfAST(val, "INT");
    }
};



class DecStmt
{
public:
    static BaseAST *dec(std::string type, Val id, bool isArray) {
        BaseAST *da = new DecAST(id, type, isArray);
        std::string strId = std::get<std::string>(id);
        if(symTable.checkId(strId, level)) {
            int size = getSize(type);
            // add entry
            Entry entry(strId, ARG, type, 
                        addrs[level], level, lexer->lineno(), size);
            symTable.insert(strId, entry);
            // update addrs
            addrs[level] += size;

            parent[0]->addChild(da);
        }
        // TODO: error recovery
        else {

        }

        return da;
    }
};

class TYPE
{
public:
    static BaseAST *type(std::string type) {
        BaseAST *ta = new TypeAST(type);
        return ta;
    }
};

class Id
{
public:
    static BaseAST *id(Val val) {
        BaseAST *ia = new IdAST(val);
        return ia;
    }
};

class FuncCall
{
public:
    static BaseAST *funcCall(Val val) {
        // check symbol table to get type
        std::string strId = std::get<std::string>(val);
        if(strId == "printf" || strId == "scanf" || strId == "getchar") {
            BaseAST *fa = new FuncCallAST(val, "INT");
            return fa;
        }
        else if(symTable.isFind(strId)) {
            //symTable.prtTable();
            if(symTable.table[strId].top().kind == FUNC) {
                std::string type = symTable.table[strId].top().type;
                BaseAST *fa = new FuncCallAST(val, type);
                return fa;
            }
            else {
                prtErr("the identifier is redefined!");
            }
        }
        // TODO: error recovery
        else {
            // print error info
            prtErr("undefined identifer \'" + strId + "\'!");
        }
        return NULL;
    }
};

class Exp
{
public:
    // constant value
    static BaseAST *exp(Val val, std::string type) {
        BaseAST *ea = new ExpAST(val, type);
        return ea;
    }
    // ID reference
    static BaseAST *exp(Val val) {
        // check symbol table to get type
        std::string strId = std::get<std::string>(val);
        std::string type;
        int level;
        if(symTable.isFind(strId)) {
            type = symTable.table[strId].top().type;
            level = symTable.table[strId].top().level;
        }
        // error recovery
        else {
            prtErr("undeclared identifer \'"+strId+"\'!");
            type = "INT";
        }
        if(level>0) return  new ExpAST(val, type, L_VAR);
        else        return  new ExpAST(val, type, G_VAR);

        return NULL;
    }
    /*
    // ADDR of ID(var / func name)
    static BaseAST *exp(BaseAST *id, nType ntype) {
        BaseAST *ea = new ExpAST(val, type, ntype);
        // check symbol table to get type
        std::string strId = std::get<std::string>(val);
        std::string type;
        if(symTable.isFind(strId)) {
            type = symTable.table[strId].top().type;
        }
        // error recovery
        else {
            prtErr("undeclared identifer \'"+strId+"\'!");
            type = "INT";
        }
        return ea;
    }
    */
    static BaseAST *sub_exp(BaseAST *node) {
        BaseAST *ea = new UniOpAST("-", node->dtype, NEG);
        ea->child.push_back(node);
        return ea;
    }
    static BaseAST *not_exp(BaseAST *node) {
        BaseAST *ea = new UniOpAST("!", node->dtype, N_NOT);
        ea->child.push_back(node);
        if(std::get<std::string>(node->val) == "!") {
            UniOpAST * n = dynamic_cast<UniOpAST *>(node);
            n->isTop = false;
        }
        return ea;
    }
    static BaseAST *binary_exp(BaseAST *node_left,BaseAST *node_right,nType ntype) {
        std::string res_type;
        if(node_left->dtype=="FLOAT"||node_right->dtype=="FLOAT") res_type = "FLOAT";
        else if(node_left->dtype=="INT"||node_right->dtype=="INT") res_type = "INT";
        else res_type = "CHAR";
        BaseAST *ea = new BinaryOpAST("binary", res_type, ntype);
        ea->child.push_back(node_left);
        ea->child.push_back(node_right);
        return ea;
    }
    static BaseAST *assign_exp(BaseAST *node_left) {
        // check symbol table to get type
        int level;
        std::string strId = std::get<std::string>(node_left->val);
        if(symTable.isFind(strId)) {
            node_left->dtype = symTable.table[strId].top().type;
            level=symTable.table[strId].top().level;
        }
        // error recovery
        else {
            prtErr("undeclared identifer \'"+strId+"\'!");
            node_left->dtype = "INT";
        }
        BaseAST *ea;
        if(level>0)
            ea = new AssignAST("=", node_left->dtype, L_VAR);
        else ea = new AssignAST("=", node_left->dtype, G_VAR);
        ea->child.push_back(node_left);
        return ea;
    }
    static BaseAST *addr_exp(Val val) {
        int level;
        std::string type;
        // check symbol table to get type
        std::string strId = std::get<std::string>(val);
        if(symTable.isFind(strId)) {
            level=symTable.table[strId].top().level;
            type = symTable.table[strId].top().type;
        }
        // error recovery
        else {
            prtErr("undeclared identifer \'"+strId+"\'!");
        }
        BaseAST *ea;
        if(level>0) {
            ea = new AddrAST(val, type, L_VAR);
        }
        else 
            ea = new AddrAST(val,type, G_VAR);
        return ea;
    }
};

#endif
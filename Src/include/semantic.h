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
            std::cout << "parent: " << std::endl;
            for(int i = 0; i < parent.size(); i++) {
                std::cout << parent[i] << std::endl;
            }
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
    static BaseAST *def(std::string type, Val id, int level) {
        BaseAST *da = new DefAST(type, id, level);
        std::string strId = std::get<std::string>(id);
        if(symTable.checkId(strId, level)) {
            int size = getSize(type);
            // add entry
            Entry entry(strId, VAR, type, 
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

    static BaseAST *def(std::string type, Val id, BaseAST *exp, int level) {
        BaseAST *da = new DefAST(type, id, exp, level);
        std::string strId = std::get<std::string>(id);
        if(symTable.checkId(strId, level)) {
            int size = getSize(type);
            // add entry
            Entry entry(strId, VAR, type, 
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

    // array def
    static void arrDef(BaseAST *arr, std::string curType) {
        arrAST *a = dynamic_cast<arrAST *>(arr);
        a->dtype = curType; 
        a->isDef = true; 
        a->isAssign = false; 

        // array name
        BaseAST *node = arr;
        int eleNum = 1;
        while(!node->child.empty()) {
            eleNum *= std::get<int>(node->child[0]->val);
            node = node->child[1];
        }
        std::string strId = a->id;
        if(symTable.checkId(strId, a->arrLevel)) {
            int size = getSize(curType) * eleNum;
            // add entry
            Entry entry(strId, ARR, curType, 
                        addrs[level], level, lexer->lineno(), size);
            symTable.insert(strId, entry);
            // update addrs
            addrs[level] += size;

            if(level >= 1) parent[level-1]->addChild(arr);
            else root->addChild(arr);
        }
        // TODO: error recovery
        else {

        }
    }

};


class CondStmt
{
public:
    static BaseAST *condStmt() {
        Val val = "cond";
        BaseAST *ca = new CondAST(val, "VOID");
        return ca; 
    }

    static BaseAST *ifStmt(BaseAST *exp) {
        Val val = "if";
        BaseAST *ia = new IfAST(val, "VOID");
        ia->addChild(exp);
        return ia;
    }

    static BaseAST *elifStmt(BaseAST *exp) {
        Val val = "elif";
        BaseAST *ea = new IfAST(val, "VOID");
        ea->addChild(exp);
        return ea;
    }

    static BaseAST *elseStmt() {
        Val val = "else";
        BaseAST *ea = new ElseAST(val, "VOID");
        return ea;
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
        int idLevel;
        if(symTable.isFind(strId)) {
            type = symTable.table[strId].top().type;
            idLevel = symTable.table[strId].top().level;
        }
        // error recovery
        else {
            prtErr("undeclared identifer \'"+strId+"\'!");
            type = "INT";
        }
        if(idLevel>0) return  new ExpAST(val, type, L_VAR);
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
            UniOpAST *n = dynamic_cast<UniOpAST *>(node);
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

    static void arr_exp(BaseAST *arr, bool isAssign) {
        arrAST *a = dynamic_cast<arrAST *>(arr);
        a->isDef = false; 
        a->isAssign = isAssign;

        std::string strId = a->id;
        // FIXME: store num of array-level in symbol table
        if(symTable.isFind(strId)) {
            a->arrLevel=symTable.table[strId].top().level;
            a->dtype = symTable.table[strId].top().type;
        }
        // error recovery
        else {
            prtErr("undeclared identifer \'"+strId+"\'!");
        }
    }

};
class Br{
public:
    static BaseAST *Brstmt(std::string type) {
        BaseAST *ra = new BrAST("br",type);
        parent[level-1]->addChild(ra);
        return ra;
    }
};

class Loop{
public: 
    static BaseAST *While_loop(BaseAST *exp) {
        BaseAST *ra = new LoopAST("loop", "while");
        parent[level-1]->addChild(ra);
        parent.push_back(ra);
        ra->addChild(exp);
        addrs.push_back(0);
        return ra;
    }
    static BaseAST *For_loop() {
        BaseAST *ra = new LoopAST("loop","for");
        parent[level-1]->addChild(ra);
        parent.push_back(ra);
        addrs.push_back(0);
        return ra;
    }

};

class Arr
{
public:
    static BaseAST *arr(BaseAST *exp, int arrLevel) {
        BaseAST *aa = new arrAST("[]", "VOID", arrLevel);
        aa->addChild(exp);
        return aa;
    }

    static void setFstArr(BaseAST *arr, Val id) {
        arrAST *a = dynamic_cast<arrAST *>(arr); 
        a->id = std::get<std::string>(id);
        a->dim = 1; 
    }

    static void setSucArr(BaseAST *arr_par, BaseAST *arr_chd) {
        arrAST *a_par = dynamic_cast<arrAST *>(arr_par); 
        arrAST *a_chd = dynamic_cast<arrAST *>(arr_chd);
        a_par->id = a_chd->id;
        a_par->dim = a_chd->dim + 1;
    }

};

#endif

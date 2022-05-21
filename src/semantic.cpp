/* ----------------------------------------------------------
 * file: semantic.cpp
 * author: Haofeng Yang 3190105301@zju.edu.cn
 *         Yuesong Wang 3190105303@zju.edu.cn
 *         Jiong   Li   3190105725@zju.edu.cn
 * copyright: © 2022 authors. All rights reserved.
 * ----------------------------------------------------------
 * function: definition of semantic analysis method classes.
 * description: sets of semantic analysis operations,
 *              according to grammarical symbols.
 * ----------------------------------------------------------
 * create date:   2022/04/29 09:02 pm
 * last modified: 2022/05/19 02:45 am
 * ---------------------------------------------------------- 
 * history: 2022/04/29 09:02 pm: create the file and create AST for function
 *          2022/04/30 11:44 pm: add RetStmt, DecStmt, TYPE, Id and Exp class
 *          2022/05/03 03:08 am  create DefStmt class and Exp class, 
 *                               add uniop function to generate UniopAST
 *          2022/05/05 00:27 am: create binaryExp function in Exp class to generate BinaryOpAST
 *          2022/05/06 03:16 am: create FuncCall class to generate FuncCallAST, 
 *                               create assignExp function in exp class
 *          2022/05/09 00:11 am: create addrExp function in Exp class to generate AddrAST
 *          2022/05/10 00:15 pm: create condstmt class and loop class
 *          2022/05/13 10:19 pm: add Arr class to generate ArrAST
 *          2022/05/19 02:45 am: improve the code structure and add comments
 */

#include "../include/semantic.h"

using namespace std;

/* ---------------------------- print error & warning info ---------------------------- */

void prtErr(string str) {
    cerr << "ERROR: [line: " << lexer->lineno() << "]" << str << endl;
}

void prtWarning(string str) {
    cerr << "WARNING: [line: " << lexer->lineno() << "]" << str << endl;
}

/* ---------------------------- Func ---------------------------- */
BaseAST* Func::func(string type, Val id) {
    // create FuncAST
    BaseAST *fa = new FuncAST(id, type);        
    // update level
    level = 0;
    string strId = get<string>(id);
    // check symbol { $$ = Func::func($1->dtype, ->val); }
    if(strId == "printf" || strId == "scanf" || strId == "getchar") {
        prtErr("the name \'" + strId + "\' conflicts that of build-in function!");
        return NULL;
    }
    if(symTable.checkId(get<string>(id), level)) {
        // add entry
        Entry entry(get<string>(id), FUNC, type, 
                        level, lexer->lineno());
        symTable.insert(get<string>(id), entry);
        // push into parent stack
        parent.push_back(fa);
    }
    // error recovery
    // else {}

    return fa;
}

/* ---------------------------- RetStmt ---------------------------- */
BaseAST* RetStmt::ret(BaseAST *exp) {
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

/* ---------------------------- DefStmt ---------------------------- */
BaseAST* DefStmt::def(string type, Val id, int level) {
    BaseAST *da = new DefAST(type, id, level);
    string strId = get<string>(id);
    // check variable id
    if(symTable.checkId(strId, level)) {
        // add entry
        Entry entry(strId, VAR, type, 
                    level, lexer->lineno());
        symTable.insert(strId, entry);

        if(level >= 1) parent[level-1]->addChild(da);
        else root->addChild(da);
    }
    // error recovery
    // else {}

    return da;
}

BaseAST* DefStmt::def(string type, Val id, BaseAST *exp, int level) {
    BaseAST *da = new DefAST(type, id, exp, level);
    string strId = get<string>(id);
    if(symTable.checkId(strId, level)) {
        // add entry
        Entry entry(strId, VAR, type, 
                    level, lexer->lineno());
        symTable.insert(strId, entry);

        // add into parent's child
        if(level >= 1) parent[level-1]->addChild(da);
        else root->addChild(da);
    }
    // error recovery
    // else {}

    return da;
}

void DefStmt::arrDef(BaseAST *arr, string curType) {
    ArrAST *a = dynamic_cast<ArrAST *>(arr);
    a->dtype = curType; 
    a->isDef = true; 
    a->isAssign = false; 

    // array name
    BaseAST *node = arr;
    int eleNum = 1;
    while(!node->child.empty()) {
        eleNum *= get<int>(node->child[0]->val);
        node = node->child[1];
    }
    string strId = a->id;
    if(symTable.checkId(strId, a->arrLevel)) {
        // add entry
        Entry entry(strId, ARR, curType, 
                    level, lexer->lineno());
        symTable.insert(strId, entry);

        // add into parent's child
        if(level >= 1) parent[level-1]->addChild(arr);
        else root->addChild(arr);
    }
    // error recovery
    // else {}
}

/* ---------------------------- CondStmt ---------------------------- */
BaseAST* CondStmt::condStmt() {
    Val val = "cond";
    BaseAST *ca = new CondAST(val, "VOID");
    return ca; 
}

BaseAST* CondStmt::ifStmt(BaseAST *exp) {
    Val val = "if";
    BaseAST *ia = new IfAST(val, "VOID");
    ia->addChild(exp);
    return ia;
}

BaseAST* CondStmt::elifStmt(BaseAST *exp) {
    Val val = "elif";
    BaseAST *ea = new IfAST(val, "VOID");
    ea->addChild(exp);
    return ea;
}

BaseAST* CondStmt::elseStmt() {
    Val val = "else";
    BaseAST *ea = new ElseAST(val, "VOID");
    return ea;
}

/* ---------------------------- DecStmt ---------------------------- */
BaseAST* DecStmt::dec(string type, Val id, bool isArray) {
    BaseAST *da = new DecAST(id, type, isArray);
    string strId = get<string>(id);
    if(symTable.checkId(strId, level)) {
        // add entry
        Entry entry(strId, ARG, type, 
                    level, lexer->lineno());
        symTable.insert(strId, entry);

        parent[0]->addChild(da);
    }
    // error recovery
    else {}

    return da;
}


/* ---------------------------- TYPE ---------------------------- */
BaseAST* TYPE::type(string type) {
    BaseAST *ta = new TypeAST(type);
    return ta;
}

/* ---------------------------- Id ---------------------------- */
BaseAST* Id::id(Val val) {
    BaseAST *ia = new IdAST(val);
    return ia;
}

/* ---------------------------- FuncCall ---------------------------- */
BaseAST* FuncCall::funcCall(Val val) {
    // check symbol table to get type
    string strId = get<string>(val);
    if(strId == "printf" || strId == "scanf" || strId == "getchar") {
        BaseAST *fa = new FuncCallAST(val, "INT");
        return fa;
    }
    else if(symTable.isFind(strId)) {
        //symTable.prtTable();
        if(symTable.table[strId].top().kind == FUNC) {
            string type = symTable.table[strId].top().type;
            BaseAST *fa = new FuncCallAST(val, type);
            return fa;
        }
        else {
            prtErr("the identifier is redefined!");
        }
    }
    else {
        // print error info
        prtErr("undefined identifer \'" + strId + "\'!");
        // error recovery
    }
    return NULL;
}

/* ---------------------------- Exp ---------------------------- */
BaseAST* Exp::exp(Val val, string type) {
    BaseAST *ea = new ExpAST(val, type);
    return ea;
}

BaseAST* Exp::exp(Val val) {
    // check symbol table to get type
    string strId = get<string>(val);
    string type;
    int idLevel;
    if(symTable.isFind(strId)) {
        type = symTable.table[strId].top().type;     // get type of the ID from the symbol table
        idLevel = symTable.table[strId].top().level; // get the level of the ID
    }
    // error recovery
    else {
        prtErr("undeclared identifer \'"+strId+"\'!");
        type = "INT";
    }
    if(idLevel>0) return  new ExpAST(val, type, L_VAR); // local ID
    else          return  new ExpAST(val, type, G_VAR); // global ID

    return NULL;
}

BaseAST* Exp::sub_exp(BaseAST *node) { 
    BaseAST *ea = new UniOpAST("-", node->dtype, NEG);
    ea->child.push_back(node);
    return ea;
}

BaseAST* Exp::not_exp(BaseAST *node) {
    BaseAST *ea = new UniOpAST("!", node->dtype, N_NOT);
    ea->child.push_back(node);
    if(get_if<string>(&(node->val)) && get<string>(node->val) == "!") {
        UniOpAST *n = dynamic_cast<UniOpAST *>(node);
        n->isTop = false;
    }
    return ea;
}

BaseAST* Exp::binary_exp(BaseAST *node_left, BaseAST *node_right, nType ntype, string op) {
    string res_type;
    // trans the type to higher
    if(node_left->dtype=="FLOAT"||node_right->dtype=="FLOAT") res_type = "FLOAT";
    else if(node_left->dtype=="INT"||node_right->dtype=="INT") res_type = "INT";
    else res_type = "CHAR";
    BaseAST *ea = new BinaryOpAST(op, res_type, ntype);

    ea->child.push_back(node_left);
    ea->child.push_back(node_right);
    return ea;
}

BaseAST* Exp::assign_exp(BaseAST *node_left) {
    // check symbol table to get type
    int level;
    string strId = get<string>(node_left->val);
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

BaseAST* Exp::addr_exp(Val val) {
    int level;
    string type;
    // check symbol table to get type
    string strId = get<string>(val);
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

void Exp::arr_exp(BaseAST *arr, bool isAssign) {
    ArrAST *a = dynamic_cast<ArrAST *>(arr);
    a->isDef = false; 
    a->isAssign = isAssign;

    string strId = a->id;
    // get array info
    if(symTable.isFind(strId)) {
        a->arrLevel=symTable.table[strId].top().level;
        a->dtype = symTable.table[strId].top().type;
    }
    // error recovery
    else {
        prtErr("undeclared identifer \'"+strId+"\'!");
    }
}

/* ---------------------------- Br ---------------------------- */
BaseAST* Br::Brstmt(string type) {
    BaseAST *ra = new BrAST("br",type);
    parent[level-1]->addChild(ra);
    return ra;
}

/* ----------------------------- Loop ---------------------------- */
BaseAST* Loop::While_loop(BaseAST *exp) {
    BaseAST *ra = new LoopAST("loop", "while");
    parent[level-1]->addChild(ra);
    parent.push_back(ra);
    ra->addChild(exp);
    return ra;
}

BaseAST* Loop::For_loop() {
    BaseAST *ra = new LoopAST("loop","for");
    parent[level-1]->addChild(ra);
    parent.push_back(ra);
    return ra;
}

/* ----------------------------- Arr ---------------------------- */
BaseAST* Arr::arr(BaseAST *exp, int arrLevel) {
    BaseAST *aa = new ArrAST("[]", "VOID", arrLevel);
    aa->addChild(exp);
    return aa;
}

void Arr::setFstArr(BaseAST *arr, Val id) {
    ArrAST *a = dynamic_cast<ArrAST *>(arr); 
    a->id = get<string>(id);
    a->dim = 1; 
}

void Arr::setSucArr(BaseAST *arr_par, BaseAST *arr_chd) {
    ArrAST *a_par = dynamic_cast<ArrAST *>(arr_par); 
    ArrAST *a_chd = dynamic_cast<ArrAST *>(arr_chd);
    a_par->id = a_chd->id;
    a_par->dim = a_chd->dim + 1;
}
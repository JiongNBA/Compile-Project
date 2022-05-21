/* ----------------------------------------------------------
 * file: semantic.h
 * author: Haofeng Yang 3190105301@zju.edu.cn
 *         Yuesong Wang 3190105303@zju.edu.cn
 *         Jiong   Li   3190105725@zju.edu.cn
 * copyright: © 2022 authors. All rights reserved.
 * ----------------------------------------------------------
 * function: definition of semantic analysis method classes.
 * description: header file  for semantic.cpp
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

#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

#include <string>
#include <vector>
#include <utility>
#include <FlexLexer.h>
#include "AST.h"
#include "SymTable.h"

extern int level;         // nested level
extern FlexLexer *lexer;  // global lexer ptr
extern SymTable symTable; // our own symbol table
extern BaseAST *root;     // AST root
extern std::vector<BaseAST *> parent; // parent node on each level

/**
 * @brief print error info
 */
void prtErr(std::string str);

/**
 * @brief print warning info
 */
void prtWarning(std::string str);

/**
 * @brief function semantic analysis
 */
class Func
{
public:
    static BaseAST *func(std::string type, Val id);
};

/**
 * @brief return semantic analysis
 * 
 */
class RetStmt
{
public:
    static BaseAST *ret(BaseAST *exp) ;
};

/**
 * @brief definition of variable & array semantic
 */
class DefStmt
{
public:
    /**
     * @brief definition without initialization
     */
    static BaseAST *def(std::string type, Val id, int level);

    /**
    * @brief definition with initialization
    */
    static BaseAST *def(std::string type, Val id, BaseAST *exp, int level);

    // array definition
    static void arrDef(BaseAST *arr, std::string curType);

};

/**
 * @brief condition statement semantic analysis
 * include: cond stmt, if stmt, elif stmt and else stmt
 */
class CondStmt
{
public:
    /**
     * @brief condition stmt
     */
    static BaseAST *condStmt();

    /**
     * @brief if stmt
     */
    static BaseAST *ifStmt(BaseAST *exp);

    /**
     * @brief elif stmt
     */
    static BaseAST *elifStmt(BaseAST *exp);

    /**
     * @brief else stmt
     */
    static BaseAST *elseStmt();
};

/**
 * @brief declaration statement semantic analysis
 * NOTE: only for function parameters
 */
class DecStmt
{
public:
    static BaseAST *dec(std::string type, Val id, bool isArray);
};

/**
 * @brief type semantic analysis
 */
class TYPE
{
public:
    static BaseAST *type(std::string type);
};

/**
 * @brief ID semantic analysis
 */
class Id
{
public:
    static BaseAST *id(Val val) ;
};

/**
 * @brief function caller semantic analysis
 */
class FuncCall
{
public:
    static BaseAST *funcCall(Val val);
};

/**
 * @brief expression semantic analysis
 */
class Exp
{
public:
    // constant value
    static BaseAST *exp(Val val, std::string type);
    // ID reference
    static BaseAST *exp(Val val);
    // unary operator '-'
    static BaseAST *sub_exp(BaseAST *node);
    // unary operator '!'
    static BaseAST *not_exp(BaseAST *node) ;
    // binary operator
    static BaseAST *binary_exp(BaseAST *node_left,BaseAST *node_right,nType ntype, std::string op);
    // assign operator
    static BaseAST *assign_exp(BaseAST *node_left);
    // get-addr operator
    static BaseAST *addr_exp(Val val);
    // array expression
    // isAssign: true-array assignment; false-load array element 
    static void arr_exp(BaseAST *arr, bool isAssign);

};

/**
 * @brief branch semantic analysis
 * including break and continue
 */
class Br{
public:
    // type: break / continue
    static BaseAST *Brstmt(std::string type);
};

/**
 * @brief loop semantic analysis
 * including for loop and while loop
 */
class Loop{
public:
   /**
    * @brief while loop semantic analysis
    */ 
    static BaseAST *While_loop(BaseAST *exp);
    /**
     * @brief while semantic analysis
     */
    static BaseAST *For_loop() ;

};

/**
 * @brief array semantic analysis
 */
class Arr
{
public:
   /**
    * @brief create array node
    * parent: [], first child: exp
    */ 
    static BaseAST *arr(BaseAST *exp, int arrLevel) ;

    /**
     * @brief append id to []
     */
    static void setFstArr(BaseAST *arr, Val id) ;

    /**
     * @brief appned subtree to []
     */
    static void setSucArr(BaseAST *arr_par, BaseAST *arr_chd);

};

#endif /*semantic.h*/
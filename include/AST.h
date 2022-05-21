/* ----------------------------------------------------------
 * file: AST.h
 * author: Haofeng Yang 3190105301@zju.edu.cn
 *         Yuesong Wang 3190105303@zju.edu.cn
 *         Jiong   Li   3190105725@zju.edu.cn
 * copyright: © 2022 authors. All rights reserved.
 * ----------------------------------------------------------
 * function: header file of AST nodes
 * description: header file for AST.cpp
 * ----------------------------------------------------------
 * create date:   2022/04/29 09:02 pm
 * last modified: 2022/05/19 02:45 am
 * ----------------------------------------------------------
 * history: 2022/04/29 21:02 pm: create the file and create parent class BaseAST
 *          2022/04/30 11:44 pm: add preTree function and add TokenAST, RetAST, TypeAST, IdAST and ExpAST, 
 *                               Codegen for FuncAST and RetAST
 *          2022/05/01 09:56 pm: Codegen for DecAST, ExpAST
 *          2022/05/03 03:08 am: add DefAST and UniOpAST, Codegen for DefAST and ExpAST
 *          2022/05/05 00:27 am: add binaryOpAST and Codegen for binaryOpAST
 *          2022/05/06 03:16 am: improve the binaryOpAST,add cast function in baseclass. 
 *                               add FuncCallAST and AssignAST, Codegen for local var and assign stmt
 *          2022/05/06 08:36 pm: Codegen for global var
 *          2022/05/09 00:11 am: add AddrAST and add Codegen function
 *          2022/05/10 00:15 pm: add CondAST and LoopAST
 *          2022/05/12 11:13 am: add visual function to print AST
 *          2022/05/13 10:19 pm: add ArrAST and improve LoopAST
 *          2022/05/19 02:45 am: improve the code structure and add comments
 */

#ifndef _AST_H_
#define _AST_H_

#include<string>
#include<vector>
#include<queue>
#include<variant>
#include<stack>
#include<iostream>
#include<fstream>
#include<llvm/IR/Value.h>
#include<llvm/IR/Type.h>
#include<llvm/Support/Error.h>
#include<llvm/IR/DerivedTypes.h>
#include<llvm/IR/Function.h>
#include<llvm/IR/Module.h>
#include<llvm/IR/IRBuilder.h>
#include<llvm/IR/LLVMContext.h>
#include<llvm/IR/BasicBlock.h>
#include<llvm/IR/ValueSymbolTable.h>
#include<llvm/ADT/StringRef.h>
#include<llvm/IR/Instruction.h>
#include<llvm/Support/Casting.h>
#include"./IO.h"

using namespace llvm;

typedef std::variant<int, double, char, std::string> Val;

extern Function *fly; // main funciton 
extern std::ofstream ofresult; // output JSON file
extern std::unique_ptr<Module> mod;
extern std::unique_ptr<LLVMContext> context;
extern std::unique_ptr<IRBuilder<> > builder;
extern std::stack<BasicBlock *> loopEnd; // insert point an the end of the loop
extern std::stack<BasicBlock *> loopStart;// insert point an the start of the loop
extern std::map<std::string, GlobalVariable*> globalValues; // symbol table of the global variable


/**
 * @brief nType explanation
 * PAR : parameter of the function
 * L_VAR : local  variables
 * G_VAR : global variables
 * RET   : return statement of the fuction
 * NEG   : the unary operator '-'
 * N_NOT : the unary operator '!'
 * B_MUL B_DIV  : binary operator '*' and '/'
 * B_SUB B_ADD  : the binary operator '-' and '+'
 * B_GR  B_GE   : the binary operator '>' '>=' binary operator '>' and '>='
 * B_LS  B_LE   : the binary operator '<' '<=' : binary operator '<' and '<='
 * B_EQ  B_UEQ  : the binary operator '==' '!=' : binary operator '==' and '!='
 * B_AND B_OR   : the binary operator '&&' '||'   : binary operator '&&' and '||'
 */
// nType: node type
enum nType {NONE = 0, PAR,   L_VAR, G_VAR, 
            RET,      NEG,   N_NOT, 
            B_MUL,    B_DIV, B_SUB, B_ADD, B_GR,  B_GE,
            B_LS,     B_LE,  B_EQ,  B_UEQ, B_AND, B_OR};

/**
 * @brief Base AST node
 * virtual class of all AST node
 */
class BaseAST
{
public:
    Val val;                      // union type of node value
    std::string dtype;            // data type
    bool isArray; 
    nType ntype;                  // node type
    std::vector<BaseAST *> child; // child node

    BaseAST();
    virtual llvm::Value *Codegen() = 0;
    virtual ~BaseAST() {};

    /* different versions of setting AST properties */
    void setAST(Val val);
    void setAST(Val val, std::string type);
    void setAST(Val val, std::string type, bool isArray);
    void setAST(Val val, std::string type, nType ntype);
    void setAST(Val val, std::string type, bool isArray, nType ntype);
    
    // add child(ren) to this BST node
    void addChild(BaseAST *node);

    // cast the Value type to what we want
    Value *cast(Value *val, std::string origType, std::string retType);
    
    // fetch type according to this dType
    Type *getType();

    // override the operator "<<"
    friend std::ostream& operator<< (std::ostream& out, BaseAST *AST);

    // print AST on the terminal
    // in level traversal
    void prtTree();

    // convert a character to a string
    std::string restoreChar(char ch);

    // convert a string which may contain escape character to one doesn't
    std::string restoreStr(std::string str);

    // output AST as a JSON file to visualize it
    void visual();

};

/**
 * @brief token AST returned by lexer
 * accepted in parser
 * NOTE: not a part of AST
 */
class TokenAST : public BaseAST
{
public:
    // different construtor of TokenAST
    TokenAST();
    TokenAST(Val val);
    TokenAST(Val val, std::string type);
    ~TokenAST();
    llvm::Value *Codegen();
};

/**
 * @brief root of AST
 */
class DocAST : public BaseAST
{
public:
    // different constructor of DocAST
    DocAST();
    DocAST(int n);
    ~DocAST();
    
    llvm::Value *Codegen();
};

/**
 * @brief function node
 */
class FuncAST : public BaseAST
{
public:
    // different constructor of FuncAST
    FuncAST();
    FuncAST(Val val, std::string type);
    ~FuncAST();
    llvm::Value *Codegen();
};

/**
 * @brief function call node
 */
class FuncCallAST : public BaseAST
{
public:
    // different constructor of FuncCallAST
    FuncCallAST();
    FuncCallAST(Val val, std::string type);
    ~FuncCallAST();
    llvm::Value *Codegen();
};

/**
 * @brief return statement node
 */
class RetAST : public BaseAST
{
public:
    // constructor of RetAST
    RetAST();
    RetAST(Val val, std::string type);
    ~RetAST();

    llvm::Value *Codegen();
};

/**
 * @brief definition node
 * include:
 * definition of local variables
 * definition of glocal variables
 * NOTE: array excluded
 */
class DefAST : public BaseAST
{
public:
    int level;

    // different constructor of DefAST
    DefAST();
    DefAST(std::string type, Val val, int level);
    DefAST(std::string type, Val val, BaseAST *exp, int level);
    ~DefAST();
    llvm::Value *Codegen();
};

/**
 * @brief condition statement node
 * parent of ifAST & elseAST
 */
class CondAST : public BaseAST
{
public:
    // different constructor of CondAST
    CondAST();
    CondAST(Val val, std::string type);
    ~CondAST();
    // common case: if - n*[else if] - else
    // special case: single if
    llvm::Value *Codegen();
};


/**
 * @brief if/[else if] node
 * first child: exp
 * following children: statement in block
 */
class IfAST : public BaseAST
{
public:
    // constructor and disconstuctor of IfAST
    IfAST();
    IfAST(Val val, std::string type);
    ~IfAST();

    llvm::Value *Codegen();
};

/**
 * @brief else node
 * children: statement in block
 */
class ElseAST : public BaseAST
{
public:
    ElseAST();
    ElseAST(Val val, std::string type);
    ~ElseAST();
    
    llvm::Value *Codegen();
};

/**
 * @brief declaration node
 * NOTE: only for parameter declaration in function
 */
class DecAST : public BaseAST
{
public:
    DecAST();
    DecAST(Val val, std::string type, bool isArray);
    ~DecAST();
    llvm::Value *Codegen();
};

/**
 * @brief type node
 * store info of type: INT, FLOAT, CHAR, VOID
 * NOTE: not a part of AST
 */
class TypeAST : public BaseAST
{
public:
    TypeAST();
    TypeAST(std::string type);
    ~TypeAST();
    llvm::Value *Codegen();
};

/**
 * @brief id node
 * store info of id
 */
class IdAST : public BaseAST
{
public:
    
    IdAST();
    IdAST(Val val);
    ~IdAST();
    llvm::Value *Codegen();
};

/**
 * @brief exp node
 * for single token expression
 * e.g. local var, global var, Constant
 */
class ExpAST : public BaseAST
{
public:
    ExpAST();
    ExpAST(Val val, std::string type);
    ExpAST(Val val, std::string type, nType ntype);
    ~ExpAST();
    
    llvm::Value *Codegen();
};

/**
 * @brief assign exp node
 * NOTE: only for variable,
 * array excluded
 */
class AssignAST : public BaseAST
{
public:
    AssignAST();
    AssignAST(Val val, std::string type,nType ntype);
    ~AssignAST();
    llvm::Value *Codegen();
};

/**
 * @brief uniop exp node
 * for NOT & NEG
 */
class UniOpAST : public BaseAST
{
public:
    bool isTop = true;
    UniOpAST();
    UniOpAST(Val val, std::string type, nType ntype);
    ~UniOpAST();
    llvm::Value *Codegen();
};

/**
 * @brief binary exp node
 */
class BinaryOpAST : public BaseAST{
public:
    BinaryOpAST();
    BinaryOpAST(Val val, std::string type, nType ntype);
    ~BinaryOpAST();
    llvm::Value *Codegen();

};

/**
 * @brief addr exp node
 * for get addr of variable
 * NOTE: array excluded
 */
class AddrAST : public BaseAST
{
public:
    AddrAST();
    AddrAST(Val val, std::string type,nType ntype);
    ~AddrAST();
    llvm::Value *Codegen();
};

/**
 * @brief loop node
 * while & for
 */
class LoopAST : public BaseAST{
public:
    LoopAST();
    LoopAST(Val val, std::string type);
    ~LoopAST();
    llvm::Value *Codegen();
};

/**
 * @brief br node
 * continue & break
 */
class BrAST : public BaseAST{
public:
    BrAST();
    BrAST(Val val, std::string type);
    ~BrAST();
    llvm::Value *Codegen();

};

/**
 * @brief arr node
 * 3 types operations:
 * - declaration;
 * - assignment;
 * - load value.
 */
class ArrAST : public BaseAST
{
public:
    bool isDef; // true: def; false: load / store
    bool isAssign; // true: store; false: load
    int arrLevel;
    std::string id;
    int dim;

    ArrAST();
    ArrAST(Val val, std::string type, int arrLevel);
    ~ArrAST();
    llvm::Value *Codegen();

    // declaration of the array
    llvm::Value *dec();

    // reference of the array
    llvm::Value *ref(bool isAssign);
};

#endif /*AST.h*/
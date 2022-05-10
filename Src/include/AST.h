#ifndef _AST_H_
#define _AST_H_

#include<string>
#include<vector>
#include<queue>
#include<variant>
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
#include"../include/IO.h"

using namespace llvm;

typedef std::variant<int, double, char, std::string> Val;

extern std::unique_ptr<LLVMContext> context;
extern std::unique_ptr<Module> mod;
extern std::unique_ptr<IRBuilder<> > builder;
extern Function *fly;
extern std::map<std::string, GlobalVariable*> Global_Values;
/*
llvm::Value *ErrorVal(const char *str)
{ 
    llvm::Error(str); 
    return 0; 
}
*/

enum nType {NONE = 0, PAR, N_VAR,L_VAR,G_VAR, RET, NEG, N_NOT,B_MUL,B_DIV,B_SUB,B_ADD,B_GR,B_GE,B_LS,B_LE,B_EQ,B_UEQ,B_AND,B_OR};

class BaseAST
{
public:
    Val val;
    std::string dtype;
    bool isArray;
    nType ntype;
    std::vector<BaseAST *> child;

    BaseAST() {};
    virtual llvm::Value *Codegen() = 0;
    virtual ~BaseAST() {};

    void setAST(Val val) {
        this->val = val;
        this->isArray = false;
        this->ntype = NONE;
    }

    void setAST(Val val, std::string type) {
        this->val = val;
        this->dtype = type;
        this->isArray = false;
        this->ntype = NONE;
    }

    void setAST(Val val, std::string type, bool isArray) {
        this->val = val;
        this->dtype = type;
        this->isArray = isArray;
        this->ntype = NONE;
    }

    void setAST(Val val, std::string type, nType ntype) {
        this->val = val;
        this->dtype = type;
        this->isArray = false;
        this->ntype = ntype;
    }

    void setAST(Val val, std::string type, bool isArray, nType ntype) {
        this->val = val;
        this->dtype = type;
        this->isArray = isArray;
        this->ntype = ntype;
    }

    void addChild(BaseAST *node) {
        child.push_back(node);
    }

    Value *cast(Value *val, std::string origType, std::string retType) {
        if(retType == "FLOAT") {
            if(origType == "INT") return builder->CreateSIToFP(val, builder->getDoubleTy());
            else if(origType == "CHAR") return builder->CreateUIToFP(val, builder->getDoubleTy());
        }
        else if(retType == "INT") {
            if(origType == "FLOAT") return builder->CreateFPToSI(val, builder->getInt32Ty());
            else if(origType == "CHAR") return builder->CreateSExt(val, builder->getInt32Ty());
        }
        else if(retType == "CHAR") {
            if(origType == "FLOAT") return builder->CreateFPToUI(val, builder->getInt8Ty());
            else if(origType == "INT") return builder->CreateTrunc(val, builder->getInt8Ty());
        }
        return val;
    }

    Type *getType() {
        Type *type;
        if(this->isArray) {
            if(this->dtype == "INT") {
                type = PointerType::getInt32PtrTy(*context);
            }
            else if(this->dtype == "FLOAT") {
                type = PointerType::getDoublePtrTy(*context);
            }
            else if(this->dtype == "CHAR") {
                type = PointerType::getInt8PtrTy(*context);
            }
        }
        else {
            if(this->dtype == "INT") {
                type = builder->getInt32Ty();
            }
            else if(this->dtype == "FLOAT") {
                type = builder->getDoubleTy();
            }
            else if(this->dtype == "CHAR") {
                type = builder->getInt8Ty();
            }
            else if(this->dtype == "VOID") {
                type = builder->getVoidTy();
            }
        }
        
        return type;
    }

    friend std::ostream& operator<< (std::ostream& out, BaseAST *AST) {
        if(std::get_if<int>(&(AST->val)) != nullptr) {
            out << std::get<int>(AST->val);
        }
        else if(std::get_if<double>(&(AST->val)) != nullptr) {
            out << std::get<double>(AST->val);
        }
        else if(std::get_if<char>(&(AST->val)) != nullptr) {
            out << std::get<char>(AST->val);
        }
        else if(std::get_if<std::string>(&(AST->val)) != nullptr) {
            out << std::get<std::string>(AST->val);
        }
        out << "(dtype: " << AST->dtype << ")";
        return out;
    }

    void prtTree() {
        std::queue<BaseAST *> nodeQueue;
        BaseAST *node = this;
        std::cout << this << std::endl;
        nodeQueue.push(this);
        while(!nodeQueue.empty()) {
            node = nodeQueue.front();
            nodeQueue.pop();
            for(auto i = 0; i < node->child.size(); i++) {
              std::cout << node->child[i];
              std::cout << " parent :[" << node << "]" << std::endl;
              nodeQueue.push(node->child[i]);
            }
        }
    }

};

class TokenAST : public BaseAST
{
public:
    TokenAST() {}
    TokenAST(Val val) {
        this->setAST(val);
    }
    TokenAST(Val val, std::string type) {
        this->setAST(val, type);
    }
    ~TokenAST() {}
    llvm::Value *Codegen() {
        return NULL;
    }
};

class DocAST : public BaseAST
{
public:
    DocAST() {}
    DocAST(int n) {
        this->val = "doc";
        this->dtype = "VOID";
    }
    ~DocAST() {}
    llvm::Value *Codegen() {
        initIO();

        for(int i = 0; i < this->child.size(); i++) {
            this->child[i]->Codegen();
        }
        return NULL;
    }
};



class CompoundAST : public BaseAST
{
public:
    // TODO:
    llvm::Value *Codegen() {
        return NULL;
    }
    ~CompoundAST() {}
};


class ParListAST : public BaseAST
{
public:
    // TODO:
    llvm::Value *Codegen() {
        return NULL;
    }
    ~ParListAST() {}
};

class FuncAST : public BaseAST
{
public:
    FuncAST() {}
    FuncAST(Val val, std::string type) {
        this->val = val;
        this->dtype = type;
    }
    ~FuncAST() {}

    llvm::Value *Codegen() {
        FunctionType *funcType;
        // create par-type list
        std::vector<Type *> parType;
        //std::cout << this->child[0];
        for (int i = 0; i < this->child.size() && this->child[i]->ntype == PAR; i++) {
            Type *type = this->child[i]->getType();
            parType.push_back(type);
        }

        if(this->dtype == "INT") {
            funcType = FunctionType::get(builder->getInt32Ty(), parType, false);
        }
        else if(this->dtype == "FLOAT") {
            funcType = FunctionType::get(builder->getDoubleTy(), parType, false);
        }
        else if(this->dtype == "CHAR") {
            funcType = FunctionType::get(builder->getInt8Ty(), parType, false);
        }
        else if(this->dtype == "VOID") {
            funcType = FunctionType::get(builder->getVoidTy(), parType, false);
        }
        Function *func = Function::Create(funcType, Function::ExternalLinkage, 
                                          std::get<std::string>(this->val), *mod);
        if(std::get<std::string>(this->val) == "fly") fly = func;
        
        BasicBlock *entry = BasicBlock::Create(*context, "entry"+std::get<std::string>(this->val),
                                               func);
        // set names for parameters
        builder->SetInsertPoint(entry); 

        //Function *callee = mod->getFunction("printf");
        //builder->CreateCall(callee, {builder->CreateGlobalStringPtr("%s\n"), builder->CreateGlobalStringPtr("hello world!")});
        //builder->CreateCall(callee, {builder->CreateGlobalStringPtr("in llvm fun, value = %d\n"), ConstantInt::get(Type::getInt32Ty(*context), APInt(32, 1))});

        int i = 0;
        for (auto iter = func->args().begin(); iter != func->args().end(); iter++) {
            // std::cout << this->child[i];
            std::string strId = std::get<std::string>(this->child[i]->val);
            BaseAST *node = this->child[i++];
            AllocaInst *alloc;
            if(node->isArray) {
                if(node->dtype == "INT") {
                    alloc = builder->CreateAlloca(PointerType::getInt32PtrTy(*context), nullptr, strId);
                }
                else if(node->dtype == "FLOAT") {
                    alloc = builder->CreateAlloca(PointerType::getDoublePtrTy(*context), nullptr, strId);
                                                            
                }
                else if(node->dtype == "CHAR") {
                    alloc = builder->CreateAlloca(PointerType::getInt8PtrTy(*context), nullptr, strId);
                }
            }
            else {
                if(node->dtype == "INT") {
                    alloc = builder->CreateAlloca(builder->getInt32Ty(), nullptr, strId); 
                }
                else if(node->dtype == "FLOAT") {
                    alloc = builder->CreateAlloca(builder->getDoubleTy(), nullptr, strId); 
                }
                else if(node->dtype == "CHAR") {
                    alloc = builder->CreateAlloca(builder->getInt8Ty(), nullptr, strId); 
                }
            }
            builder->CreateStore(iter, alloc);
        }

        
        for(int i = 0; i < this->child.size(); i++) {
            builder->SetInsertPoint(entry); 
            this->child[i]->Codegen();
            if(this->child[i]->ntype == RET) break;
        }
        return NULL;
    }
};

class FuncCallAST : public BaseAST
{
public:
    FuncCallAST() {}
    FuncCallAST(Val val, std::string type) {
        this->setAST(val, type);
    }
    ~FuncCallAST() {}
    llvm::Value *Codegen() {
        std::string strId = std::get<std::string>(this->val);
        llvm::Function *callee = mod->getFunction(strId);

/*
        if(callee == NULL) {
            if(strId == "printf") callee = createPrintf();
            else if(strId == "scanf") callee = createScanf();
            else if(strId == "getchar") callee = createGetchar();
            builder->GetInsertBlock()->getParent();
        } 
*/
        // parameters
        std::vector<Value *> args;
        for (int i = 0; i < this->child.size(); i++) {
            std::cout << this->child[i] << std::endl; 
            args.push_back(this->child[i]->Codegen());
        }

        /*
        // consider variant parameter number & type
        if(strId == "printf" || strId == "scanf") {
            callee = mod->getFunction(strId);
            std::vector<Type *> IOFuncArgs;
            IOFuncArgs.push_back(builder->getInt8PtrTy());
            for(int i = 1; i < this->child.size(); i++) {
                IOFuncArgs.push_back(args[i]->getType());
            }
            FunctionType *funcType = FunctionType::get(builder->getInt32Ty(), IOFuncArgs, true);
            FunctionCallee IOCallee = mod->getOrInsertFunction(strId, funcType);
            return builder->CreateCall(IOCallee, args);
        }
        */
        //return builder->CreateCall(callee, args);
        std::cout << "before call" << std::endl;
        Value *Val = builder->CreateCall(callee, args);
        std::cout << "after call" << std::endl;

        return Val;
    }
};


class RetAST : public BaseAST
{
public:
    RetAST() {}
    RetAST(Val val, std::string type) {
        this->setAST(val, type, RET);
    }
    llvm::Value *Codegen() {
        Value *retVal = this->child[0]->Codegen();
        retVal = this->cast(retVal, this->child[0]->dtype, this->dtype);
        return builder->CreateRet(retVal);
    }
};

class DefAST : public BaseAST
{
public:
    int level;
    DefAST() {}
    DefAST(std::string type, Val val, bool isArray, int level) {
        this->setAST(val, type, isArray);
        this->level = level;
    }
    DefAST(std::string type, Val val, bool isArray, BaseAST *exp, int level) {
        this->setAST(val, type, isArray);
        this->level = level;
        this->child.push_back(exp);
    }
    ~DefAST() {}
    llvm::Value *Codegen() {
        std::string strId = std::get<std::string>(this->val);
        // local var
        if(level > 0) {
            AllocaInst *alloc;
            if(this->dtype == "INT") {
                alloc = builder->CreateAlloca(builder->getInt32Ty(), nullptr, strId); 
            }
            else if(this->dtype == "FLOAT") {
                alloc = builder->CreateAlloca(builder->getDoubleTy(), nullptr, strId); 
            }
            else if(this->dtype == "CHAR") {
                alloc = builder->CreateAlloca(builder->getInt8Ty(), nullptr, strId); 
            }
            if(!this->child.empty()) {
                Value *val = this->child[0]->Codegen();
                val = this->cast(val, this->child[0]->dtype, this->dtype);
                builder->CreateStore(val, alloc);
            }  
        }
        // global var
        else {
            Type *type = this->getType();
            mod->getOrInsertGlobal(strId, type);
            GlobalVariable *val = mod->getNamedGlobal(strId);
            if(this->dtype == "INT") {
                val->setInitializer(Constant::getIntegerValue(type, APInt(32, 0)));
            }
            else if(this->dtype == "FLOAT") {
                val->setInitializer(ConstantFP::get(type, 0.0));
            }
            else if(this->dtype == "CHAR") {
                val->setInitializer(Constant::getIntegerValue(type, APInt(8, 0)));
            }
            Global_Values[strId] = val;
            if(!this->child.empty()) {
                Value *l = this->child[0]->Codegen();
                l = this->cast(l, this->child[0]->dtype, this->dtype);
                if(this->dtype == "INT") {
                    val->setInitializer(llvm::cast<ConstantInt>(l));
                }
                else if(this->dtype == "FLOAT") {
                    val->setInitializer(llvm::cast<ConstantFP>(l));
                }
                else if(this->dtype == "CHAR") {
                    val->setInitializer(llvm::cast<ConstantInt>(l));
                }
            }  
        }

        return NULL;
    }
};


class IfAST : public BaseAST
{
public:
    IfAST() {}
    IfAST(Val val, std::string type) {
        this->setAST(val, type);
    }
    ~IfAST() {}
    llvm::Value *Codegen() {
        return NULL;
    }
};


class DecAST : public BaseAST
{
public:
    DecAST() {}
    DecAST(Val val, std::string type, bool isArray) {
        this->setAST(val, type, isArray, PAR);
    }
    llvm::Value *Codegen() {
        return NULL;
    }
};

class TypeAST : public BaseAST
{
public:
    // TODO:
    llvm::Value *Codegen() {
        return NULL;
    }
    TypeAST() {}
    TypeAST(std::string type) {
        this->dtype = type;
    }
    ~TypeAST() {}
};

class IdAST : public BaseAST
{
public:
    // TODO:
    llvm::Value *Codegen() {
        return NULL;
    }
    IdAST() {}
    IdAST(Val val) {
        this->val = val;
    }
    ~IdAST() {}
};

class ExpAST : public BaseAST
{
public:
    ExpAST() {}
    ExpAST(Val val, std::string type) {
        this->setAST(val, type);
    }
    ExpAST(Val val, std::string type, nType ntype) {
        this->setAST(val, type, ntype);
    }
    ~ExpAST() {}
    llvm::Value *Codegen() {
        if(this->ntype == L_VAR) {
            std::string strId = std::get<std::string>(this->val);
            ValueSymbolTable *table = builder->GetInsertBlock()->getValueSymbolTable();
            return builder->CreateLoad(table->lookup(strId));
        }
        else  if(this->ntype ==G_VAR) {
            std::string strId = std::get<std::string>(this->val);
            return builder->CreateLoad(Global_Values[strId]);

        }
        else {
            if(this->dtype == "INT") {
                return ConstantInt::get(Type::getInt32Ty(*context), std::get<int>(this->val));
            }
            else if(this->dtype == "CHAR") {
                return ConstantInt::get(Type::getInt8Ty(*context), std::get<char>(this->val));
            }
            else if(this->dtype == "FLOAT") {
                return ConstantFP::get(Type::getDoubleTy(*context), std::get<double>(this->val));
            }
            else if(this->dtype == "LITERAL") {
                std::string strId = std::get<std::string>(this->val);
                return builder->CreateGlobalStringPtr(strId, ".str", 0);
                //llvm::Constant *str = llvm::ConstantDataArray::getString(*context, std::get<std::string>(this->val));
                //return new llvm::GlobalVariable(*mod, str->getType(), true, llvm::GlobalValue::PrivateLinkage, str);
            }
        }
        return NULL;
    }
};

class AssignAST : public BaseAST
{
public:
    AssignAST() {}
    AssignAST(Val val, std::string type,nType ntype) {
        this->setAST(val, type,ntype);
    }
    ~AssignAST() {}
    llvm::Value *Codegen() {
        Value *val_right = this->child[1]->Codegen();
        ValueSymbolTable *table = builder->GetInsertBlock()->getValueSymbolTable();
        std::string strId = std::get<std::string>(this->child[0]->val);
        // TODO: array
        if(this->child[0]->isArray) {
            Value *val_left = this->child[0]->Codegen();
        }
        else {
            if(this->ntype ==L_VAR){
            val_right = this->cast(val_right, this->child[1]->dtype, this->dtype);
            builder->CreateStore(val_right, table->lookup(strId));
            return val_right;
            }
            else {
            val_right = this->cast(val_right, this->child[1]->dtype, this->dtype);
            builder->CreateStore(val_right, Global_Values[strId]);
            return val_right;
            }
        }
        
        return NULL;
    }
};


class UniOpAST : public BaseAST
{
public:
    bool isTop = true;
    UniOpAST() {}
    UniOpAST(Val val, std::string type, nType ntype) {
        this->val = val;
        this->dtype = type;
        this->ntype = ntype;
    }
    ~UniOpAST() {}
    llvm::Value *Codegen() {
        Value *val = this->child[0]->Codegen();
        if(ntype == NEG) {
            if(this->dtype == "INT") {
                Value *l = ConstantInt::get(Type::getInt32Ty(*context), 0);
                return builder->CreateSub(l, val, "subtmpi");
            }
            else if(this->dtype == "FLOAT") {
                Value *l = ConstantFP::get(Type::getDoubleTy(*context), 0.0);
                return builder->CreateFSub(l, val, "subtmpf");
            }
        }
        else if(ntype == N_NOT) {
            
            if(this->dtype == "FLOAT") {
                if(this->child[0]->ntype != N_NOT) {
                    Value *l = ConstantFP::get(Type::getDoubleTy(*context), 0.0);
                    val = builder->CreateFCmpUNE(l, val, "cmpneqf");
                }
                val = builder->CreateNot(val);
                if(this->isTop) {
                    val = builder->CreateUIToFP(val, Type::getDoubleTy(*context));
                }
                return val;
            }
            else if(this->dtype == "INT" || this->dtype == "CHAR") {
                if(this->child[0]->ntype != N_NOT) {
                    Value *l; 
                    if(this->dtype == "INT")
                        l = ConstantInt::get(Type::getInt32Ty(*context), 0);
                    else l = ConstantInt::get(Type::getInt8Ty(*context), 0);
                    val = builder->CreateICmpNE(l, val, "cmpneqi");
                }
                val = builder->CreateNot(val);
                if(this->isTop) {
                    val = builder->CreateZExt(val, Type::getInt32Ty(*context));
                }
                return val;
            }
        }

        return NULL;
    }
};

class BinaryOpAST : public BaseAST{
    public:
    BinaryOpAST() {}
    BinaryOpAST(Val val, std::string type, nType ntype) {
        this->val = val;
        this->dtype = type;
        this->ntype = ntype;
    }
    ~BinaryOpAST() {}
    llvm::Value *Codegen() {
        Value *val_left = this->child[0]->Codegen();
        Value *val_right = this->child[1]->Codegen();
        Value *val_res;
        
        if(this->dtype == "FLOAT") {
            val_left  = builder->CreateSIToFP(val_left ,Type::getDoubleTy(*context));
            val_right = builder->CreateSIToFP(val_right,Type::getDoubleTy(*context));
        }
        else if(this->dtype == "INT") {
            val_left  = builder->CreateZExt(val_left ,Type::getInt32Ty(*context));
            val_right = builder->CreateZExt(val_right,Type::getInt32Ty(*context));
        }

        if(ntype == B_MUL) {
            if(this->dtype == "FLOAT") return builder->CreateFMul(val_left, val_right, "multmp");
            else                       return builder->CreateMul(val_left, val_right, "multmp");
        }
        else if(ntype == B_DIV) {
            if(this->dtype == "FLOAT")  return builder->CreateFDiv(val_left, val_right, "divtmp");
            else                        return builder->CreateSDiv(val_left, val_right, "divtmp");
        }
        else if(ntype == B_ADD) {
            if(this->dtype == "FLOAT")      return builder->CreateFAdd(val_left, val_right, "addtmp");
            else if(this->dtype == "INT") return builder->CreateAdd(val_left, val_right, "addtmp");
            else {
                val_res = builder->CreateAdd(val_left, val_right, "addtmp");
                return builder->CreateTrunc(val_res,Type::getInt8Ty(*context));
            }
        }
        else if(ntype == B_SUB) {
            if(this->dtype == "FLOAT")      return builder->CreateFSub(val_left, val_right, "subtmp");
            else if(this->dtype == "INT") return builder->CreateSub(val_left, val_right, "subtmp");
            else {
                val_res = builder->CreateSub(val_left, val_right, "subtmp");
                return builder->CreateTrunc(val_res,Type::getInt8Ty(*context));
            }
        }
        else if(ntype == B_GR) {
            if(this->dtype == "FLOAT") {
                val_res = builder ->CreateFCmpUGT(val_left,val_right);
                return builder->CreateUIToFP(val_res, Type::getDoubleTy(*context));
            }
            else {
                val_res = builder ->CreateICmpSGT(val_left,val_right);
                if(this->dtype == "INT")
                    return builder->CreateZExt(val_res, Type::getInt32Ty(*context));
                else if(this->dtype == "CHAR")
                    return builder->CreateZExtOrTrunc(val_res, Type::getInt8Ty(*context));
            }
        }
        else if(ntype == B_GE) {
            if(this->dtype == "FLOAT") {
                val_res = builder ->CreateFCmpUGE(val_left,val_right);
                return builder->CreateUIToFP(val_res, Type::getDoubleTy(*context));
            }
            else {
                val_res = builder ->CreateICmpSGE(val_left,val_right);
                if(this->dtype == "INT")
                    return builder->CreateZExt(val_res, Type::getInt32Ty(*context));
                else if(this->dtype == "CHAR")
                    return builder->CreateZExtOrTrunc(val_res, Type::getInt8Ty(*context));
            }

        }
        else if(ntype == B_LS) {
            if(this->dtype == "FLOAT") {
                val_res = builder ->CreateFCmpULT(val_left,val_right);
                return builder->CreateUIToFP(val_res, Type::getDoubleTy(*context));
            }
            else {
                val_res = builder ->CreateICmpSLT(val_left,val_right);
                if(this->dtype == "INT")
                    return builder->CreateZExt(val_res, Type::getInt32Ty(*context));
                else if(this->dtype == "CHAR")
                    return builder->CreateZExtOrTrunc(val_res, Type::getInt8Ty(*context));
            }
        }
        else if(ntype == B_LE) {
            if(this->dtype == "FLOAT") {
                val_res = builder ->CreateFCmpULE(val_left,val_right);
                return builder->CreateUIToFP(val_res, Type::getDoubleTy(*context));
            }
            else {
                val_res = builder ->CreateICmpSLE(val_left,val_right);
                if(this->dtype == "INT")
                    return builder->CreateZExt(val_res, Type::getInt32Ty(*context));
                else if(this->dtype == "CHAR")
                    return builder->CreateZExtOrTrunc(val_res, Type::getInt8Ty(*context));
            }
        }
        else if(ntype == B_EQ) {
            if(this->dtype == "FLOAT") {
                val_res = builder ->CreateFCmpUEQ(val_left,val_right);
                return builder->CreateUIToFP(val_res, Type::getDoubleTy(*context));
            }
            else {
                val_res = builder ->CreateICmpEQ(val_left,val_right);
                if(this->dtype == "INT")
                    return builder->CreateZExt(val_res, Type::getInt32Ty(*context));
                else if(this->dtype == "CHAR")
                    return builder->CreateZExtOrTrunc(val_res, Type::getInt8Ty(*context));
            }
        }
        else if(ntype == B_UEQ) {
            if(this->dtype == "FLOAT") {
                val_res = builder ->CreateFCmpUNE(val_left,val_right);
                return builder->CreateUIToFP(val_res, Type::getDoubleTy(*context));
            }
            else {
                val_res = builder ->CreateICmpNE(val_left,val_right);
                if(this->dtype == "INT")
                    return builder->CreateZExt(val_res, Type::getInt32Ty(*context));
                else if(this->dtype == "CHAR")
                    return builder->CreateZExtOrTrunc(val_res, Type::getInt8Ty(*context));
            }
        }
        else if(ntype == B_AND) {
            val_left = builder->CreateFPToSI(val_left,  Type::getInt32Ty(*context));
            val_right= builder->CreateFPToSI(val_right, Type::getInt32Ty(*context));
            val_res = builder ->CreateAnd(val_left,val_right);
            Value *l = ConstantInt::get(Type::getInt32Ty(*context), 0);
            val_res = builder->CreateICmpNE(l, val_res, "cmpneqi");
            if(this->dtype == "FLOAT") return builder->CreateUIToFP(val_res, Type::getDoubleTy(*context));
            else {
                Value *val = builder->CreateZExt(val_res, Type::getInt32Ty(*context));
                if(this->dtype == "CHAR") val = builder->CreateTrunc(val_res, Type::getInt8Ty(*context)); 
                return val;
            }
        }
        else if(ntype == B_OR) {
            val_left = builder->CreateFPToSI(val_left,  Type::getInt32Ty(*context));
            val_right= builder->CreateFPToSI(val_right, Type::getInt32Ty(*context));
            val_res = builder ->CreateOr(val_left,val_right);
            Value *l = ConstantInt::get(Type::getInt32Ty(*context), 0);
            val_res = builder->CreateICmpNE(l, val_res, "cmpneqi");
            if(this->dtype == "FLOAT") return builder->CreateUIToFP(val_res, Type::getDoubleTy(*context));
            else {
                Value *val = builder->CreateZExt(val_res, Type::getInt32Ty(*context));
                if(this->dtype == "CHAR") val = builder->CreateTrunc(val_res, Type::getInt8Ty(*context)); 
                return val;
            }
        }
     return NULL;
    }

};

class AddrAST : public BaseAST
{
public:
    AddrAST() {}
    AddrAST(Val val, std::string type,nType ntype) {
        this->setAST(val, type,ntype);
    }
    ~AddrAST() {}
    llvm::Value *Codegen() {
        ValueSymbolTable *table = builder->GetInsertBlock()->getValueSymbolTable();
        std::string strId = std::get<std::string>(this->val);
        /*AllocaInst *alloc;
        if(this->dtype == "INT") {
            alloc = builder->CreateAlloca(PointerType::getInt32PtrTy(*context), nullptr);
        }
        else if(this->dtype == "FLOAT") {
            alloc = builder->CreateAlloca(PointerType::getDoublePtrTy(*context), nullptr);
                                                    
        }
        else if(this->dtype == "CHAR") {
            alloc = builder->CreateAlloca(PointerType::getInt8PtrTy(*context), nullptr);
        }
        if(this->ntype ==L_VAR){
        builder->CreateStore( table->lookup(strId),alloc);
        }
        else {
        builder->CreateStore( Global_Values[strId],alloc);
        }
        return alloc;*/
        Value *v;
        if(this->ntype ==L_VAR){
            v = table->lookup(strId);
        }
        else {
            v = Global_Values[strId];
        }
        if(this->dtype == "INT") {
            return builder->CreateIntToPtr(v,PointerType::getInt32PtrTy(*context));
        }
        else if(this->dtype == "FLOAT") {
            return builder->CreateIntToPtr(v,PointerType::getDoublePtrTy(*context));
        }
        else if(this->dtype == "CHAR") {
             return builder->CreateIntToPtr(v,PointerType::getInt8PtrTy(*context));
        }
        return NULL;
    }
};


#endif
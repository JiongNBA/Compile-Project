/* ----------------------------------------------------------
 * file: AST.cpp
 * author: Haofeng Yang 3190105301@zju.edu.cn
 *         Yuesong Wang 3190105303@zju.edu.cn
 *         Jiong   Li   3190105725@zju.edu.cn
 * copyright: © 2022 authors. All rights reserved.
 * ----------------------------------------------------------
 * function: header file of AST nodes
 * description: define basic AST node, common methonds, and 
 *              AST nodes of different grammarical symbols.
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

#include"../include/AST.h"

using namespace std;
using namespace llvm;

/* ---------------------------- BaseAST: Base class of AST node ---------------------------- */
BaseAST::BaseAST() {};

void BaseAST::setAST(Val val) {
        this->val = val;
        this->isArray = false;
        this->ntype = NONE;
    }
void BaseAST::setAST(Val val, string type) {
        this->val = val;
        this->dtype = type;
        this->isArray = false;
        this->ntype = NONE;
    }
void BaseAST::setAST(Val val, string type, bool isArray) {
        this->val = val;
        this->dtype = type;
        this->isArray = isArray;
        this->ntype = NONE;
    }
void BaseAST::setAST(Val val, string type, nType ntype) {
        this->val = val;
        this->dtype = type;
        this->isArray = false;
        this->ntype = ntype;
    }
void BaseAST::setAST(Val val, string type, bool isArray, nType ntype) {
        this->val = val;
        this->dtype = type;
        this->isArray = isArray;
        this->ntype = ntype;
    }
void BaseAST::addChild(BaseAST *node) {
        child.push_back(node);
    } 

Value* BaseAST::cast(Value *val, string origType, string retType) { 
        if(retType == "FLOAT") {
            if(origType == "INT") return builder->CreateSIToFP(val, builder->getDoubleTy());        // float -> int
            else if(origType == "CHAR") return builder->CreateUIToFP(val, builder->getDoubleTy());  // float -> char
        }
        else if(retType == "INT") {
            if(origType == "FLOAT") return builder->CreateFPToSI(val, builder->getInt32Ty());       // int -> float
            else if(origType == "CHAR") return builder->CreateSExt(val, builder->getInt32Ty());     // int ->char
        }
        else if(retType == "CHAR") {
            if(origType == "FLOAT") return builder->CreateFPToUI(val, builder->getInt8Ty());        // char -> float
            else if(origType == "INT") return builder->CreateZExtOrTrunc(val, builder->getInt8Ty());// char -> int
        }
        else if(retType == "BOOL") {
            if(origType == "FLOAT") return builder->CreateFPToUI(val, builder->getInt1Ty());        // float -> bool
            else if(origType == "INT") {
                Value *l = ConstantInt::get(builder->getInt32Ty(), 0);
                return val = builder->CreateICmpNE(l, val, ".cmpneqi");                             // int -> bool
            }
            else if(origType == "CHAR" ) {
                Value *l = ConstantInt::get(builder->getInt8Ty(), 0);
                return val = builder->CreateICmpNE(l, val, ".cmpneqc");                             // char -> bool
            }
        }
        return val;
    } 

Type* BaseAST::getType() {
        Type *type;
        // array in function parameters
        if(this->isArray && this->ntype == PAR) {
            if(!this->dtype.empty()) {
                switch(this->dtype[0]) {
                    case 'I': type = PointerType::getInt32PtrTy(*context);  break; // int *
                    case 'F': type = PointerType::getDoublePtrTy(*context); break; // float *
                    case 'C': type = PointerType::getInt8PtrTy(*context);   break; // char *
                }
            }
        }
        // other case
        else {
            if(!this->dtype.empty()) {
                switch(this->dtype[0]) {
                    case 'I': type = builder->getInt32Ty();  break; // return the int   type
                    case 'F': type = builder->getDoubleTy(); break; // return the float type
                    case 'C': type = builder->getInt8Ty();   break; // return the char  type
                    case 'V': type = builder->getVoidTy();   break; // return the void  type
                }
            }
        }
        
        return type;
    }

ostream& operator<< (ostream& out, BaseAST *AST) {
    if(get_if<int>(&(AST->val)) != nullptr) {              // int
        out << get<int>(AST->val);
    }
    else if(get_if<double>(&(AST->val)) != nullptr) {      // float
        out << get<double>(AST->val);
    }
    else if(get_if<char>(&(AST->val)) != nullptr) {        // char
        out << get<char>(AST->val);
    }
    else if(get_if<string>(&(AST->val)) != nullptr) { // string
        out << get<string>(AST->val);
    }
    out << "(dtype: " << AST->dtype << ")";                     // print dtype
    return out;
}
void BaseAST::prtTree() {
    queue<BaseAST *> nodeQueue;
    BaseAST *node = this;
    cout << this << endl;
    nodeQueue.push(this);
    while(!nodeQueue.empty()) {
        node = nodeQueue.front();
        nodeQueue.pop();
        for(auto i = 0; i < node->child.size(); i++) {
            cout << node->child[i];
            cout << " parent :[" << node << "]" << endl;
            nodeQueue.push(node->child[i]);
        }
    }
}
string BaseAST::restoreChar(char ch) {
    switch(ch) { // change escape character
        case '\a': return "\\a";
        case '\b': return "\\b";
        case '\f': return "\\f";
        case '\n': return "\\n";
        case '\r': return "\\r";
        case '\t': return "\\t";
        case '\v': return "\\v";
        case '\\': return "\\\\";
        case '\'': return "\\\'";
        case '\"': return "\\\"";
        case '\?': return "\\?";
        case '\0': return "0";
    }
    string ret;
    ret.push_back(ch);
    return ret;
}
string BaseAST::restoreStr(string str) {
    string ret;
    for(int i = 0; i < str.size(); i++) {
        ret += restoreChar(str[i]);
    }
    return ret;
}
void BaseAST::visual() {
    if(get_if<int>(&(this->val)) != nullptr) {
        ofresult << "{\"name\": \"" << get<int>(this->val) << "|" << this->dtype << "\"," << endl;
    }
    else if(get_if<double>(&(this->val)) != nullptr) {
        ofresult << "{\"name\": \"" << get<double>(this->val) << "|" << this->dtype  << "\"," << endl;
    }
    else if(get_if<char>(&(this->val)) != nullptr) {
        ofresult << "{\"name\": \"" << restoreChar(get<char>(this->val)) << "|" << this->dtype  << "\"," << endl;
    }
    else if(get_if<string>(&(this->val)) != nullptr) {
        ofresult << "{\"name\": \"" << restoreStr(get<string>(this->val)) << "|" << this->dtype  << "\"," << endl;
    }
    ofresult << "\"children\":[" << endl;
    for(int i=0; i<this->child.size(); i++ ) {
        if( i!=0 ) {
            ofresult << "," << endl;
        }
        child[i]->visual();
    }
    ofresult << "]}" << endl;
}

/* ---------------------------- TokenAST: AST node for lexer token ---------------------------- */
TokenAST::TokenAST() {}
TokenAST::TokenAST(Val val) {
    this->setAST(val);
}
TokenAST::TokenAST(Val val, string type) {
	this->setAST(val, type);
}

TokenAST::~TokenAST() {}

Value *TokenAST::Codegen() {
	return NULL;
}

DocAST::DocAST() {}
DocAST::DocAST(int n) {
    this->val = "doc";
    this->dtype = "VOID";
}
DocAST::~DocAST() {}
Value *DocAST::Codegen() {
    initIO();
    for(int i = 0; i < this->child.size(); i++) {
        this->child[i]->Codegen();
    }
    return NULL;
}

/* ---------------------------- FuncAST: AST node for function ---------------------------- */

FuncAST::FuncAST() {}
FuncAST::FuncAST(Val val, string type) {
    this->val = val;
    this->dtype = type;
}
FuncAST::~FuncAST() {}
Value* FuncAST::Codegen() {
        FunctionType *funcType;
        // create par-type list
        vector<Type *> parType;
        for (int i = 0; i < this->child.size() && this->child[i]->ntype == PAR; i++) {
            Type *type = this->child[i]->getType();
            parType.push_back(type);
        }
        // define function type & create function
        funcType = FunctionType::get(this->getType(), parType, false);
        Function *func = Function::Create(funcType, Function::ExternalLinkage, 
                                          get<string>(this->val), *mod);
        if(get<string>(this->val) == "fly") fly = func;
        
        BasicBlock *entry = BasicBlock::Create(*context, "entry"+get<string>(this->val),
                                               func);
        // set names for parameters
        builder->SetInsertPoint(entry); 

        // parameter: alloca & store
        int i = 0;
        for (auto iter = func->args().begin(); iter != func->args().end(); iter++) {
            string strId = get<string>(this->child[i]->val);
            BaseAST *node = this->child[i++];
            AllocaInst *alloc;
            alloc = builder->CreateAlloca(node->getType(), nullptr, strId);
            builder->CreateStore(iter, alloc);
        }
        // codegen for block member statement
        builder->SetInsertPoint(entry); 
        for(int i = 0; i < this->child.size(); i++) {
            this->child[i]->Codegen();
            // early break for return statement
            if(this->child[i]->ntype == RET) {
                break;
            }
        }
        // default return
        if(!builder->GetInsertBlock()->getTerminator()) {
            Type *type = this->getType();
            if(this->dtype == "INT") {
                builder->CreateRet(Constant::getIntegerValue(type, APInt(32, 0)));
            }
            else if(this->dtype == "FLOAT") {
                builder->CreateRet(ConstantFP::get(type, 0.0));
            }
            else if(this->dtype == "CHAR") {
                builder->CreateRet(Constant::getIntegerValue(type, APInt(8, 0)));
            }
            else if(this->dtype == "VOID") {
                builder->CreateRetVoid();
            }
        }
        return NULL;
    }

/* -------------------------- FuncCallAST: AST node for function call expression -------------------------- */

FuncCallAST::FuncCallAST() {}
FuncCallAST::FuncCallAST(Val val, string type) {
    this->setAST(val, type);
}
FuncCallAST::~FuncCallAST() {}
Value* FuncCallAST::Codegen() {
        // get callee function ptr
        string strId = get<string>(this->val);
        Function *callee = mod->getFunction(strId);

        // parameters
        vector<Value *> args;
        for (int i = 0; i < this->child.size(); i++) {
            args.push_back(this->child[i]->Codegen());
        }
        // call
        Value *Val = builder->CreateCall(callee, args);

        return Val;
    }

/* ---------------------------- RetAST: AST node for return statement ---------------------------- */

RetAST::RetAST() {}
RetAST::RetAST(Val val, string type) {
    this->setAST(val, type, RET);
}
RetAST::~RetAST() {}
Value* RetAST::Codegen() {
        // get return value
        Value *retVal = this->child[0]->Codegen();
        // cast the return object's dtype to return type
        retVal = this->cast(retVal, this->child[0]->dtype, this->dtype);
        return builder->CreateRet(retVal);
    }

/* -------------------------- DefAST: AST node for definition of variables -------------------------- */

DefAST::DefAST() {}
DefAST::DefAST(string type, Val val, int level) {
    this->setAST(val, type);
    this->level = level;
}
DefAST::DefAST(string type, Val val, BaseAST *exp, int level) {
    this->setAST(val, type);
    this->level = level;
    this->child.push_back(exp);
}
DefAST::~DefAST() {}
Value* DefAST::Codegen() {
    string strId = get<string>(this->val);
    // if is a local var's definition
    if(level > 0) {
        AllocaInst *alloc; // allocate a space for the local variable
        alloc = builder->CreateAlloca(this->getType(), nullptr, strId);
        // initial value
        if(!this->child.empty()) {
            Value *val = this->child[0]->Codegen();
            val = this->cast(val, this->child[0]->dtype, this->dtype);
            builder->CreateStore(val, alloc);
        }  
    }
    // if is a global var's definition
    else {
        Type *type = this->getType();
        mod->getOrInsertGlobal(strId, type);
        GlobalVariable *val = mod->getNamedGlobal(strId);
        // allocate a space for a global variable and initilise it
        if(this->dtype == "INT") {
            val->setInitializer(Constant::getIntegerValue(type, APInt(32, 0)));
        }
        else if(this->dtype == "FLOAT") {
            val->setInitializer(ConstantFP::get(type, 0.0));
        }
        else if(this->dtype == "CHAR") {
            val->setInitializer(Constant::getIntegerValue(type, APInt(8, 0)));
        }
        globalValues[strId] = val;
        // initial value
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

/* -------------------------- CondAST: AST node for condition statement -------------------------- */

CondAST::CondAST() {}
CondAST::CondAST(Val val, string type) {
    this->setAST(val, type);
}
CondAST::~CondAST() {}
Value* CondAST::Codegen() {
    Function *func = builder->GetInsertBlock()->getParent();
    vector<BasicBlock *> blocks;
    vector<BasicBlock *> elses;
    bool hasElse = (get<string>(this->child.back()->val) == "else")? true: false;
    // create then-block & else-block
    for (int i = 0; i < this->child.size(); i++) {
        // exclude the possible last else 
        if(i != this->child.size() - 1 || !hasElse)
            blocks.push_back(BasicBlock::Create(*context, ".then"));
        // exclude first if
        if(i != 0 ) elses.push_back(BasicBlock::Create(*context, ".else"));
    }
    BasicBlock *merge = BasicBlock::Create(*context, ".merge");
    for (int i = 0; i < blocks.size(); i++) {
        /* if / elif */
        Value *condExp = this->child[i]->child[0]->Codegen();
        condExp = this->cast(condExp, this->child[i]->child[0]->dtype, "BOOL");
        // br
        if(i + 1 == blocks.size() && !hasElse) // no else for last if
            builder->CreateCondBr(condExp, blocks[i], merge);
        else builder->CreateCondBr(condExp, blocks[i], elses[i]);
        // add block
        func->getBasicBlockList().push_back(blocks[i]);
        // set insert block
        builder->SetInsertPoint(blocks[i]);
        // codegen for statements in current block
        for(int j = 1; j < this->child[i]->child.size(); j++) {
            this->child[i]->child[j]->Codegen();
            // early break for return, break, continue statement
            if(this->child[i]->child[j]->ntype == RET || 
                this->child[i]->child[j]->dtype == "break" ||
                this->child[i]->child[j]->dtype == "continue") {
                break;
            }
        }
        // no terminator, create branch inst
        if(!builder->GetInsertBlock()->getTerminator()) builder->CreateBr(merge);
        /* else of elif-stmt & else*/
        if(i < elses.size()) {
            func->getBasicBlockList().push_back(elses[i]);
            builder->SetInsertPoint(elses[i]);
            // else
            if(i + 1 == elses.size() && hasElse) {
                for(int j = 0; j < this->child.back()->child.size(); j++) {
                    this->child.back()->child[j]->Codegen();
                    // early break for return, break, continue statement
                    if(this->child.back()->child[j]->ntype == RET || 
                        this->child.back()->child[j]->dtype == "break" ||
                        this->child.back()->child[j]->dtype == "continue") {
                        break;
                    }
                }
                // no terminator, create branck inst
                if(!builder->GetInsertBlock()->getTerminator()) builder->CreateBr(merge);
            }
        }
    }
    func->getBasicBlockList().push_back(merge);
    builder->SetInsertPoint(merge);

    return NULL;
}

/* -------------------------- IfAST: AST node for if/elif statement -------------------------- */

IfAST::IfAST() {}
IfAST::IfAST(Val val, string type) {
    this->setAST(val, type);
}
IfAST::~IfAST() {}
Value* IfAST::Codegen() {
    return NULL;
}

/* -------------------------- ElseAST: AST node for else statement -------------------------- */

ElseAST::ElseAST() {}
ElseAST::ElseAST(Val val, string type) {
        this->setAST(val, type);
    }
ElseAST::~ElseAST() {}
Value* ElseAST::Codegen() {
    return NULL;
}

/* -------------------------- DecAST: AST node for function parameter -------------------------- */

DecAST::DecAST() {}
DecAST::DecAST(Val val, string type, bool isArray) {
    this->setAST(val, type, isArray, PAR);
}
DecAST::~DecAST() {}
Value* DecAST::Codegen() {
    return NULL;
}

/* -------------------------- TypeAST: AST node for type token -------------------------- */

TypeAST::TypeAST() {}
TypeAST::TypeAST(string type) {
    this->dtype = type;
}
TypeAST::~TypeAST() {}
Value* TypeAST::Codegen() {
    return NULL;
}

/* ---------------------------- IdAST: AST node for Id token ---------------------------- */

IdAST::IdAST() {}
IdAST::IdAST(Val val) {
    this->val = val;
}
IdAST::~IdAST() {}
Value* IdAST::Codegen() {
    return NULL;
}

/* ---------------------------- ExpAST: AST node for expression ---------------------------- */

ExpAST::ExpAST() {}
ExpAST::ExpAST(Val val, string type) {
    this->setAST(val, type);
}
ExpAST::ExpAST(Val val, string type, nType ntype) {
    this->setAST(val, type, ntype);
}
ExpAST::~ExpAST() {}
Value* ExpAST::Codegen() {
    if(this->ntype == L_VAR) { // local variable
        string strId = get<string>(this->val);
        ValueSymbolTable *table = builder->GetInsertBlock()->getValueSymbolTable();
        return builder->CreateLoad(table->lookup(strId));
    }
    else  if(this->ntype == G_VAR) { // global variable
        string strId = get<string>(this->val);
        return builder->CreateLoad(globalValues[strId]);

    }
    // constant
    else {
        if(this->dtype == "INT") {
            return ConstantInt::get(Type::getInt32Ty(*context), get<int>(this->val));
        }
        else if(this->dtype == "CHAR") {
            return ConstantInt::get(Type::getInt8Ty(*context), get<char>(this->val));
        }
        else if(this->dtype == "FLOAT") {
            return ConstantFP::get(Type::getDoubleTy(*context), get<double>(this->val));
        }
        else if(this->dtype == "LITERAL") {
            string strId = get<string>(this->val);
            return builder->CreateGlobalStringPtr(strId, ".str", 0);
        }
    }
    return NULL;
}

/* ------------------------ AssignAST: AST node for assignment expression ------------------------ */

AssignAST::AssignAST() {}
AssignAST::AssignAST(Val val, string type,nType ntype) {
    this->setAST(val, type,ntype);
}
AssignAST::~AssignAST() {}
Value* AssignAST::Codegen() {
    Value *valRight = this->child[1]->Codegen();
    ValueSymbolTable *table = builder->GetInsertBlock()->getValueSymbolTable();
    string strId = get<string>(this->child[0]->val);
    
    // local variable
    if(this->ntype ==L_VAR){ 
        valRight = this->cast(valRight, this->child[1]->dtype, this->dtype);
        builder->CreateStore(valRight, table->lookup(strId));
        return valRight;
    }
    // global variable
    else {  
        valRight = this->cast(valRight, this->child[1]->dtype, this->dtype);
        builder->CreateStore(valRight, globalValues[strId]);
        return valRight;
    }
    
    return NULL;
}

/* ------------------------ UniOpAST: AST node for Uni-operation ------------------------ */

UniOpAST::UniOpAST() {}
UniOpAST::UniOpAST(Val val, string type, nType ntype) {
    this->val = val;
    this->dtype = type;
    this->ntype = ntype;
}
UniOpAST::~UniOpAST() {}
Value* UniOpAST::Codegen() {
	// get uni-operand
	Value *val = this->child[0]->Codegen();
	// NEG: 0/0.0 - operand
	if(ntype == NEG) { // if the operatoror is "-"
		if(this->dtype == "INT") {
			Value *l = ConstantInt::get(Type::getInt32Ty(*context), 0);
			return builder->CreateSub(l, val, "subtmpi");
		}
		else if(this->dtype == "FLOAT") {
			Value *l = ConstantFP::get(Type::getDoubleTy(*context), 0.0);
			return builder->CreateFSub(l, val, "subtmpf");
		}
	}
	else if(ntype == N_NOT) { // if the operator is "!"
		if(this->dtype == "FLOAT") { // if it is a float number
			// bottom NOT, cmp operand with 0.0
			if(this->child[0]->ntype != N_NOT) {
				Value *l = ConstantFP::get(Type::getDoubleTy(*context), 0.0);
				val = builder->CreateFCmpUNE(l, val, "cmpneqf");
			}
			// NOT
			val = builder->CreateNot(val);
			// top NOT, cast to float type
			if(this->isTop) {
				val = builder->CreateUIToFP(val, Type::getDoubleTy(*context));
			}
			return val;
		}
		else if(this->dtype == "INT" || this->dtype == "CHAR") { // if it is an integer or a character
			// bottom NOT, cmp operand with 0
			if(this->child[0]->ntype != N_NOT) {
				Value *l; 
				if(this->dtype == "INT")
					l = ConstantInt::get(Type::getInt32Ty(*context), 0);
				else l = ConstantInt::get(Type::getInt8Ty(*context), 0);
				val = builder->CreateICmpNE(l, val, "cmpneqi");
			}
			//NOT
			val = builder->CreateNot(val);
			// top NOT, cast to int type
			if(this->isTop) {
                if(this->dtype == "INT") val = builder->CreateZExt(val, Type::getInt32Ty(*context));
                else val = builder->CreateZExtOrTrunc(val, Type::getInt8Ty(*context));
			}
			return val;
		}
	}

	return NULL;
}


/* ------------------------ BinaryAST: AST node for binary operation ------------------------ */

BinaryOpAST::BinaryOpAST() {}
BinaryOpAST::BinaryOpAST(Val val, string type, nType ntype) {
    this->val = val;
    this->dtype = type;
    this->ntype = ntype;
}
BinaryOpAST::~BinaryOpAST() {}
Value* BinaryOpAST::Codegen() {
    Value *valLeft = this->child[0]->Codegen(); // left operand of the exp
    Value *valRight = this->child[1]->Codegen(); // right operand of the exp
    Value *valRes;
    
    // cast the type to float
    if(this->dtype == "FLOAT") {
        valLeft  = builder->CreateSIToFP(valLeft ,Type::getDoubleTy(*context));
        valRight = builder->CreateSIToFP(valRight,Type::getDoubleTy(*context));
    }
    // cast the type to int
    else if(this->dtype == "INT") {
        valLeft  = builder->CreateZExt(valLeft ,Type::getInt32Ty(*context));
        valRight = builder->CreateZExt(valRight,Type::getInt32Ty(*context));
    }
    // binary operator '*'
    if(ntype == B_MUL) {
        if(this->dtype == "FLOAT") return builder->CreateFMul(valLeft, valRight, "multmp");
        else                       return builder->CreateMul(valLeft, valRight, "multmp");
    }
    // binary operator '/'
    else if(ntype == B_DIV) {
        if(this->dtype == "FLOAT")  return builder->CreateFDiv(valLeft, valRight, "divtmp");
        else                        return builder->CreateSDiv(valLeft, valRight, "divtmp");
    }
    // binary operator '+'
    else if(ntype == B_ADD) {
        if(this->dtype == "FLOAT")      return builder->CreateFAdd(valLeft, valRight, "addtmp");
        else if(this->dtype == "INT")   return builder->CreateAdd(valLeft, valRight, "addtmp");
        // add operator for char
        else {
            valRes = builder->CreateAdd(valLeft, valRight, "addtmp");
            return builder->CreateZExtOrTrunc(valRes,Type::getInt8Ty(*context));
        }
    }
    // binary operator '-'
    else if(ntype == B_SUB) {
        if(this->dtype == "FLOAT")      return builder->CreateFSub(valLeft, valRight, "subtmp");
        else if(this->dtype == "INT")   return builder->CreateSub(valLeft, valRight, "subtmp");
        // sub operator for char
        else {
            valRes = builder->CreateSub(valLeft, valRight, "subtmp");
            return builder->CreateZExtOrTrunc(valRes,Type::getInt8Ty(*context));
        }
    }
    // binary operator '>'
    else if(ntype == B_GR) {
        if(this->dtype == "FLOAT") {
            valRes = builder->CreateFCmpUGT(valLeft,valRight);
            return builder->CreateUIToFP(valRes, Type::getDoubleTy(*context));
        }
        else {
            valRes = builder->CreateICmpSGT(valLeft,valRight);
            if(this->dtype == "INT")
                return builder->CreateZExt(valRes, Type::getInt32Ty(*context));
            else if(this->dtype == "CHAR")
                return builder->CreateZExtOrTrunc(valRes, Type::getInt8Ty(*context));
        }
    }
    // binary operator '>='
    else if(ntype == B_GE) {
        if(this->dtype == "FLOAT") {
            valRes = builder->CreateFCmpUGE(valLeft,valRight);
            return builder->CreateUIToFP(valRes, Type::getDoubleTy(*context));
        }
        else {
            valRes = builder->CreateICmpSGE(valLeft,valRight);
            if(this->dtype == "INT")
                return builder->CreateZExt(valRes, Type::getInt32Ty(*context));
            else if(this->dtype == "CHAR")
                return builder->CreateZExtOrTrunc(valRes, Type::getInt8Ty(*context));
        }

    }
    // binary operator '<'
    else if(ntype == B_LS) {
        if(this->dtype == "FLOAT") {
            valRes = builder->CreateFCmpULT(valLeft,valRight);
            return builder->CreateUIToFP(valRes, Type::getDoubleTy(*context));
        }
        else {
            valRes = builder->CreateICmpSLT(valLeft,valRight);
            if(this->dtype == "INT")
                return builder->CreateZExt(valRes, Type::getInt32Ty(*context));
            else if(this->dtype == "CHAR")
                return builder->CreateZExtOrTrunc(valRes, Type::getInt8Ty(*context));
        }
    }
    // binary operator '<='
    else if(ntype == B_LE) {
        if(this->dtype == "FLOAT") {
            valRes = builder->CreateFCmpULE(valLeft,valRight);
            return builder->CreateUIToFP(valRes, Type::getDoubleTy(*context));
        }
        else {
            valRes = builder ->CreateICmpSLE(valLeft,valRight);
            if(this->dtype == "INT")
                return builder->CreateZExt(valRes, Type::getInt32Ty(*context));
            else if(this->dtype == "CHAR")
                return builder->CreateZExtOrTrunc(valRes, Type::getInt8Ty(*context));
        }
    }
    // binary operator '=='
    else if(ntype == B_EQ) {
        if(this->dtype == "FLOAT") {
            valRes = builder->CreateFCmpUEQ(valLeft,valRight);
            return builder->CreateUIToFP(valRes, Type::getDoubleTy(*context));
        }
        else {
            valRes = builder ->CreateICmpEQ(valLeft,valRight);
            if(this->dtype == "INT")
                return builder->CreateZExt(valRes, Type::getInt32Ty(*context));
            else if(this->dtype == "CHAR")
                return builder->CreateZExtOrTrunc(valRes, Type::getInt8Ty(*context));
        }
    }
    // binary operator '!='
    else if(ntype == B_UEQ) {
        if(this->dtype == "FLOAT") {
            valRes = builder->CreateFCmpUNE(valLeft,valRight);
            return builder->CreateUIToFP(valRes, Type::getDoubleTy(*context));
        }
        else {
            valRes = builder->CreateICmpNE(valLeft,valRight);
            if(this->dtype == "INT")
                return builder->CreateZExt(valRes, Type::getInt32Ty(*context));
            else if(this->dtype == "CHAR")
                return builder->CreateZExtOrTrunc(valRes, Type::getInt8Ty(*context));
        }
    }
    // binary operator '&&'or "" / '||'
    else if(ntype == B_AND || ntype == B_OR) {
        valLeft = this->cast(valLeft, this->child[0]->dtype, "INT");
        valRight = this->cast(valRight, this->child[1]->dtype, "INT");
        valRes = ntype == B_AND? builder->CreateAnd(valLeft,valRight): builder->CreateOr(valLeft,valRight);
        Value *l = ConstantInt::get(Type::getInt32Ty(*context), 0);
        valRes = builder->CreateICmpNE(l, valRes, "cmpneqi");
        if(this->dtype == "FLOAT") return builder->CreateUIToFP(valRes, Type::getDoubleTy(*context));
        else {
            Value *val = builder->CreateZExt(valRes, Type::getInt32Ty(*context));
            if(this->dtype == "CHAR") val = builder->CreateZExtOrTrunc(valRes, Type::getInt8Ty(*context)); 
            return val;
        }
    }
    
    return NULL;
}

/* ------------------------ AddrAST: AST node for get-addr operation ------------------------ */

AddrAST::AddrAST() {}
AddrAST::AddrAST(Val val, string type,nType ntype) {
    this->setAST(val, type,ntype);
}
AddrAST::~AddrAST() {}
Value* AddrAST::Codegen() {
    // get llvm symbol table
    ValueSymbolTable *table = builder->GetInsertBlock()->getValueSymbolTable();
    string strId = get<string>(this->val);

    Value *v;
    if(this->ntype == L_VAR){ // local variable
        v = table->lookup(strId);
    }
    else { // global variable
        v = globalValues[strId];
    }

    // change into a pointer
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

/* -------------------------- LoopAST: AST node for while/for statement -------------------------- */

LoopAST::LoopAST(){}
LoopAST::LoopAST(Val val, string type) {
    this->setAST(val, type);
}
LoopAST::~LoopAST(){}
Value* LoopAST::Codegen() {
    if(this->dtype =="while") { // while loop codegen
        BasicBlock * BBori = builder->GetInsertBlock();
        Function   * func = BBori->getParent();
        BasicBlock * BBwhileStart =BasicBlock::Create(*context, "whileStart", func);
        BasicBlock * BBwhileBody  =BasicBlock::Create(*context, "whileBody" , func);
        BasicBlock * BBwhileEnd   =BasicBlock::Create(*context, "whileEnd"  , func);
        
        loopEnd.push(BBwhileEnd);       // add the loop end to the stack
        loopStart.push(BBwhileStart);   // add the loop start to the stack

        builder->CreateBr(BBwhileStart); 

        // condition check
        builder->SetInsertPoint(BBwhileStart);
        Value * cond = this->child[0]->Codegen();
        cond = this->cast(cond, this->child[0]->dtype, "BOOL");
        builder->CreateCondBr(cond, BBwhileBody, BBwhileEnd);
    
        // codegen for while block statements
        builder->SetInsertPoint(BBwhileBody);
        for(int i = 1; i < this->child.size(); i++) {
            this->child[i]->Codegen();
        }
        // no terminator, create branch inst
        if(!builder->GetInsertBlock()->getTerminator())
            builder->CreateBr(BBwhileStart);

        builder->SetInsertPoint(BBwhileEnd); 
        loopEnd.pop();
        loopStart.pop();
    }
    else if(this->dtype == "for") {  // for loop codegen 
        Function * func = builder->GetInsertBlock()->getParent();
        BasicBlock * BBForCond  = BasicBlock::Create(*context, "BBForCond", func); 
        BasicBlock * BBForStart = BasicBlock::Create(*context, "BBForStart", func);
        BasicBlock * BBForBody  = BasicBlock::Create(*context, "BBForBody", func);
        BasicBlock * BBForEnd   = BasicBlock::Create(*context, "BBForEnd", func);
        
        loopEnd.push(BBForEnd);      // add the loop end to the stack
        loopStart.push(BBForStart);  // add the loop start to the stack
        
        // declaration / assignment
        this->child[0]->Codegen(); 
        builder->CreateBr(BBForCond);

        // condition check
        builder->SetInsertPoint(BBForCond);
        Value *cond = this->child[1]->Codegen();
        cond = this->cast(cond, this->child[1]->dtype, "BOOL");
        builder->CreateCondBr(cond, BBForBody, BBForEnd); // 
        
        // codegen for while block statements
        builder->SetInsertPoint(BBForBody);
        for(int i = 3;i<this->child.size();++i) {
            this->child[i]->Codegen();
        }
        // no terminator, create branch inst
        if(!builder->GetInsertBlock()->getTerminator())
            builder->CreateBr(BBForStart);
        
        // loop end action
        builder->SetInsertPoint(BBForStart);
        this->child[2]->Codegen();
        builder->CreateBr(BBForCond);

        builder->SetInsertPoint(BBForEnd);
        loopStart.pop();
        loopEnd.pop();
    }

    return nullptr;
    
}

/* ------------------------ BrAST: AST node for break/continue statement ------------------------ */

BrAST::BrAST(){}
BrAST::BrAST(Val val, string type) {
    this->setAST(val, type);
}
BrAST::~BrAST(){}
Value* BrAST::Codegen() {
    if(this->dtype == "continue") builder->CreateBr(loopStart.top()); // jump to the start of the loop
    else builder->CreateBr(loopEnd.top());                            // jump to the end of the loop
    return NULL;
}

/* ----------------------- ArrAST: AST node for array-related operation ------------------------- */

ArrAST::ArrAST() {}
ArrAST::ArrAST(Val val, string type, int arrLevel) {
    this->setAST(val, type, true);
    this->arrLevel = arrLevel;
}
ArrAST::~ArrAST() {}
Value* ArrAST::Codegen() {
    if(this->isDef) return dec();
    else return ref(this->isAssign);
}
Value* ArrAST::dec() {
    Type *type = this->getType();
    // traverse the array tree
    BaseAST *node = this;
    while(!node->child.empty()) {
        int size = get<int>(node->child[0]->val);
        type = ArrayType::get(type, size);
        node = node->child[1];
    }
    // local array
    if(arrLevel > 0) {
        return builder->CreateAlloca(type, nullptr, this->id);
    }
    // global array
    else {
        Constant *initializer = ConstantAggregateZero::get(type);
        GlobalVariable *arr = new GlobalVariable(*mod, type, false, 
                                                    GlobalVariable::CommonLinkage, 
                                                    initializer, this->id);
        // add into global symbol table
        globalValues[this->id] = arr; 
        return arr;
    }
}
Value* ArrAST::ref(bool isAssign) {
    Value *lVal;
    if(this->arrLevel > 0) {
        ValueSymbolTable *table = builder->GetInsertBlock()->getValueSymbolTable();
        lVal = table->lookup(this->id); // get the value of the array from the symbol table
    }
    else lVal = globalValues[this->id];
    
    // traverse the reference tree to get the subscipt of the array
    vector<Value *> offsets;
    BaseAST *node = this;
    while(!node->child.empty()) {
        Value *offset = node->child[0]->Codegen(); 
        offsets.push_back(offset);
        node = node->child[1];
    }
    
    int cnt = offsets.size(); // the dimension of the array
    while(cnt--) {
        lVal = builder->CreateGEP(lVal, {builder->getInt32(0), offsets[cnt]});
    }

    // assignment
    if(isAssign) {
        Value *val = this->child.back()->Codegen();
        val = this->cast(val, this->child.back()->dtype, this->dtype);
        builder->CreateStore(val, lVal);
        return val;
    }
    // load value
    else return builder->CreateLoad(lVal);
}
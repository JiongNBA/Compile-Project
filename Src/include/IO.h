#ifndef _ATTACHED_IO_H_
#define _ATTACHED_IO_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace llvm;

extern std::unique_ptr<LLVMContext> context;
extern std::unique_ptr<Module> mod;
extern std::unique_ptr<IRBuilder<> > builder;

static Function *createPrintf()
{   
    // arguments' type
    std::vector<Type *> printfArgs;
    printfArgs.push_back(builder->getInt8PtrTy());
    // printf's type
    FunctionType *printfType = FunctionType::get(builder->getInt32Ty(), printfArgs, true);
    // create printf
    Function *printfFunc = Function::Create(printfType, Function::ExternalLinkage, "printf", *mod);

    return  printfFunc;
}

static Function *createScanf()
{
    // arguments' type
    std::vector<Type *> scanfArgs;
    scanfArgs.push_back(builder->getInt8PtrTy());
    // scanf's type
    FunctionType *scanfType = FunctionType::get(builder->getInt32Ty(), scanfArgs, true);
    Function *scanfFunc = Function::Create(scanfType, Function::ExternalLinkage, "scanf", *mod);

    return scanfFunc;
}

static Function *createGetchar()
{
    // arguments' type
    std::vector<Type *> getCharArgs;
    // getchar's type
    FunctionType *getcharType = FunctionType::get(builder->getInt32Ty(), getCharArgs, true);
    // create getchar
    Function *getcharFunc = Function::Create(getcharType, Function::ExternalLinkage, "getchar", *mod);

    return getcharFunc;
}

static void initIO() 
{
    createPrintf();
    createScanf();
    createGetchar();
}

#endif
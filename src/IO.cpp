/* ----------------------------------------------------------
 * file: IO.cpp
 * author: Haofeng Yang 3190105301@zju.edu.cn
 *         Yuesong Wang 3190105303@zju.edu.cn
 *         Jiong   Li   3190105725@zju.edu.cn
 * copyright: © 2022 authors. All rights reserved.
 * ----------------------------------------------------------
 * function: create IO functions: printf, scanf, getchar.
 * description: define function type of linked IO functions.
 * ----------------------------------------------------------
 * create date:   2022/05/08 00:25 pm
 * last modified: 2022/05/19 02:45 am
 * ----------------------------------------------------------
 * history: 2022/05/08 00:25 pm: create the file and def printf and scanf
 *          2022/05/09 00:11 am: create getchar funtion
 *          2022/05/10 00:15 pm: improve the IO
 *          2022/05/19 02:45 am: improve the code structure and add comments
 */

#include "../include/IO.h"

using namespace std;
using namespace llvm;

/**
 * @brief create print function
 */
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

/**
 * @brief create scanf function
 */
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

/**
 * @brief create getchar function
 */
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

/**
 * @brief initialize IO functions
 */
void initIO() 
{
    createPrintf();
    createScanf();
    createGetchar();
}

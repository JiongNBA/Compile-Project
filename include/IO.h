/* ----------------------------------------------------------
 * file: IO.h
 * author: Haofeng Yang 3190105301@zju.edu.cn
 *         Yuesong Wang 3190105303@zju.edu.cn
 *         Jiong   Li   3190105725@zju.edu.cn
 * copyright: © 2022 authors. All rights reserved.
 * ----------------------------------------------------------
 * function: create IO functions: printf, scanf, getchar.
 * description: header file for IO.cpp
 * ----------------------------------------------------------
 * create date:   2022/05/08 00:25 pm
 * last modified: 2022/05/19 02:45 am
 * ----------------------------------------------------------
 * history: 2022/05/08 00:25 pm: create the file and def printf and scanf
 *          2022/05/09 00:11 am: create getchar funtion
 *          2022/05/10 00:15 pm: improve the IO
 *          2022/05/19 02:45 am: improve the code structure and add comments
 */

#ifndef _ATTACHED_IO_H_
#define _ATTACHED_IO_H_
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Function.h>

extern std::unique_ptr<llvm::Module> mod;           // global llvm mod
extern std::unique_ptr<llvm::LLVMContext> context;  // global llvm context
extern std::unique_ptr<llvm::IRBuilder<> > builder; // global llvm builder

static llvm::Function *createPrintf();  // create print function
static llvm::Function *createScanf();   // create scanf function
static llvm::Function *createGetchar(); // create getchar function
void initIO();                          // initialize IO functions

#endif /*IO.h*/
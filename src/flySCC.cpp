/* ----------------------------------------------------------
 * file: flySCC.cpp
 * author: Haofeng Yang 3190105301@zju.edu.cn
 *         Yuesong Wang 3190105303@zju.edu.cn
 *         Jiong   Li   3190105725@zju.edu.cn
 * ----------------------------------------------------------
 * function: main function of fly Subset-of-C Compiler
 * description: source fly code -> lexer -> parser -> 
 *              AST -> IR -> run IR / generate object code
 * ----------------------------------------------------------
 * create date:   2022/04/23 0:04 am
 * last modified: 2022/05/19 2:45 am
 * ----------------------------------------------------------
 * history: 2022/04/23 00:04 am: create the file
 *          2022/04/30 11:44 pm: add code to generate IR
 *          2022/05/06 08:36 pm: add global symbol table
 *          2022/05/12 11:13 am: try to print AST
 *          2022/05/13 10:19 pm: try to create obj file
 *          2022/05/19 00:45 am: improve the code structure and add comments
 */

#include <iostream>
#include <ios>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <FlexLexer.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h>
#include "../include/AST.h"
#include "../include/SymTable.h"
extern int yyparse();
using namespace std;
using namespace llvm;

//define TEST
#ifdef TEST
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/ManagedStatic.h>
//#else
// #include <llvm/Support/Host.h>
// #include <llvm/Support/TargetRegistry.h>
// #include <llvm/Support/TargetSelect.h>
// #include <llvm/Support/FileSystem.h>
// #include <llvm/Support/raw_ostream.h>
// #include <llvm/Target/TargetMachine.h>
// #include <llvm/Target/TargetOptions.h>
// #include <llvm/IR/LegacyPassManager.h>

#endif

FlexLexer* lexer; // global lexer ptr
BaseAST* root;    // AST root
Function *fly;    // main function ptr
map<string, GlobalVariable*> globalValues; // symbol table for global variable
unique_ptr<LLVMContext> context = make_unique<LLVMContext>();           // global context
unique_ptr<Module> mod = make_unique<Module>("flySCC", *context);       // global module
unique_ptr<IRBuilder<> > builder = make_unique<IRBuilder<> >(*context); // global builder
ofstream ofresult("../visual/result.json",ios::trunc);                  // output AST to a JSON file

int main(int argc, char** argv)
{
    // check parameters
    if(argc == 1) {
        fprintf(stderr, "running: %s\n", argv[0]);
        return 1;
    }
    // open source code
    ifstream* srcCode = new ifstream();
    srcCode->open(argv[1], ios::in);
    if(srcCode->fail()) {
        cerr << "the file: \'" << argv[1];
        fprintf(stderr, "\' doesn't exist or cannot be accessed!!!\n");
        return 1;
    }
    // set source file name
    mod->setSourceFileName(argv[1]);
    // set target triple to optimize code
    mod->setTargetTriple(sys::getDefaultTargetTriple());

    // create lexer instance
    lexer = new yyFlexLexer(srcCode);
    // init root
    root = new DocAST(0);
    // grammar & semantic analysis
    yyparse();
    
    //root->prtTree();    
    root->visual();

    // free obj    
    delete srcCode;
    delete lexer;

    // generate IR from AST
    root->Codegen();
    // check module
    if(verifyModule(*mod, &errs())) {
        errs() << "there are errors in module!" << "\n";
        return 0;
    }

    cout << "generate IR file..." << endl;

    // set IR file name with profix '.ll'
    string fileName = string(argv[1]);
    int dotPos = fileName.find_last_of('.');
    fileName.erase(dotPos+1, fileName.size()-dotPos-1);
    string IRFileName = fileName + "ll";

    // output IR file
    std::error_code EC;
    raw_fd_ostream IRFile(IRFileName, EC);
    IRFile << *mod << "\n";

#ifdef TEST
    // execute IR
    ExecutionEngine *engine = EngineBuilder(std::move(mod)).create();
    vector<GenericValue> args;
    cout << "start execution..." << endl;
    GenericValue retVal = engine->runFunction(fly, args);
    outs() << "ret Intval: " << retVal.IntVal << "\n" 
           << "ret Doubleval: " << retVal.DoubleVal << "\n";
    outs().flush();
    delete engine;
    llvm_shutdown();
#else
    // generate object file by calling clang
    string binFileName = fileName + "o";
    string cmd = "clang " + IRFileName + " -o " + binFileName;
    const char *sysCmd = cmd.data();
    FILE *fp;
    // execute command
    if((fp = popen(sysCmd, "r")) == NULL) {
      cout << "error happends when generating obj!" << endl;
      return 1;
    } 
    pclose(fp);

#endif
    return 0;
}
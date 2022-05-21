#include <iostream>
#include <ios>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
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

#define TEST
#ifdef TEST
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/ManagedStatic.h>
#else
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/IR/LegacyPassManager.h>

#endif

map<string, GlobalVariable*> Global_Values;
FlexLexer* lexer;
BaseAST* root;
unique_ptr<LLVMContext> context = make_unique<LLVMContext>();
unique_ptr<Module> mod = make_unique<Module>("flySCC", *context);
unique_ptr<IRBuilder<> > builder = make_unique<IRBuilder<> >(*context);
Function *fly;

extern vector<int> addrs;
extern SymTable symTable;

ofstream ofresult("../visual/result.json",ios::trunc);

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
    addrs.push_back(0); 
    // grammar & semantic analysis
    yyparse();
    
    cout << "here" << endl;
    //root->prtTree();    
    root->visual();
    
    symTable.prtTable();

    // free obj    
    delete srcCode;
    delete lexer;

    cout << "Start generate IR..." << endl;

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
    std::error_code EC;
    raw_fd_ostream IRFile(IRFileName, EC);
    // output IR file
    IRFile << *mod << "\n";

#ifdef TEST
    // execute IR
    ExecutionEngine *engine = EngineBuilder(std::move(mod)).create();
    vector<GenericValue> args;
    cout << "start execution..." << endl;
    GenericValue retVal = engine->runFunction(fly, args);
    outs() << "ret Intval: " << retVal.IntVal <<"\n"<<"ret Doubleval: " << retVal.DoubleVal << "\n";
    outs().flush();
    delete engine;
    llvm_shutdown();
    //pause();
#else
    // compile IR and get machine code
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    std::string err;
    auto target = TargetRegistry::lookupTarget(sys::getDefaultTargetTriple(), err);

  // Print an error and exit if we couldn't find the requested target.
  if (!target) {
    errs() << err;
    return 1;
  }

  auto CPU = "generic";
  auto Features = "";

  TargetOptions opt;
  auto RM = Optional<Reloc::Model>();
  auto TheTargetMachine =
       target->createTargetMachine(sys::getDefaultTargetTriple(), CPU, Features, opt, RM);

  mod->setDataLayout(TheTargetMachine->createDataLayout());

  auto binFileName = fileName + "o";
  raw_fd_ostream dest(binFileName, EC, sys::fs::F_None);

  if (EC) {
    errs() << "Could not open file: " << EC.message();
    return 1;
  }

  legacy::PassManager pass;
  auto FileType = CGFT_ObjectFile;

  if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    errs() << "TheTargetMachine can't emit a file of this type";
    return 1;
  }
pass.run(*mod);
  dest.flush();
  outs() << "writes " << binFileName << "\n";

#endif
    return 0;
}

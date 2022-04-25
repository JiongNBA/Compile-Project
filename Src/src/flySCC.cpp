#include <iostream>
#include <ios>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <FlexLexer.h>
#include "../include/tree.h"
extern int yyparse();
using namespace std;

FlexLexer* lexer;
treeNode* root;
parseTree pTree;

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
        fprintf(stderr, "this file doesn't exist or cannot be accessed!!!\n");
        return 1;
    }
    // create lexer instance
    lexer = new yyFlexLexer(srcCode);
    // grammar analysis
    yyparse();
    // free obj    
    delete srcCode;
    delete lexer;

    // deal with parse tree
    pTree.root = root;

    // print parse tree
    pTree.prtTree();

    return 0;
}
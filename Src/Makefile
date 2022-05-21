CC=g++
FLEX=flex -+
YACC=bison



run1: flySCC
	./flySCC ../test/test1.fly
	chmod +x ../test/test1.o
	chmod +x ../tester/quicksort/linux-amd64
	./../tester/quicksort/linux-amd64 ../test/test1.o
run2: flySCC
	./flySCC ../test/test2.fly
	chmod +x ../test/test2.o
	chmod +x ../tester/matrix-multiplication/linux-amd64
	./../tester/matrix-multiplication/linux-amd64 ../test/test2.o
run3: flySCC
	./flySCC ../test/test3.fly
	chmod +x ../test/test3.o
	chmod +x ../tester/auto-advisor/linux-amd64
	./../tester/auto-advisor/linux-amd64 ../test/test3.o
flySCC: flySCC.cpp lexer.yy.cc parser.tab.cpp IO.cpp AST.cpp semantic.cpp SymTable.cpp
	$(CC) `llvm-config --cxxflags` -std=c++17 -g \
	      lexer.yy.cc parser.tab.cpp flySCC.cpp IO.cpp AST.cpp semantic.cpp SymTable.cpp \
	      -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING=1 -o $@  \
		  `llvm-config --ldflags --system-libs --libs all` -lfl
lexer.yy.cc: lexer.l parser.tab.cpp
	$(FLEX) -o lexer.yy.cc lexer.l
parser.tab.cpp: parser.ypp
	$(YACC) -d parser.ypp -v --debug
clean:
	rm lexer.yy.cc parser.tab.cpp parser.tab.hpp parser.output ../test/*.ll flySCC 2>/dev/null
CC=g++
FLEX=flex -+
YACC=bison

# LAtest1: lexer
#	./lexer ../test/test1.c > test1-LA
# LAtest2: lexer
# 	./lexer ../test/test2.c > test2-LA
# LAtest3: lexer
# 	./lexer ../test/test3.c > test3-LA
# GAtest0: flySCC
# 	./flySCC ../test/test0.c 
# GAtest1: flySCC
# 	./flySCC ../test/test1.c > ../test/test1-GA
# GAtest2: flySCC
# 	./flySCC ../test/test2.c > ../test/test2-GA
# GAtest3: flySCC
# 	./flySCC ../test/test3.c > ../test/test3-GA
SAtest0: flySCC
	./flySCC ../test/test0.c 
# SAtest1: flySCC
# 	./flySCC ../test/test1.c
# SAtest2: flySCC
# 	./flySCC ../test/test2.c
# SAtest3: flySCC
# 	./flySCC ../test/test3.c


debug: flySCC
	gdb ./flySCC

IR:
	clang -emit-llvm ../test/test0.c -S -o  ../test/test0-clang.ll

run1: flySCC
	./flySCC ../test/test1.fly
run2: flySCC
	./flySCC ../test/test2.fly
run3: flySCC
	./flySCC ../test/test3.fly


flySCC: flySCC.cpp lexer.yy.cc parser.tab.cpp
	$(CC) `llvm-config --cxxflags` -std=c++17 -g lexer.yy.cc parser.tab.cpp flySCC.cpp -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING=1 -o $@ `llvm-config --ldflags --system-libs --libs all` -lfl
lexer.yy.cc: lexer.l parser.tab.cpp
	$(FLEX) -o lexer.yy.cc lexer.l
parser.tab.cpp: parser.ypp
	$(YACC) -d parser.ypp -v --debug
clean:
	rm lexer.yy.cc parser.tab.cpp parser.tab.hpp flySCC

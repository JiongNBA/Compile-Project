CC=g++
FLEX=flex -+
YACC=bison

SAtest0:
	$(MAKE) $@ -C ./src

debug:
	$(MAKE) $@ -C ./src

run: compiler
	./compiler

compiler: lexer.yy.cc parser.tab.cc
	$(CC) -o $@ lexer.yy.cc parser.tab.cc -lfl -lm

lexer.yy.cc: lexer.l parser.tab.hh
	$(FLEX) -o parser.tab.cc lexer.l

parser.tab.cc: parser.y
	$(YACC) -d parser.y

run1:
	$(MAKE) $@ -C ./test

run2:
	$(MAKE) $@ -C ./test

run3:
	$(MAKE) $@ -C ./test

clean:
	$(MAKE) $@ -C ./src
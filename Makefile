CC=g++
FLEX=flex -+
YACC=bison

run1:
	$(MAKE) $@ -C ./src

run2:
	$(MAKE) $@ -C ./src

run3:
	$(MAKE) $@ -C ./src

clean:
	$(MAKE) $@ -C ./src
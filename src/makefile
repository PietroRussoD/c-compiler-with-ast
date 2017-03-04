LEX = flex
YACC = bison

CC = gcc

scanner:
	$(YACC) -d parser.y
	$(LEX) scanner.l
	$(CC) -g lex.yy.c parser.tab.c -o ../bin/scanner

clean:
	rm -rf lex.yy.c parser.tab.c parser.tab.h
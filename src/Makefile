scanner: lex.yy.c
	gcc lex.yy.c -lfl -o scanner
parser.tab.h: parser.x
	bison -d parser.x
lex.yy.c: scanner.x parser.tab.h
	flex scanner.x

.PHONY: clean 
clean:
	-@rm -rf parser.tab.c parser.tab.h lex.yy.c bcc 2>/dev/null || true

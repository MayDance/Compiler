test: errormsg.o lex.yy.o y.tab.o util.o absyn.o parse.o prabsyn.o symbol.o table.o
	cc -g -o test errormsg.o lex.yy.o y.tab.o util.o absyn.o parse.o prabsyn.o symbol.o table.o

errormsg.o: errormsg.c errormsg.h util.h
	cc -g -c errormsg.c

lex.yy.c: c--.lex
	flex c--.lex

y.tab.c: c--.y
	bison -d c--.y -o y.tab.c

y.tab.h: y.tab.c
	echo "generating y.tab.c"

y.tab.o: y.tab.c util.h symbol.h errormsg.h absyn.h tokens.h
	cc -g -c y.tab.c

lex.yy.o: lex.yy.c errormsg.h absyn.h y.tab.h tokens.h
	cc -g -c lex.yy.c

util.o: util.c util.h
	cc -g -c util.c

symbol.o: symbol.c util.h symbol.h table.h
	cc -g -c symbol.c

absyn.o: absyn.c util.h symbol.h absyn.h
	cc -g -c absyn.c

parse.o: parse.c symbol.h absyn.h errormsg.h parse.h prabsyn.c
	cc -g -c parse.c

prabsyn.o: prabsyn.c util.h symbol.h absyn.h errormsg.h prabsyn.h
	cc -g -c prabsyn.c

table.o: table.c util.h table.h
	cc -g -c table.c

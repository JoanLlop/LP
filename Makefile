mountains: mountains.g
	antlr -gt mountains.g
	dlg -ci parser.dlg scan.c
	g++ --std=c++11 -o mountains mountains.c scan.c err.c

clean: 
	rm -f *.o mountains.c scan.c err.c parser.dlg tokens.h mode.h

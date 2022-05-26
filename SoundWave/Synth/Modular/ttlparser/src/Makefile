LEX = flex
YACC = bison

all: turtle_parser.cpp turtle_lexer.cpp

turtle_parser.cpp: turtle_parser.y
	${YACC} --defines=turtle_lexer.h --output=turtle_parser.cpp turtle_parser.y

turtle_lexer.cpp: turtle_lexer.l
	${LEX} -o turtle_lexer.cpp turtle_lexer.l

clean:
	rm -f turtle_parser.cpp turtle_lexer.h turtle_lexer.cpp

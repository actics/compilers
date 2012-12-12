program_name = ArithmeticInterpreter
grammar_path = ./grammar/
grammar_code_path = ./antlrgen/
cflags = -lantlr3c

grammar_names = $(addprefix $(grammar_path), $(program_name).g $(program_name)Tree.g)

grammar_code_suffixes = Lexer.c Parser.c Tree.c
grammar_code_names = $(addprefix $(grammar_code_path)grammar/$(program_name), $(grammar_code_suffixes)) ArithmeticExpressionAssembler.cpp ArithmeticInterpreterAssembler.cpp

build:
	antlr3 -o $(grammar_code_path) $(grammar_names)
	g++ $(cflags) -o interpreter interpreter.c $(grammar_code_names)


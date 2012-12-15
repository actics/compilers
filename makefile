program_name = ArithmeticInterpreter
grammar_path = ./grammar/
grammar_code_path = ./antlrgen/
cflags = -lantlr3c

grammar_names = $(addprefix $(grammar_path), $(program_name).g $(program_name)Tree.g)

grammar_code_suffixes = Lexer.c Parser.c Tree.c
grammar_code_names = $(addprefix $(grammar_code_path)grammar/$(program_name), $(grammar_code_suffixes))

assembler_path = ./assembler/
assembler_names = CompilerAssembler.cpp ArithmeticAssembler.cpp
assembler_code_names = $(addprefix $(assembler_path), $(assembler_names))


build:
	antlr3 -o $(grammar_code_path) $(grammar_names)
	g++ $(cflags) -o compiler compiler.cpp $(grammar_code_names) $(assembler_code_names)


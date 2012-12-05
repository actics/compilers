micc_name := MAX_INPUT_CHARS
max_input_chars := 100

const_flags_value := $(max_input_chars)

const_flags := $(micc_name)
const_flags := $(addprefix -D, $(const_flags))
const_flags := $(addsuffix =,  $(const_flags))
const_flags := $(join $(const_flags), $(const_flags_value))

source_dir       := .
grammar_dir_name := grammar
autogen_dir_name := autogen
notation_name    := notation
grammar_name     := ArithmeticExpression
input_name       := input
compile_flags    := -lantlr3c

grammar_dir      := $(addprefix $(source_dir)/, $(grammar_dir_name))
autogen_dir      := $(addprefix $(grammar_dir)/, $(autogen_dir_name))
grammar_name_full   := $(addprefix $(grammar_dir)/, $(addsuffix .g, $(grammar_name)))
autogen_name        := $(addsuffix Lexer, $(grammar_name))
autogen_name        := $(autogen_name) $(addsuffix Parser, $(grammar_name))
autogen_source      := $(addsuffix .c, $(autogen_name))
autogen_header      := $(addsuffix .h, $(autogen_name))
autogen_source_full := $(addprefix $(autogen_dir)/, $(autogen_source))
notation_name_full  := $(addprefix $(source_dir)/, $(addsuffix .c, $(notation_name)))

build : 
	antlr3 -o $(autogen_dir) $(grammar_name_full)
	mv $(addsuffix /grammar/*, $(autogen_dir)) $(autogen_dir)
	rm -rf $(addsuffix /grammar, $(autogen_dir))
	g++ -o $(notation_name) $(notation_name_full) $(autogen_source_full) $(compile_flags) $(const_flags)

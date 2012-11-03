#include <stdio.h>
#include <string.h>

#include "./grammar/autogen/ArithmeticExpressionParser.h"
#include "./grammar/autogen/ArithmeticExpressionLexer.h"

#ifndef MAX_INPUT_CHARS
#define MAX_INPUT_CHARS 100
#endif

#define NEW_STRING_STREAM(str, name) antlr3NewAsciiStringInPlaceStream(\
    str, \
    strlen((const char *)str), \
    (pANTLR3_UINT8)(unsigned char*) name\
)
     
int main(int argc, char * argv[]) {
    pANTLR3_INPUT_STREAM        input;
    pArithmeticExpressionLexer  lex;
    pANTLR3_COMMON_TOKEN_STREAM tokens;
    pArithmeticExpressionParser parser;
    
    char input_string[MAX_INPUT_CHARS + 1];
    fgets (input_string , MAX_INPUT_CHARS + 1, stdin);
    
	pANTLR3_UINT8 input_str = (pANTLR3_UINT8) input_string;
	input = NEW_STRING_STREAM(input_str, "standard input");

	if (input == (pANTLR3_INPUT_STREAM) ANTLR3_ERR_NOMEM ) { 
		fprintf(stderr, "no memory\n");
		exit(EXIT_FAILURE);
	}
	else if (input == (pANTLR3_INPUT_STREAM)ANTLR3_ERR_NOFILE) { 
		fprintf(stderr, "file not found\n");
		exit(EXIT_FAILURE);
	}
	
    lex    = ArithmeticExpressionLexerNew(input);
    tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
    parser = ArithmeticExpressionParserNew(tokens);

    parser ->axiom(parser);

    parser ->free(parser);
    tokens ->free(tokens);
    lex    ->free(lex);
    input  ->close(input);

    return 0;
}

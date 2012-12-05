#include <iostream>
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
    std::string                 input_string;
    std::string                 input_line;
    pANTLR3_UINT8               input_str;
    pANTLR3_INPUT_STREAM        input;
    pArithmeticExpressionLexer  lexer;
    pANTLR3_COMMON_TOKEN_STREAM tokens;
    pArithmeticExpressionParser parser;

    while(std::getline(std::cin, input_line)) {
        input_string += input_line + "\n";
    }

    input_str = (pANTLR3_UINT8) input_string.c_str();
    input = NEW_STRING_STREAM(input_str, "standard input");

    lexer  = ArithmeticExpressionLexerNew(input);
    tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
    parser = ArithmeticExpressionParserNew(tokens);

    parser ->axiom(parser);

    parser ->free(parser);
    tokens ->free(tokens);
    lexer  ->free(lexer);
    input  ->close(input);
    input_string.erase();

    return 0;
}

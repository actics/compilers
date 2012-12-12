#include <iostream>
#include <fstream>
#include <string.h>

#include "./antlrgen/grammar/ArithmeticInterpreterParser.h"
#include "./antlrgen/grammar/ArithmeticInterpreterLexer.h"
#include "./antlrgen/grammar/ArithmeticInterpreterTree.h"

#ifndef MAX_INPUT_CHARS
#define MAX_INPUT_CHARS 100
#endif

#define NEW_STRING_STREAM(str, name) antlr3NewAsciiStringInPlaceStream(\
    str, \
    strlen((const char *)str), \
    (pANTLR3_UINT8)(unsigned char*) name\
)
     
int main(int argc, char * argv[]) {
    pANTLR3_INPUT_STREAM            input;
    pArithmeticInterpreterLexer     lexer;
    pArithmeticInterpreterParser    parser;
    pANTLR3_COMMON_TOKEN_STREAM     tokens;
    pANTLR3_BASE_TREE               tree;
    pANTLR3_COMMON_TREE_NODE_STREAM nodes;
    pArithmeticInterpreterTree      walker;
    
    input = antlr3AsciiFileStreamNew((pANTLR3_UINT8) argv[1]);

    lexer  = ArithmeticInterpreterLexerNew(input);
    tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
    parser = ArithmeticInterpreterParserNew(tokens);

    tree   = parser->axiom(parser).tree;
    nodes  = antlr3CommonTreeNodeStreamNewTree(tree, ANTLR3_SIZE_HINT);
    walker = ArithmeticInterpreterTreeNew(nodes);
    
    std::string code = walker->axiom(walker);
    std::ofstream nasm_file;
    nasm_file.open(argv[2]);
    nasm_file << code;
    nasm_file.close();
    
    walker ->free(walker);
    nodes  ->free(nodes);
    parser ->free(parser);
    tokens ->free(tokens);
    lexer  ->free(lexer);
    input  ->close(input);
    return 0;
}

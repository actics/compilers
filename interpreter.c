#include <iostream>
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
    std::string                     input_line;
    std::string                     input_string;
    pANTLR3_UINT8                   input_str;
    pANTLR3_INPUT_STREAM            input;
    pArithmeticInterpreterLexer     lexer;
    pArithmeticInterpreterParser    parser;
    pANTLR3_COMMON_TOKEN_STREAM     tokens;
    pANTLR3_BASE_TREE               tree;
    pANTLR3_COMMON_TREE_NODE_STREAM nodes;
    pArithmeticInterpreterTree      walker;
    
    ArithmeticInterpreterParser_axiom_return axiom_return;
    
    while (std::getline(std::cin, input_line)) {
        input_string += input_line + "\n";
    }
    
    input_str = (pANTLR3_UINT8) input_string.c_str();
    input = NEW_STRING_STREAM(input_str, "standard input");

    lexer  = ArithmeticInterpreterLexerNew(input);
    tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
    parser = ArithmeticInterpreterParserNew(tokens);

    axiom_return = parser->axiom(parser);
    tree = axiom_return.tree;
    
    printf("< %s >\n", (const char *) tree->toStringTree(tree)->chars);
    
    nodes = antlr3CommonTreeNodeStreamNewTree(tree, ANTLR3_SIZE_HINT);
    
    walker = ArithmeticInterpreterTreeNew(nodes);
    
    walker->axiom(walker);
    
    walker ->free(walker);
    nodes  ->free(nodes);
    //tree   ->free(tree);
    parser ->free(parser);
    tokens ->free(tokens);
    lexer  ->free(lexer);
    input  ->close(input);
    input_string.erase();
    return 0;
}

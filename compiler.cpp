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
    std::cout<< "< " << (const char *) tree->toStringTree(tree)->chars << " >" << std::endl;
//    exit(0);
    nodes  = antlr3CommonTreeNodeStreamNewTree(tree, ANTLR3_SIZE_HINT);
    walker = ArithmeticInterpreterTreeNew(nodes);
   
    std::ostringstream stream;
    std::string nasm_file_name;
    std::string object_file_name;
    std::string nasm_command;
    std::string ld_command;
    std::string rm_command;

    stream << argv[2] << ".asm";
    nasm_file_name = stream.str();
    stream.str(std::string());

    stream << argv[2] << ".o";
    object_file_name = stream.str();
    stream.str(std::string());

    stream << "nasm -f elf64 " << nasm_file_name;
    nasm_command = stream.str();
    stream.str(std::string());

    stream << "ld -lc -lm -m elf_x86_64 -I/lib/ld-linux-x86-64.so.2 " << object_file_name << " -o " << argv[2];
    ld_command = stream.str();
    stream.str(std::string());

    stream << "rm -f " << nasm_file_name << " " << object_file_name;
    rm_command = stream.str();
    stream.str(std::string());

    std::string code = walker->axiom(walker);
    std::ofstream nasm_file;
    nasm_file.open(nasm_file_name.c_str());
    nasm_file << code;
    nasm_file.close();
    
    system(nasm_command.c_str());
    system(ld_command.c_str());
 //   system(rm_command.c_str());

    walker ->free(walker);
    nodes  ->free(nodes);
    parser ->free(parser);
    tokens ->free(tokens);
    lexer  ->free(lexer);
    input  ->close(input);
    return 0;
}


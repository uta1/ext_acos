#include "../lib/file_operations.h"
#include "../lib/checking_definitions.h"
#include "../lib/string_basic.h"

struct lexeme{
    char* text;
    int len;
};

int lexParsing(char* text, lexeme** lexemeArray) {
    int lexemeArrayLen = 1;
    
    *lexemeArray = (lexeme*)calloc(lexemeArrayLen, sizeof(lexeme*));
    ALLOC_CHECK(*lexemeArray);
    
    
}

int getExpression(char* text, int len, int* pos) {
    return 0;
}

int getNumber(char* text, int len, int* pos, lexeme** res) {
    *res = (lexeme*)calloc(1, sizeof(lexeme));
    ALLOC_CHECK(*res);
    
    int posStart = *pos;
    
    while (*pos < len && isChar(text[*pos])) {
        (*res)->len++;
        (*pos)++;
    }
    
    (*res)->text = (char)calloc((*res)->len, sizeof(char));
    ALLOC_CHECK((*res)->text);
    
    for (int i = posStart; i < *pos; ++i) {
        (*res)->text[i] = text[*pos] - '0';
    }
    
    return 0;
}

int main(int argc, int** argv) {
    if (argc < 2) {
        fprintf(stderr, "\033[31mError:\033[0m Expected path to source\n");
        return 1;
    }
    
    if (!isValidFile(argv[1])) {
        fprintf(stderr, "\033[31mError:\033[0m Invalid input file\n");
        return 1;
    }
    
    int fileSize = getFileSize(argv[1]);
    
    char* text = NULL;
    
    if (readFile(argv[1], fileSize, &text) != 0) {
        return 1;
    }
    
    lexeme* lexemeArray = NULL;
    lexParsing(text, &lexemeArray);
    
    return 0;
}

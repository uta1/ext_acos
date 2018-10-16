#include <unistd.h>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <string>
#include "command.hpp"
#include "stack.hpp"
#include "file_operations.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;

bool decompile(char const* pathIn, char const* pathOut) {
    int finSize = getFileSize(pathIn);
    if (!isValidFile(pathIn)) {
        fprintf(stderr, "Invalid path to bin-file\n");
        return false;
    }

    FILE* fin = fopen(pathIn, "rb");
    FILE* fout = fopen(pathOut, "w");

    unsigned char* binSource = (unsigned char*)calloc(finSize + 1, sizeof(unsigned char));
    
    read(fileno(fin), binSource, finSize);
    
    for (int i = 0; i < finSize; i += 8) {
        int type = binSource[i + 0] + (binSource[i + 1] << 8) + (binSource[i + 2] << 16) + (binSource[i + 3] << 24);
        int arg = binSource[i + 4] + (binSource[i + 5] << 8) + (binSource[i + 6] << 16) + (binSource[i + 7] << 24);
        if (i == 0 && type != CommandType::CANARY) {
            fprintf(stderr, "Broken bin-fine: starts not with canary\n");
            return false;
        }
        
        switch (type) {
          case CommandType::PUSH:
            fprintf(fout, "PUSH %i\n", arg);
            break;
            
          case CommandType::POP:
            fprintf(fout, "POP\n");
            break;
            
          case CommandType::ADD:
            fprintf(fout, "ADD\n");
            break;
            
          case CommandType::MUL:
            fprintf(fout, "MUL\n");
            break;
            
          case CommandType::IN:
            fprintf(fout, "IN\n");
            break;
            
          case CommandType::OUT:
            fprintf(fout, "OUT\n");
            break;
            
          case CommandType::AND:
            fprintf(fout, "AND\n");
            break;

          case CommandType::CANARY:
            if (i > 0) {
                fprintf(stderr, "Broken bin-fine: non-initial canary\n");
                return false;
            }
            break;
            
          default:
            fprintf(stderr, "Broken bin-file: unknown command:\n command type: %i\n command arg: %i\n", type, arg);
        }
    }
    
    fclose(fin);
    fclose(fout);
    free(binSource);
}


int main(int argc, char* argv[]) {
    if (argc <= 2 || !isValidFile(argv[1])) {
        printf("Invalid argument: first must be path to bin and second - path to source.\n");
        return 1;
    }
    
    return !decompile(argv[1], argv[2]);
}

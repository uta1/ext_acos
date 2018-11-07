#include <cstdio>
#include <sstream>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include "command.hpp"
#include "stack.hpp"
#include "file_operations.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::set;
using std::map;

void writeCanary(FILE* fout) {
    Command canary = Command::GetCanary();

    unsigned char canaryArray[sizeof(Command)];

    canary.Compile(canaryArray);

    for (int i = 0; i < sizeof(Command); ++i) {
        fprintf(fout, "%c", canaryArray[i]);
    }
}

void writeCommandToFile(FILE* fout, CommandType commandType, int arg, unsigned char* binCommand) {
    Command(commandType, arg).Compile(binCommand);
    for (int i = 0; i < sizeof(Command); ++i) {
        fprintf(fout, "%c", binCommand[i]);
    }
}

bool processLabel(std::istringstream &stream, map<int, set<int> >* labels, int commandIndex, int* name) {
    int arg = 0;
    stream >> arg;
    
    *name = arg;
    
    if ((*labels)[arg].size() == 1 && *((*labels)[arg].begin()) != commandIndex) {
        fprintf(stderr, "\033[31mCompilation error:\033[0m redefine of label %i\n", arg);
        return false;
    }
    
    (*labels)[arg].insert(commandIndex);
    
    return true;
}

void writeUserCommand(FILE* fout, std::istringstream &stream, CommandType commandType, unsigned char* binCommand, bool withArg) {
    int arg = 0;
    if (withArg) {
        stream >> arg;
    }

    writeCommandToFile(fout, commandType, arg, binCommand);
}

void writeJumpCommand(FILE* fout, std::istringstream &stream, map<int, set<int> >* labels, CommandType commandType, unsigned char* binCommand) {
    int arg = 0;
    stream >> arg;

    if (labels->count(arg) > 0) {
        arg = *((*labels)[arg].begin());
        writeCommandToFile(fout, commandType, arg, binCommand);
    }
}

bool compileCycle(char const* pathIn, char const* pathOut, map<int, set<int> > *labels) {
    int finSize = getFileSize(pathIn);
    if (!isValidFile(pathIn)) {
        fprintf(stderr, "\033[Error:\033[0m invalid path to source\n");
        return false;
    }

    char* source = readFile(pathIn, finSize);
    
    std::istringstream stream;
    stream.str(source);
    
    unsigned char* binCommand = (unsigned char*)calloc(sizeof(Command), sizeof(unsigned char));
    char* type = (char*)calloc(10, sizeof(char));
    char* sysArg = (char*)calloc(10, sizeof(char)); 
    
    FILE* fout = fopen(pathOut, "wb");
    writeCanary(fout);

    int commandIndex = 0; // canary was first, first user command has number 1
    
    #define COMPILE_CMD(name, strName, n, compileCode) \
        if (strcmp(type, strName) == 0) { \
            compileCode; \
            continue; \
        }
    while (stream >> type) {
        ++commandIndex;
        
        #include "commands.h"
        
        fprintf(stderr, "\033[31mCompilation error:\033[0m unknown command type \"%s\"\n", type);
        
        free(type);
        free(binCommand);
        
        return false;
    }
    #undef COMPILE_CMD
    
    fclose(fout);
    free(type);
    free(sysArg);
    free(binCommand);
    free(source);
    
    return true;
}

int main(int argc, char* argv[]) {
    if (argc <= 2 || !isValidFile(argv[1])) {
        printf("\033[31mError:\033[0m invalid argument: first must be path to source and second - path to bin.\n");
        return 1;
    }
    
    map<int, set<int> > labels;
    if (!compileCycle(argv[1], argv[2], &labels)) {
        return 1;
    }
    
    return !compileCycle(argv[1], argv[2], &labels);
}

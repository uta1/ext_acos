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

void writeGoto(FILE* fout, std::istringstream &stream, map<int, set<int> >* labels, unsigned char* binCommand) {
    int arg = 0;
    stream >> arg;

    if (labels->count(arg) > 0) {
        arg = *((*labels)[arg].begin());
        writeCommandToFile(fout, CommandType::GOTO, arg, binCommand);
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
    while (stream >> type) {
        ++commandIndex;
        
        if (strcmp(type, ":") == 0) {
           int name = 0;
           processLabel(stream, labels, commandIndex, &name);
           writeCommandToFile(fout, CommandType::LABEL, name, binCommand);
           continue;
        }
        
        if (strcmp(type, "GOTO") == 0) {
            writeGoto(fout, stream, labels, binCommand);
            continue;
        }
        
        if (strcmp(type, "PUSH") == 0) {
            stream >> sysArg;
            
            if (strcmp(sysArg, "S") == 0) {
                writeUserCommand(fout, stream, CommandType::PUSH,    binCommand, true);
            } else 
            
            if (strcmp(sysArg, "RAX") == 0) {
                writeUserCommand(fout, stream, CommandType::PUSHRAX, binCommand, false);
            } else
            if (strcmp(sysArg, "RBX") == 0) {
                writeUserCommand(fout, stream, CommandType::PUSHRBX, binCommand, false);
            } else
            if (strcmp(sysArg, "RCX") == 0) {
                writeUserCommand(fout, stream, CommandType::PUSHRCX, binCommand, false);
            } else 
            if (strcmp(sysArg, "RDX") == 0) {
                writeUserCommand(fout, stream, CommandType::PUSHRDX, binCommand, false);
            } else 

            if (strcmp(sysArg, "RAM") == 0) {
                writeUserCommand(fout, stream, CommandType::PUSHRAM, binCommand, true);
            }
            
            continue;
        }

        if (strcmp(type, "POP") == 0) {
            stream >> sysArg;
            
            if (strcmp(sysArg, "S") == 0) {
                writeUserCommand(fout, stream, CommandType::POP,    binCommand, false);
            } else 
            
            if (strcmp(sysArg, "RAX") == 0) {
                writeUserCommand(fout, stream, CommandType::POPRAX, binCommand, false);
            } else
            if (strcmp(sysArg, "RBX") == 0) {
                writeUserCommand(fout, stream, CommandType::POPRBX, binCommand, false);
            } else
            if (strcmp(sysArg, "RCX") == 0) {
                writeUserCommand(fout, stream, CommandType::POPRCX, binCommand, false);
            } else 
            if (strcmp(sysArg, "RDX") == 0) {
                writeUserCommand(fout, stream, CommandType::POPRDX, binCommand, false);
            } else 

            if (strcmp(sysArg, "RAM") == 0) {
                writeUserCommand(fout, stream, CommandType::POPRAM, binCommand, true);
            }
            
            continue;
        }

        if (strcmp(type, "ADD") == 0) {
            writeUserCommand(fout, stream, CommandType::ADD, binCommand, false);
            continue;
        }

        if (strcmp(type, "MUL") == 0) {
            writeUserCommand(fout, stream, CommandType::MUL, binCommand, false);
            continue;
        }

        if (strcmp(type, "IN") == 0) {
            writeUserCommand(fout, stream, CommandType::IN,  binCommand, false);
            continue;
        }

        if (strcmp(type, "OUT") == 0) {
            writeUserCommand(fout, stream, CommandType::OUT, binCommand, false);
            continue;
        }

        if (strcmp(type, "AND") == 0) {
            writeUserCommand(fout, stream, CommandType::AND, binCommand, false);
            continue;
        }

        fprintf(stderr, "\033[31mCompilation error:\033[0m unknown command type \"%s\"\n", type);
        
        free(type);
        free(binCommand);
        
        return false;
    }
    
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

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

void writeCanary(FILE* fout) {
    Command canary = Command::GetCanary();

    unsigned char canaryArray[8] = "\0\0\0\0\0\0\0";

    canary.Compile(canaryArray);

    for (int i = 0; i < 8; ++i) {
        fprintf(fout, "%c", canaryArray[i]);
    }
}

void writeUserCommand(FILE* fout, std::istringstream &stream, CommandType commandType, unsigned char* binCommand, bool withArg) {
    int arg = 0;
    if (withArg) {
        stream >> arg;
    }

    Command(commandType, arg).Compile(binCommand);
    for (int i = 0; i < 8; ++i) {
        fprintf(fout, "%c", binCommand[i]);
    }
}

bool compile(char const* pathIn, char const* pathOut) {
    int finSize = getFileSize(pathIn);
    if (!isValidFile(pathIn)) {
        fprintf(stderr, "Invalid path to source\n");
        return false;
    }

    char* source = readFile(pathIn, finSize);
    
    std::istringstream stream;
    stream.str(source);
    
    unsigned char* binCommand = (unsigned char*)calloc(8, sizeof(unsigned char));
    char* type = (char*)calloc(10, sizeof(char));
    char* sysArg = (char*)calloc(10, sizeof(char)); 
    
    FILE* fout = fopen(pathOut, "wb");
    writeCanary(fout);

    while (stream >> type) {
        if (strcmp(type, "PUSH") == 0) {
            stream >> sysArg;
            
            if (strcmp(sysArg, "S") == 0) {
                writeUserCommand(fout, stream, CommandType::PUSH, binCommand, true);
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
            }
            
            continue;
        }

        if (strcmp(type, "POP") == 0) {
            stream >> sysArg;
            
            if (strcmp(sysArg, "S") == 0) {
                writeUserCommand(fout, stream, CommandType::POP, binCommand, false);
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
            writeUserCommand(fout, stream, CommandType::IN, binCommand, false);
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

        fprintf(stderr, "Unknown command type: %s\n", type);
        
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
        printf("Invalid argument: first must be path to source and second - path to bin.\n");
        return 1;
    }
    
    return !compile(argv[1], argv[2]);
}

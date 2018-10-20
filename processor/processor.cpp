#include <iostream>
#include <unistd.h>
#include "file_operations.hpp"
#include "command.hpp"
#include "stack.hpp"

using std::cout;
using std::endl;

class Processor {
  public:
    enum Error{
        OK = 0,
        INVALID_COMMAND = 1,
        EXTRA_CANARY = 2,
        WRANG_ENTERED_NUMBER = 3,
        SOURCE_ERROR = 4
    };
    
  private:
    static const int INTS_PER_COMMAND = sizeof(Command) / sizeof(int);
    Stack<int> s;
    int rax = 0;
    int rbx = 0;
    int rcx = 0;
    int rdx = 0;
    
    Error CheckNonemptyStack() const {
        if (s.GetSize() < 1) {
            fprintf(stderr, "Source error:\n getting item from empty stack\n");
            return Error(SOURCE_ERROR);
        }
        
        return Error(OK);
    }
    
    Error PushReg(int* reg) {
        Error error = CheckNonemptyStack();
        if (error != Error::OK) {
            return error;
        }
        
        s.GetTop(reg);
        
        return Error(OK);
    }
    
    Error PrintTop() const {
        Error error = CheckNonemptyStack();
        if (error != Error::OK) {
            return error;
        }

        int result = 0;
        s.GetTop(&result);
        cout << result << endl;
        
        return Error(OK);
    }
    
    Error Process(Command command) {
        if (!command.IsValid()) {
            fprintf(stderr, "Invalid command:\n type: %i\n arg: %i\n", (int)command.GetType(), command.GetArg());
            return Error(INVALID_COMMAND);
        }
        
        int arg1 = 0;
        int arg2 = 0;
        int enteredNumber = 0;
        
        switch (command.GetType()) {
          case CommandType::CANARY:
            fprintf(stderr, "Non-initial canary\n");
            return Error(EXTRA_CANARY);
            
          case CommandType::PUSH:            
            s.Push(command.GetArg());
            break;
            
          case CommandType::PUSHRAX:
            PushReg(&rax);
            break;
            
          case CommandType::PUSHRBX:
            PushReg(&rbx);
            break;
            
          case CommandType::PUSHRCX:
            PushReg(&rcx);
            break;
            
          case CommandType::PUSHRDX:
            PushReg(&rdx);
            break;
            
          case CommandType::POP:
            s.Pop();
            break;
            
          case CommandType::POPRAX:
            s.Push(rax);
            break;

          case CommandType::POPRBX:
            s.Push(rbx);
            break;

          case CommandType::POPRCX:
            s.Push(rcx);
            break;

          case CommandType::POPRDX:
            s.Push(rdx);
            break;

          case CommandType::ADD:
            s.GetTop(&arg1);
            s.Pop();
            s.GetTop(&arg2);
            s.Pop();
            s.Push(arg1 + arg2);
            break;
            
          case CommandType::MUL:
            s.GetTop(&arg1);
            s.Pop();
            s.GetTop(&arg2);
            s.Pop();
            s.Push(arg1 * arg2);
            break;
            
          case CommandType::IN:
            if (scanf("%i", &enteredNumber) != 1) {
                fprintf(stderr, "Wrang number entered\n");
                return Error(WRANG_ENTERED_NUMBER);
            }
            s.Push(enteredNumber);
            break;
            
          case CommandType::OUT:            
            PrintTop();
            break;
        }
        
        return Error(OK);
    }
    
  public:
    Processor() {}

    bool Execute(unsigned char const* dataCommandArray, int totalCommands) {
        Command canary(dataCommandArray);
        if (!canary.EqualsCanary()) {
            fprintf(stderr, "The program does not comply with the protocol:\n broken file beginnig\n");
            return false;
        }
        
        for (int i = 1; i < totalCommands; ++i) {
            Command current(dataCommandArray + i * sizeof(Command));
            if (current.GetType() == CommandType::AND) {
                s.Clear();
                return true;
            }
            Error error = Process(current);
            if (error != Error::OK) {
                return false;
            }
        }
        
        s.Clear();
    }
    
    bool Execute(char const* pathToBin) {
        if (!isValidFile(pathToBin)) {
            fprintf(stderr, "Invalid path to bin-file\n");
            return false;
        }
        
        int finSize = getFileSize(pathToBin);
        if (finSize % 8 != 0) {
            fprintf(stderr, "The program does not comply with the protocol:\n commands must be 8 bytes each\n");
            return false;
        }

        FILE* fin = fopen(pathToBin, "rb");
        unsigned char* binSource = (unsigned char*)calloc(finSize + 1, sizeof(unsigned char));
        read(fileno(fin), binSource, finSize);
        Execute(binSource, finSize >> 3);
        
        fclose(fin);
        free(binSource);
        
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc <= 1 || !isValidFile(argv[1])) {
        printf("Invalid argument: it must be path to source\n");
        return 1;
    }
    
    Processor processor;
    
    return !processor.Execute(argv[1]);
}


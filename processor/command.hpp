#pragma once

enum CommandType {
    PUSH = 101,
    POP = 102,
    ADD = 201,
    MUL = 202,
    IN = 301,
    OUT = 302,
    AND = 401,
    CANARY = 1000
};

struct Command {
  private:
    static void Compile(unsigned char* pointer, int value) {
        for (int i = 0; i < sizeof(int); ++i) {
            pointer[i] = value & 255;
            value >>= 8;
        }
    }

    static void Decompile(unsigned char const* pointer, int* value) {
        *value = pointer[0] + (pointer[1] << 8) + (pointer[2] << 16) + (pointer[3] << 24);
    }

    CommandType type{(CommandType)0};
    int arg{0};
    
  public:
    Command(CommandType type_, int arg_) : type(type_), arg(arg_) {}
    Command(unsigned char const* pointer) {
        Decompile(pointer);
    }
    
    CommandType GetType() const {
        return type;
    }
    
    int GetArg() const {
        return arg;
    }
    
    void Decompile(unsigned char const* pointer) {
        int tempType = 0;
        Decompile(pointer, &tempType);
        type = (CommandType)tempType;
        Decompile(pointer + sizeof(int), &arg);
    }
    
    void Compile(unsigned char* pointer) {
        Compile(pointer, (int)type);
        Compile(pointer + sizeof(int), arg);
    }
    
    bool IsValid() const {
        int tempType = (int)type;
        return (tempType == CommandType::PUSH || tempType == CommandType::POP || tempType == CommandType::ADD 
                || tempType == CommandType::MUL || tempType == CommandType::IN || tempType == CommandType::OUT
                || tempType == CommandType::AND || tempType == CommandType::CANARY);
    }
    
    static Command GetCanary() {
        return Command(CommandType::CANARY, 0);
    }
};


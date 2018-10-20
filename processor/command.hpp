#pragma once

enum CommandType {
    PUSH = 101, //Stack-operation group
    PUSHRAX = 111,
    PUSHRBX = 121,
    PUSHRCX = 131,
    PUSHRDX = 141,
    POP = 102, //Stack-operation group
    POPRAX = 112,
    POPRBX = 122,
    POPRCX = 132,
    POPRDX = 142,
    ADD = 201, //Arithmetic group
    MUL = 202, //Arithmetic group
    IN = 301, //IO group
    OUT = 302, //IO group
    AND = 401, //System group
    LABEL = 501, //Label group
    GOTO = 502, //Label group
    CANARY = 1000 //Security group
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
    static const int CANARY_ARG = 16091998;
    
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
    
    void Compile(unsigned char* pointer) const {
        Compile(pointer, (int)type);
        Compile(pointer + sizeof(int), arg);
    }
    
    bool IsValid() const {
        int tempType = (int)type;
        return (tempType == CommandType::PUSH || 
                tempType == CommandType::PUSHRAX || 
                tempType == CommandType::PUSHRBX || 
                tempType == CommandType::PUSHRCX || 
                tempType == CommandType::PUSHRDX || 
                tempType == CommandType::POP || 
                tempType == CommandType::POPRAX || 
                tempType == CommandType::POPRBX || 
                tempType == CommandType::POPRCX || 
                tempType == CommandType::POPRDX || 
                tempType == CommandType::ADD || tempType == CommandType::MUL || tempType == CommandType::IN 
                || tempType == CommandType::OUT || tempType == CommandType::AND || 
                tempType == CommandType::LABEL || tempType == CommandType::GOTO || tempType == CommandType::CANARY);
    }
    
    static Command GetCanary() {
        return Command(CommandType::CANARY, CANARY_ARG);
    }

    bool EqualsCanary() const {
        return (int)type == (int)CommandType::CANARY && arg == CANARY_ARG;
    }
};


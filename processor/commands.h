COMPILE_CMD(
    LABEL,
    ":",
    501,
    {
        int name = 0;
        processLabel(stream, labels, commandIndex, &name);
        writeCommandToFile(fout, CommandType::LABEL, name, binCommand);    
    }
)

COMPILE_CMD(
    GOTO,
    "GOTO",
    502,
    writeJumpCommand(fout, stream, labels, CommandType::GOTO, binCommand);
)

COMPILE_CMD(
    PUSH, 
    "PUSH",
    101, 
    {
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
    }
)

COMPILE_CMD(
    POP,
    "POP",
    102,
    {
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
    }
)

COMPILE_CMD(
    CMP,
    "CMP",
    601, 
    {
        stream >> sysArg;
        
        if (strcmp(sysArg, "S") == 0) {
            writeUserCommand(fout, stream, CommandType::CMP,    binCommand, true);
        } else 
        
        if (strcmp(sysArg, "RAX") == 0) {
            writeUserCommand(fout, stream, CommandType::CMPRAX, binCommand, true);
        } else
        if (strcmp(sysArg, "RBX") == 0) {
            writeUserCommand(fout, stream, CommandType::CMPRBX, binCommand, true);
        } else
        if (strcmp(sysArg, "RCX") == 0) {
            writeUserCommand(fout, stream, CommandType::CMPRCX, binCommand, true);
        } else 
        if (strcmp(sysArg, "RDX") == 0) {
            writeUserCommand(fout, stream, CommandType::CMPRDX, binCommand, true);
        }
    }
)

COMPILE_CMD(
    JE,
    "JE",
    602,
    writeJumpCommand(fout, stream, labels, CommandType::JE, binCommand);
)

COMPILE_CMD(
    JL,
    "JL",
    603,
    writeJumpCommand(fout, stream, labels, CommandType::JL, binCommand);
)

COMPILE_CMD(
    CALL,
    "CALL",
    701,
    writeJumpCommand(fout, stream, labels, CommandType::CALL, binCommand);
)

#define COMPILE_CMD_NOARG(name, n) \
    COMPILE_CMD(name, #name, n, writeUserCommand(fout, stream, CommandType::name, binCommand, false))

COMPILE_CMD_NOARG(ADD, 201)
COMPILE_CMD_NOARG(MUL, 202)
COMPILE_CMD_NOARG(IN, 301)
COMPILE_CMD_NOARG(OUT, 302)
COMPILE_CMD_NOARG(AND, 401)
COMPILE_CMD_NOARG(RET, 702)

#undef COMPILE_CMD_NOARG

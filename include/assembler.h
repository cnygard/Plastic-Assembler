#pragma once

#include <stdio.h>

#include "bytecode.h"
#include "token.h"
#include "bytebuffer.h"
#include "parser.h"

enum _AssemblerStatus
{
    ASSEMBLER_SUCCESS,
    ASSEMBLER_ERROR
};
typedef enum _AssemblerStatus AssemblerStatus;

struct _Assembler
{
    AssemblerStatus status;
    TokenList* tokens;
    ByteBuffer* bytecode;
};
typedef struct _Assembler Assembler;

void assembler_start(Assembler* c, Parser* p);
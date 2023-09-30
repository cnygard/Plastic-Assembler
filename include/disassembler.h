#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "bytecode.h"
#include "token.h"
#include "util.h"

enum _DisassemblerStatus
{
    DISASSEMBLER_SUCCESS,
    DISASSEMBLER_ERROR
};
typedef enum _DisassemblerStatus DisassemblerStatus;

struct _Disassembler
{
    int pos;
    uint8_t* source;
    uint32_t length;
    uint32_t ip;
    DisassemblerStatus status;
    bool running;
};
typedef struct _Disassembler Disassembler;

int8_t d_next8(Disassembler* d);
int16_t d_next16(Disassembler* d);
int32_t d_next32(Disassembler* d);

void disassembler_init(Disassembler* d, uint8_t* source, uint32_t length, int pos);
void disassembler_start(Disassembler* d);
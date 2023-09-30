#pragma once

#include <stdbool.h>

#include "bytecode.h"
#include "util.h"
#include "token.h"

#define STACK_SIZE 1024
#define REG_COUNT 16

enum _RuntimeStatus
{
    RUNTIME_SUCCESS,
    RUNTIME_ERROR
};
typedef enum _RuntimeStatus RuntimeStatus;

struct _Runtime
{
    uint8_t* code;
    RuntimeStatus status;
    char stack[STACK_SIZE];
    int32_t alu;
    uint8_t regCount;
    uint32_t regs[REG_COUNT];
    uint32_t exit;
    bool running;
};
typedef struct _Runtime Runtime;

void runtime_init(Runtime* runtime, uint8_t* code);
void runtime_start(Runtime* runtime);

// Code functions
int8_t r_next8(Runtime* r);
int16_t r_next16(Runtime* r);
int32_t r_next32(Runtime* r);

// Stack functions
void push8(Runtime* r, uint8_t data);
void push16(Runtime* r, uint16_t data);
void push32(Runtime* r, uint32_t data);
void set8(Runtime* r, uint8_t data, uint32_t addr);
void set16(Runtime* r, uint16_t data, uint32_t addr);
void set32(Runtime* r, uint32_t data, uint32_t addr);
int8_t pop8(Runtime* r);
int16_t pop16(Runtime* r);
int32_t pop32(Runtime* r);
int8_t peak8(Runtime* r, uint32_t addr);
int16_t peak16(Runtime* r, uint32_t addr);
int32_t peak32(Runtime* r, uint32_t addr);

// Register functions
uint64_t combine_registers(Runtime* r, int reg1, int reg2);
void update_flags(Runtime* runtime);
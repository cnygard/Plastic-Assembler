#pragma once

#include "token.h"

enum _Opcode
{
    NOP_OP = 0x00,
    PUSH_CONST,
    PUSH_REG,
    PUSH_STR,
    POP_REG,
    SET_MEM_CONST,
    SET_MEM_REG,
    SET_MEM_STR,
    SET_MEM_CONST_CONST,
    SET_MEM_CONST_REG,
    SET_MEM_CONST_STR,
    GET_MEM_REG_MODE,
    GET_MEM_CONST_REG_MODE,
    MOV_REG_CONST,
    MOV_REG_MEM,
    MOV_REG_MEM_CONST,
    MOV_REG_REG,
    ADD_STACK,
    ADD_REG,
    ADD_REG_CONST,
    ADD_REG_MEM,
    ADD_REG_MEM_CONST,
    ADD_REG_REG,
    SUB_STACK,
    SUB_REG,
    SUB_REG_CONST,
    SUB_REG_MEM,
    SUB_REG_MEM_CONST,
    SUB_REG_REG,
    MUL_STACK,
    MUL_REG,
    MUL_REG_CONST,
    MUL_REG_MEM,
    MUL_REG_MEM_CONST,
    MUL_REG_REG,
    DIV_STACK,
    DIV_CONST,
    DIV_REG,
    INC_REG,
    DEC_REG,
    JMP_STACK,
    JMP_CONST,
    JMP_REG,
    JZ_STACK,
    JZ_CONST,
    JZ_REG,
    JG_STACK,
    JG_CONST,
    JG_REG,
    JL_STACK,
    JL_CONST,
    JL_REG,
    JGQ_STACK,
    JGQ_CONST,
    JGQ_REG,
    JLQ_STACK,
    JLQ_CONST,
    JLQ_REG,
    JNZ_STACK,
    JNZ_CONST,
    JNZ_REG,
    JR_STACK,
    JR_CONST,
    JR_REG,
    JRZ_STACK,
    JRZ_CONST,
    JRZ_REG,
    JRNZ_STACK,
    JRNZ_CONST,
    JRNZ_REG,
    JRG_STACK,
    JRG_CONST,
    JRG_REG,
    JRL_STACK,
    JRL_CONST,
    JRL_REG,
    JRGQ_STACK,
    JRGQ_CONST,
    JRGQ_REG,
    JRLQ_STACK,
    JRLQ_CONST,
    JRLQ_REG,
    CMP_STACK,
    CMP_REG,
    CMP_CONST,
    CMP_REG_CONST,
    CMP_REG_MEM,
    CMP_REG_MEM_CONST,
    CMP_REG_REG,
    TEST_OP = 0xFD,
    RESET_OP = 0xFE,
    HLT_OP = 0xFF
};
typedef enum _Opcode Opcode;

TokenInst opcode_to_token_inst(Opcode op);
char* get_inst_string(Opcode op);

enum _Register
{
    EAX_REGISTER = 0x00,
    EBX_REGISTER,
    ECX_REGISTER,
    EDX_REGISTER,
    EBP_REGISTER,
    ESP_REGISTER,
    EIP_REGISTER,
    EIR_REGISTER,
    ALU_TEST,
    SV_TEST,
    FLAG_REGISTER = 0x0F
};
typedef enum _Register Register;

char* get_register_string(Register reg);

enum _Mode
{
    INT_MODE = 0,
    STR_MODE
};
typedef enum _Mode Mode;

char* get_mode_string(Mode mode);

enum _Flag
{
    FLAG_CARRY =     1,
    FLAG_PARITY =    2,
    FLAG_AUX_CARRY = 3,
    FLAG_ZERO =      4,
    FLAG_SIGN =      5,
    FLAG_TRAP =      6,
    FLAG_DIRECTION = 7,
    FLAG_OVERFLOW =  8,
};
typedef enum _Flag Flag;

char* get_flag_string(Flag flag);
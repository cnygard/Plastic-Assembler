#include "../include/bytecode.h"

TokenInst opcode_to_token_inst(Opcode op)
{
    switch (op)
    {
        default: { return -1; break; }
        case NOP_OP: { return NOP; break; }
        case PUSH_CONST: { return PUSH; break; }
        case PUSH_REG: { return PUSH; break; }
        case PUSH_STR: { return PUSH; break; }
        case POP_REG: { return POP; break; }
        case SET_MEM_CONST_CONST: { return SET; break; }
        case SET_MEM_CONST_REG: { return SET; break; }
        case SET_MEM_CONST_STR: { return SET; break; }
        case GET_MEM_CONST_REG_MODE: { return GET; break; }
        case MOV_REG_CONST: { return MOV; break; }
        case MOV_REG_MEM: { return MOV; break; }
        case MOV_REG_MEM_CONST: { return MOV; break; }
        case MOV_REG_REG: { return MOV; break; }
        case ADD_STACK: { return ADD; break; }
        case ADD_REG: { return ADD; break; }
        case ADD_REG_CONST: { return ADD; break; }
        case ADD_REG_MEM: { return ADD; break; }
        case ADD_REG_MEM_CONST: { return ADD; break; }
        case ADD_REG_REG: { return ADD; break; }
        case SUB_STACK: { return SUB; break; }
        case SUB_REG: { return SUB; break; }
        case SUB_REG_CONST: { return SUB; break; }
        case SUB_REG_MEM: { return SUB; break; }
        case SUB_REG_MEM_CONST: { return SUB; break; }
        case SUB_REG_REG: { return SUB; break; }
        case MUL_STACK: { return MUL; break; }
        case MUL_REG: { return MUL; break; }
        case MUL_REG_CONST: { return MUL; break; }
        case MUL_REG_MEM: { return MUL; break; }
        case MUL_REG_MEM_CONST: { return MUL; break; }
        case MUL_REG_REG: { return MUL; break; }
        case DIV_STACK: { return DIV; break; }
        case DIV_CONST: { return DIV; break; }
        case DIV_REG: { return DIV; break; }
        case INC_REG: { return INC; break; }
        case DEC_REG: { return DEC; break; }
        case JMP_STACK: { return JMP; break; }
        case JMP_CONST: { return JMP; break; }
        case JMP_REG: { return JMP; break; }
        case JZ_STACK: { return JZ; break; }
        case JZ_CONST: { return JZ; break; }
        case JZ_REG: { return JZ; break; }
        case JNZ_STACK: { return JNZ; break; }
        case JNZ_CONST: { return JNZ; break; }
        case JNZ_REG: { return JNZ; break; }
        case JG_STACK: { return JG; break; }
        case JG_CONST: { return JG; break; }
        case JG_REG: { return JG; break; }
        case JL_STACK: { return JL; break; }
        case JL_CONST: { return JL; break; }
        case JL_REG: { return JL; break; }
        case JGQ_STACK: { return JGQ; break; }
        case JGQ_CONST: { return JGQ; break; }
        case JGQ_REG: { return JGQ; break; }
        case JLQ_STACK: { return JLQ; break; }
        case JLQ_CONST: { return JLQ; break; }
        case JLQ_REG: { return JLQ; break; }
        case JR_STACK: { return JR; break; }
        case JR_CONST: { return JR; break; }
        case JR_REG: { return JR; break; }
        case JRZ_STACK: { return JRZ; break; }
        case JRZ_CONST: { return JRZ; break; }
        case JRZ_REG: { return JRZ; break; }
        case JRNZ_STACK: { return JRNZ; break; }
        case JRNZ_CONST: { return JRNZ; break; }
        case JRNZ_REG: { return JRNZ; break; }
        case CMP_STACK: { return CMP; break; }
        case CMP_REG: { return CMP; break; }
        case CMP_CONST: { return CMP; break; }
        case CMP_REG_CONST: { return CMP; break; }
        case CMP_REG_MEM: { return CMP; break; }
        case CMP_REG_MEM_CONST: { return CMP; break; }
        case CMP_REG_REG: { return CMP; break; }
        case TEST_OP: { return TEST; break; }
        case RESET_OP: { return RESET; break; }
        case HLT_OP: { return HLT; break; }
    }
}

char* get_inst_string(Opcode op)
{
    switch (op)
    {
        default: { return "unknown opcode"; break; }
        case NOP_OP: { return "nop"; break; }
        case PUSH_CONST: { return "push"; break; }
        case PUSH_REG: { return "push"; break; }
        case PUSH_STR: { return "push"; break; }
        case POP_REG: { return "pop"; break; }
        case SET_MEM_CONST_CONST: { return "set"; break; }
        case SET_MEM_CONST_REG: { return "set"; break; }
        case SET_MEM_CONST_STR: { return "set"; break; }
        case GET_MEM_CONST_REG_MODE: { return "get"; break; }
        case MOV_REG_CONST: { return "mov"; break; }
        case MOV_REG_MEM: { return "mov"; break; }
        case MOV_REG_MEM_CONST: { return "mov"; break; }
        case MOV_REG_REG: { return "mov"; break; }
        case ADD_STACK: { return "add"; break; }
        case ADD_REG: { return "add"; break; }
        case ADD_REG_CONST: { return "add"; break; }
        case ADD_REG_MEM: { return "add"; break; }
        case ADD_REG_MEM_CONST: { return "add"; break; }
        case ADD_REG_REG: { return "add"; break; }
        case SUB_STACK: { return "sub"; break; }
        case SUB_REG: { return "sub"; break; }
        case SUB_REG_CONST: { return "sub"; break; }
        case SUB_REG_MEM: { return "sub"; break; }
        case SUB_REG_MEM_CONST: { return "sub"; break; }
        case SUB_REG_REG: { return "sub"; break; }
        case MUL_STACK: { return "mul"; break; }
        case MUL_REG: { return "mul"; break; }
        case MUL_REG_CONST: { return "mul"; break; }
        case MUL_REG_MEM: { return "mul"; break; }
        case MUL_REG_MEM_CONST: { return "mul"; break; }
        case MUL_REG_REG: { return "mul"; break; }
        case DIV_STACK: { return "div"; break; }
        case DIV_CONST: { return "div"; break; }
        case DIV_REG: { return "div"; break; }
        case INC_REG: { return "inc"; break; }
        case DEC_REG: { return "dec"; break; }
        case JMP_STACK: { return "jmp"; break; }
        case JMP_CONST: { return "jmp"; break; }
        case JMP_REG: { return "jmp"; break; }
        case JZ_STACK: { return "jz"; break; }
        case JZ_CONST: { return "jz"; break; }
        case JZ_REG: { return "jz"; break; }
        case JNZ_STACK: { return "jnz"; break; }
        case JNZ_CONST: { return "jnz"; break; }
        case JNZ_REG: { return "jnz"; break; }
        case JG_STACK: { return "jg"; break; }
        case JG_CONST: { return "jg"; break; }
        case JG_REG: { return "jg"; break; }
        case JL_STACK: { return "jl"; break; }
        case JL_CONST: { return "jl"; break; }
        case JL_REG: { return "jl"; break; }
        case JGQ_STACK: { return "jgq"; break; }
        case JGQ_CONST: { return "jgq"; break; }
        case JGQ_REG: { return "jgq"; break; }
        case JLQ_STACK: { return "jlq"; break; }
        case JLQ_CONST: { return "jlq"; break; }
        case JLQ_REG: { return "jlq"; break; }
        case JR_STACK: { return "jr"; break; }
        case JR_CONST: { return "jr"; break; }
        case JR_REG: { return "jr"; break; }
        case JRZ_STACK: { return "jrz"; break; }
        case JRZ_CONST: { return "jrz"; break; }
        case JRZ_REG: { return "jrz"; break; }
        case JRNZ_STACK: { return "jrnz"; break; }
        case JRNZ_CONST: { return "jrnz"; break; }
        case JRNZ_REG: { return "jrnz"; break; }
        case JRG_STACK: { return "jrg"; break; }
        case JRG_CONST: { return "jrg"; break; }
        case JRG_REG: { return "jrg"; break; }
        case JRL_STACK: { return "jrl"; break; }
        case JRL_CONST: { return "jrl"; break; }
        case JRL_REG: { return "jrl"; break; }
        case JRGQ_STACK: { return "jrgq"; break; }
        case JRGQ_CONST: { return "jrgq"; break; }
        case JRGQ_REG: { return "jrgq"; break; }
        case JRLQ_STACK: { return "jrlq"; break; }
        case JRLQ_CONST: { return "jrlq"; break; }
        case JRLQ_REG: { return "jrlq"; break; }
        case CMP_STACK: { return "cmp"; break; }
        case CMP_REG: { return "cmp"; break; }
        case CMP_CONST: { return "cmp"; break; }
        case CMP_REG_CONST: { return "cmp"; break; }
        case CMP_REG_MEM: { return "cmp"; break; }
        case CMP_REG_MEM_CONST: { return "cmp"; break; }
        case CMP_REG_REG: { return "cmp"; break; }
        case TEST_OP: { return "test"; break; }
        case RESET_OP: { return "reset"; break; }
        case HLT_OP: { return "hlt"; break; }
    }
}

char* get_register_string(Register reg)
{
    switch (reg)
    {
        default: { return "unknown register"; break; }
        case EAX_REGISTER: { return "eax"; break; }
        case EBX_REGISTER: { return "ebx"; break; }
        case ECX_REGISTER: { return "ecx"; break; }
        case EDX_REGISTER: { return "edx"; break; }
        case EBP_REGISTER: { return "ebp"; break; }
        case ESP_REGISTER: { return "esp"; break; }
        case EIP_REGISTER: { return "eip"; break; }
        case EIR_REGISTER: { return "eir"; break; }
        case ALU_TEST: { return "alu"; break; }
        case SV_TEST: { return "sv"; break; }
        case FLAG_REGISTER: { return "flag"; break; } // Do I need this?
    }
}

char* get_mode_string(Mode mode)
{
    switch (mode)
    {
        default: { return "unknown mode"; break; }
        case INT_MODE: { return "int"; break; }
        case STR_MODE: { return "str"; break; }
    }
}

char* get_flag_string(Flag flag)
{
    switch (flag)
    {
        default: { return "unknown flag"; break; }
        case FLAG_CARRY: { return "carry"; break; }
        case FLAG_PARITY: { return "parity"; break; }
        case FLAG_AUX_CARRY: { return "aux carry"; break; }
        case FLAG_ZERO: { return "zero"; break; }
        case FLAG_SIGN: { return "sign"; break; }
        case FLAG_TRAP: { return "trap"; break; }
        case FLAG_DIRECTION: { return "direction"; break; }
        case FLAG_OVERFLOW: { return "overflow"; break; }
    }
}
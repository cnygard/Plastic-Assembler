#include "../include/token.h"

void token_create(Token* tok, int type, int data, int line)
{
    tok->type = type;
    tok->data = data;
    tok->line = line;
}

void token_create_char(Token* tok, int type, char* data, int line)
{
    tok->type = type;
    // tok->data = data;
    tok->line = line;
}

void token_destroy(Token* tok)
{
    // free(tok);
}

void token_list_create(TokenList* list, int size)
{
    list->data = (Token*) malloc(sizeof(Token) * size);
    list->ptr = 0;
    list->size = size;
}

void token_list_add(TokenList* list, Token tok)
{
    if (list->ptr >= list->size)
    {
        list->size *= 2;
        list->data = (Token*) realloc(list->data, sizeof(Token) * list->size);
    }

    Token* t = &list->data[list->ptr++];
    token_create(t, tok.type, tok.data, tok.line);
}

Token* token_list_get(TokenList* list, int index)
{
    return &list->data[index];
}

void token_list_destroy(TokenList* list)
{
    for (int i = 0; i < list->ptr; i++)
    {
        token_destroy(&list->data[i]);
    }
    free(list->data);
}

char* get_token_inst_string(TokenInst inst)
{
    switch (inst)
    {
        default: { return "unknown instruction"; }
        case NOP: { return "nop"; }
        case PUSH: { return "push"; }
        case POP: { return "pop"; }
        case SET: { return "set"; }
        case GET: { return "get"; }
        case MOV: { return "mov"; }
        case ADD: { return "add"; }
        case SUB: { return "sub"; }
        case MUL: { return "mul"; }
        case DIV: { return "div"; }
        case INC: { return "inc"; }
        case DEC: { return "dec"; }
        case JMP: { return "jmp"; }
        case JZ: { return "jz"; }
        case JNZ: { return "jnz"; }
        case JG: { return "jg"; }
        case JL: { return "jl"; }
        case JGQ: { return "jgq"; }
        case JLQ: { return "jlq"; }
        case JR: { return "jr"; }
        case JRZ: { return "jrz"; }
        case JRNZ: { return "jrnz"; }
        case JRG: { return "jrg"; }
        case JRL: { return "jrl"; }
        case JRGQ: { return "jrgq"; }
        case JRLQ: { return "jrlq"; }
        case CMP: { return "cmp"; }
        case TEST: { return "test"; }
        case RESET: { return "reset"; }
        case HLT: { return "hlt"; }
    }
}

char* get_token_type_string(TokenType tok)
{
    switch (tok)
    {
        default: { return "unknown token type"; break; }
        case INST: { return "instruction"; break; }
        case NUMBER: { return "number"; break; }
        case REGISTER: { return "register"; break; }
        case MEMORY: { return "memory"; break; }
        case LABEL: { return "label"; break; }
        case STRING: { return "string"; break; }
        case MODE: { return "mode"; break; }
        case INST_END: { return "instruction end"; break; }
    }
}
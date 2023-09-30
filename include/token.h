#pragma once

#include <stdlib.h>

#include <stdio.h>

enum _TokenType
{
    INST = 0x00,
    NUMBER,
    REGISTER,
    MEMORY,
    LABEL,
    STRING,
    MODE,
    INST_END = 0xFF
};
typedef enum _TokenType TokenType;

enum _TokenInst
{
    NOP = 0x00,
    PUSH,
    POP,
    SET,
    GET,
    MOV,
    ADD,
    SUB,
    MUL,
    DIV,
    INC,
    DEC,
    JMP,
    JZ,
    JNZ,
    JG,
    JL,
    JGQ,
    JLQ,
    JR,
    JRZ,
    JRNZ,
    JRG,
    JRL,
    JRGQ,
    JRLQ,
    CMP,
    TEST = 0xFD,
    RESET = 0xFE,
    HLT = 0xFF
};
typedef enum _TokenInst TokenInst;

enum _TokenRegister
{
    EAX = 0,
    EBX,
    ECX,
    EDX,
    EBP,
    ESP,
    EIP,
    EIR,
    ALU,
    SV,
    FLAG = 0x0F
};
typedef enum _TokenRegister TokenRegister;

enum _TokenMode
{
    INT = 0,
    STR
};
typedef enum _TokenMode TokenMode;

struct _Token
{
    int type;
    int data;
    int line;
};
typedef struct _Token Token;

void token_create(Token* tok, int type, int data, int line);
void token_create_char(Token* tok, int type, char* data, int line);
void token_destroy(Token* tok);

struct _TokenList
{
    Token* data;
    int ptr;
    int size;
};
typedef struct _TokenList TokenList;

void token_list_create(TokenList* list, int size);
void token_list_add(TokenList* list, Token tok);
Token* token_list_get(TokenList* list, int index);
void token_list_destroy(TokenList* list);

char* get_token_type_string(TokenType tok);
char* get_token_inst_string(TokenInst inst);
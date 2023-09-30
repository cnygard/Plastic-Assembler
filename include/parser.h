#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include "token.h"
#include "util.h"

struct _Parser
{
    char* strings;
    int nextFree;
};
typedef struct _Parser Parser;

enum _ParserStatus
{
    PARSER_SUCCESS,
    PARSER_SYNTAX_ERROR
};
typedef enum _ParserStatus ParserStatus;

void preproc(char* str, int line);
ParserStatus parser_start(Parser* p, TokenList* list, const char* source);
uint32_t parser_get_number(const char* buf);
uint32_t parser_get_hex(const char* buf);
TokenInst parser_get_inst(const char* buf);
TokenRegister parser_get_reg(const char* buf);
TokenMode parser_get_mode(const char* buf);

void parser_set_string(char* str, const char* buf, int start, int end);
void parser_create_string(Parser* p);
void parser_add_string(Parser* p, char* str);
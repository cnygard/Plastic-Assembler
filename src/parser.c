#include "../include/parser.h"

ParserStatus parser_start(Parser* p, TokenList* list, const char* source)
{
    char lex[256];
    int lexi = 0;
    int i = 0;
    int line = 1;
    char delim = ' ';
    while (true)
    {
        // Reset lex
        memset(&lex, '\0', 256);
        lexi = 0;

        while(source[i] != delim)
        {
            // Preprocessors
            if (source[i] == '#' && lexi == 0)
            {
                while (source[i] != '\n' && source[i] != '\0')
                    lex[lexi++] = source[i++];
                lex[lexi] = '\0';
                preproc(lex, line);
                memset(&lex, '\0', 256);
                lexi = 0;
            }

            // End of something
            if (source[i] == '\n' || source[i] == '\0')
                break;

            // Care about empty lines and tabs
            if (lexi == 0 && (source[i] == '\n' || source[i] == '\t'))
            {
                i++;
                if (source[i] == '\n') { line++; }
                continue;
            }
            // Comments
            else if (source[i] == ';')
            {
                while (source[i] != '\n' && source[i] != '\0')
                    i++;
                break;
            }

            // Strings
            else if (source[i] == '"')
            {
                delim = '"';
            }

            lex[lexi++] = source[i++];
        }

        if (delim == '"')
        {
            delim = ' ';
            lex[lexi++] = '"';
            i++;
        }
        
        Token token;

        // Numerical hexidecimal constants
        if (isHex(lex))
        {
            int num = parser_get_hex(lex);
            // printf("NUMBER: %d\n", num);
            token_create(&token, NUMBER, num, line);
            token_list_add(list, token);
        }

        // Numerical constants
        else if (isNumber(lex))
        {
            int num = parser_get_number(lex);
            // printf("NUMBER: %d\n", num);
            token_create(&token, NUMBER, num, line);
            token_list_add(list, token);
        }

        // Labels
        else if (lex[strlen(lex) - 1] == ':')
        {
            printf("Label: %s\n", lex);
            // token_create_str(&token, LABEL, lex, line);
            // token_list_add(list, token);
        }

        // Strings
        else if (lex[0] == '"' && lex[strlen(lex) - 1] == '"')
        {
            char* str = (char*) malloc(sizeof(char) * (strlen(lex) - 1));
            parser_set_string(str, lex, 1, strlen(lex) - 1);
            token_create(&token, STRING, p->nextFree, line);
            parser_add_string(p, str);
            token_list_add(list, token);
            free(str);
        }

        // Addresses
        else if (lex[0] == '[')
        {
            lexi = 1;
            char str[256];
            for (int index = 0; index < 256; index++)
                str[index] = '\0';
            int x = 0;
            while (lex[lexi] != ']')
                str[x++] = lex[lexi++];
            int reg = parser_get_reg(str);

            lexi++;
            char sign = lex[lexi++];
            for (int index = 0; index < 256; index++)
                str[index] = '\0';
            if (sign == '+' || sign == '-')
            {
                x = 0;
                while (lex[lexi] != '\0' && lex[lexi] != '\n' && lex[lexi] != delim)
                    str[x++] = lex[lexi++];
                
                int num;
                if (isNumber(str))
                {
                    num = parser_get_number(str);
                }
                else if (isHex(str))
                {
                    num = parser_get_hex(str);
                }
                else
                {
                    printf("%d: Syntax error: must offset by number\n", line);
                    return PARSER_SYNTAX_ERROR;
                }

                num = sign == '+' ? num : -num;

                if (reg >= 0)
                {
                    token_create(&token, MEMORY, reg, line);
                    token_list_add(list, token);
                    token_create(&token, NUMBER, num, line);
                    token_list_add(list, token);
                }
                else
                {
                    printf("%d: Syntax error: no such register '%s'\n", line, lex);
                    return PARSER_SYNTAX_ERROR;
                }
            }
            else
            {
                if (reg >= 0)
                {
                    token_create(&token, MEMORY, reg, line);
                    token_list_add(list, token);
                }
                else
                {
                    printf("%d: Syntax error: no such register '%s'\n", line, lex);
                    return PARSER_SYNTAX_ERROR;
                }
            }
        }

        // Registers
        else if (parser_get_reg(lex) != -1)
        {
            // printf("%s\n", lex);
            int reg = parser_get_reg(lex);
            if (reg >= 0)
            {
                token_create(&token, REGISTER, reg, line);
                token_list_add(list, token);
            }
            else
            {
                printf("%d: Syntax error: no such register '%s'\n", line, lex);
                return PARSER_SYNTAX_ERROR;
            }
        }

        // Instructions
        else if (parser_get_inst(lex) != -1)
        {
            // printf("%s\n", lex);
            int inst = parser_get_inst(lex);
            if (inst >= 0)
            {
                // printf("INST: %s\n", lex);
                token_create(&token, INST, inst, line);
                token_list_add(list, token);
            }
            else
            {
                printf("%d: Syntax error: no such instruction '%s'\n", line, lex);
                return PARSER_SYNTAX_ERROR;
            }
        }

        // Modes
        else if (parser_get_mode(lex) != -1)
        {
            int mode = parser_get_mode(lex);
            if (mode >= 0)
            {
                token_create(&token, MODE, mode, line);
                token_list_add(list, token);
            }
            else
            {
                printf("%d: Syntax error: no such mode '%s'\n", line, lex);
                return PARSER_SYNTAX_ERROR;
            }
            token_create(&token, MODE, 0, line);
            token_list_add(list, token);
        }

        // End instructions (only used when instruction has an undefined number of args)
        else if (lex[0] == '&')
        {
            token_create(&token, INST_END, 0, line);
            token_list_add(list, token);
        }

        // Unknown
        else
        {
                printf("%d: Syntax error: unknown syntax '%s'\n", line, lex);
                return PARSER_SYNTAX_ERROR;
        }


        if (source[i] == ' ')
        {
            i++;
        }
        else if (source[i] == '\n')
        {
            i++;
            line++;
        }
        else if (source[i] == '\0')
            return PARSER_SUCCESS;
        else
            return PARSER_SYNTAX_ERROR;
    }
}

void preproc(char* str, int line)
{
    char lex[256];
    int i = 0;
    int lexi = 0;
    memset(&lex, '\0', 256);

    while (str[i] != ' ')
    {
        if (str[i] == '\0')
            return;
        
        lex[lexi++] = str[i++];
    }

    if (strcmp(lex, "#section") == 0)
    {
        lexi = 0;
        memset(&lex, '\0', 256);
        while (str[i] == '\n' || str[i] == '\0')
            lex[lexi++] = str[i++];

        if (strcmp(lex, "data") == 0)
        // no clue what to do here but I'll do it later
        return;
    }

    // Unknown
    printf("%d: Syntax error: Unknown preprocessor '%s'\n", line, str);

    return;
}

uint32_t parser_get_number(const char* buf)
{
    long num = atoi(&buf[0]);
    return (num <= UINT32_MAX) ? num : 0;
}

uint32_t parser_get_hex(const char* buf)
{
    long num = strtol(&buf[2], NULL, 16);
    return (num <= UINT32_MAX) ? num : 0;
}

TokenInst parser_get_inst(const char* buf)
{
    if (strcmp(buf, "nop") == 0) { return NOP; }
    if (strcmp(buf, "push") == 0) { return PUSH; }
    if (strcmp(buf, "pop") == 0) { return POP; }
    if (strcmp(buf, "set") == 0) { return SET; }
    if (strcmp(buf, "get") == 0) { return GET; }
    if (strcmp(buf, "mov") == 0) { return MOV; }
    if (strcmp(buf, "add") == 0) { return ADD; }
    if (strcmp(buf, "sub") == 0) { return SUB; }
    if (strcmp(buf, "mul") == 0) { return MUL; }
    if (strcmp(buf, "div") == 0) { return DIV; }
    if (strcmp(buf, "inc") == 0) { return INC; }
    if (strcmp(buf, "dec") == 0) { return DEC; }
    if (strcmp(buf, "jmp") == 0) { return JMP; }
    if (strcmp(buf, "jz") == 0) { return JZ; }
    if (strcmp(buf, "jnz") == 0) { return JNZ; }
    if (strcmp(buf, "jg") == 0) { return JG; }
    if (strcmp(buf, "jl") == 0) { return JL; }
    if (strcmp(buf, "jgq") == 0) { return JGQ; }
    if (strcmp(buf, "jlq") == 0) { return JLQ; }
    if (strcmp(buf, "jr") == 0) { return JR; }
    if (strcmp(buf, "jrz") == 0) { return JRZ; }
    if (strcmp(buf, "jrnz") == 0) { return JRNZ; }
    if (strcmp(buf, "jrg") == 0) { return JRG; }
    if (strcmp(buf, "jrl") == 0) { return JRL; }
    if (strcmp(buf, "jrgq") == 0) { return JRGQ; }
    if (strcmp(buf, "jrlq") == 0) { return JRLQ; }
    if (strcmp(buf, "cmp") == 0) { return CMP; }
    if (strcmp(buf, "test") == 0) { return TEST; }
    if (strcmp(buf, "reset") == 0) { return RESET; }
    if (strcmp(buf, "hlt") == 0) { return HLT; }
    return (TokenInst) - 1;
}

TokenRegister parser_get_reg(const char* buf)
{
    if (strcmp(buf, "eax") == 0) { return EAX; }
    if (strcmp(buf, "ebx") == 0) { return EBX; }
    if (strcmp(buf, "ecx") == 0) { return ECX; }
    if (strcmp(buf, "edx") == 0) { return EDX; }
    if (strcmp(buf, "ebp") == 0) { return EBP; }
    if (strcmp(buf, "esp") == 0) { return ESP; }
    if (strcmp(buf, "eip") == 0) { return EIP; }
    if (strcmp(buf, "eir") == 0) { return EIR; }
    if (strcmp(buf, "alu") == 0) { return ALU; }
    if (strcmp(buf, "sv") == 0) { return SV; }
    if (strcmp(buf, "flag") == 0) { return FLAG; }
    return (TokenRegister) - 1;
}

TokenMode parser_get_mode(const char* buf)
{
    if (strcmp(buf, "int") == 0) { return INT; }
    if (strcmp(buf, "str") == 0) { return STR; }
    return -1;
}

void parser_set_string(char* str, const char* buf, int start, int end)
{
    memcpy(str, buf + 1, end - start);
    str[end - start + 1] = '\0';
}

void parser_create_string(Parser* p)
{
    p->strings = (char*) malloc(sizeof(char) * 4096);
    memset(p->strings, '\0', 4095);
    p->nextFree = 0;
}

void parser_add_string(Parser* p, char* str)
{
    int size = strlen(str);
    memcpy(&p->strings[p->nextFree], str, size + 1);
    p->nextFree += size + 1;

    // testing
    int ps = 0;
    while (ps < p->nextFree)
    {
        char* output = &p->strings[ps];
        printf("%s\n", output);
        ps += strlen(output) + 1;
    }
    printf("done\n");
}
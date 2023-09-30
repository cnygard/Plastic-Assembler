#include "../include/disassembler.h"

void disassembler_error(Disassembler* d, TokenType expected, TokenType recieved)
{
    printf("0x%04x Error: Expected %s, recieved %s %d\n", d->ip, get_token_type_string(expected), get_token_type_string(recieved), recieved);
    d->running = false;
    d->status = DISASSEMBLER_ERROR;
    return;
}

void disassembler_init(Disassembler* d, uint8_t* source, uint32_t length, int pos)
{
    d->pos = pos;
    d->source = source;
    d->length = length;
    d->ip = 0;
}

void disassembler_start(Disassembler* d)
{
    d->running = true;

    while (d->running)
    {
        // printf("Runtime->ip val: %d, runtime->ip: %d\n", read8(d->source, d->ip), d->ip);

        if (d->ip >= d->length)
        {
            d->status = DISASSEMBLER_SUCCESS;
            d->running = false;
            break;
        }

        if (d_next8(d) != INST)
        {
            TokenType tmp = d->source[d->ip - 1];
            disassembler_error(d, INST, tmp);
        }
        printf("0x%04x:\t", d->ip - 1);

        switch ((uint8_t)d_next8(d))
        {
            case NOP_OP:
            {
                printf("NOP\n");
                break;
            }

            case PUSH_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("PUSH\t\t<stack>\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case PUSH_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("PUSH\t\t<stack>\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case PUSH_STR:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == STRING)
                {
                    printf("PUSH\t\t<stack>\t");
                    while (d->source[d->ip] != '\0')
                        printf("%c", d_next8(d));
                    printf("\n");
                    d_next8(d);
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case POP_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("POP\t\t%s\t<stack>\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case SET_MEM_CONST_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == MEMORY &&
                    d_next8(d) == NUMBER &&
                    d_next8(d) == NUMBER)
                {
                    if (read32(d->source, d->ip + 1) >= 0)
                        printf("SET\t\t[%s]+%d\t%d\n",
                            get_register_string(d_next8(d)),
                            d_next32(d),
                            d_next32(d));
                    else
                        printf("SET\t\t[%s]%d\t%d\n",
                            get_register_string(d_next8(d)),
                            d_next32(d),
                            d_next32(d));
                    break;
                }
                disassembler_error(d, MEMORY, tmp);
                return;
            }

            case SET_MEM_CONST_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == MEMORY &&
                    d_next8(d) == NUMBER &&
                    d_next8(d) == REGISTER)
                {
                    if (read32(d->source, d->ip + 1) >= 0)
                        printf("SET\t\t[%s]+%d\t%s\n",
                            get_register_string(d_next8(d)),
                            d_next32(d),
                            get_register_string(d_next8(d)));
                    else
                        printf("SET\t\t[%s]%d\t%s\n",
                            get_register_string(d_next8(d)),
                            d_next32(d),
                            get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, MEMORY, tmp);
                return;
            }

            case SET_MEM_CONST_STR:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == MEMORY &&
                    d_next8(d) == NUMBER &&
                    d_next8(d) == STRING)
                {
                    if (read32(d->source, d->ip + 1) >= 0)
                        printf("SET\t\t[%s]+%d\t", get_register_string(d_next8(d)), d_next32(d));
                    else
                        printf("SET\t\t[%s]%d\t", get_register_string(d_next8(d)), d_next32(d));
                    while (d->source[d->ip] != '\0')
                        printf("%c", d_next8(d));
                    printf("\n");
                    d_next8(d);
                    break;
                }
                disassembler_error(d, MEMORY, tmp);
                return;
            }

            case SET_MEM_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == MEMORY &&
                    d_next8(d) == NUMBER)
                {
                    printf("SET\t\t[%s]\t%d\n",
                        get_register_string(d_next8(d)),
                        d_next32(d));
                    break;
                }
                disassembler_error(d, MEMORY, tmp);
                return;
            }

            case SET_MEM_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == MEMORY &&
                    d_next8(d) == REGISTER)
                {
                    printf("SET\t\t[%s]\t%s\n",
                        get_register_string(d_next8(d)),
                        get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, MEMORY, tmp);
                return;
            }

            case SET_MEM_STR:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == MEMORY &&
                    d_next8(d) == STRING)
                {
                    if (read32(d->source, d->ip + 1) >= 0)
                        printf("SET\t\t[%s]\t", get_register_string(d_next8(d)));
                    while (d->source[d->ip] != '\0')
                        printf("%c", d_next8(d));
                    printf("\n");
                    d_next8(d);
                    break;
                }
                disassembler_error(d, MEMORY, tmp);
                return;
            }

            case GET_MEM_CONST_REG_MODE:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == MEMORY &&
                    d_next8(d) == NUMBER &&
                    d_next8(d) == REGISTER &&
                    d_next8(d) == MODE)
                {
                    if (read32(d->source, d->ip + 1) >= 0)
                        printf("GET\t\t[%s]+%d\t%s\t%s\n",
                            get_register_string(d_next8(d)),
                            d_next32(d),
                            get_register_string(d_next8(d)),
                            get_mode_string(d_next8(d)));
                    else
                        printf("GET\t\t[%s]%d\t%s\t%s\n",
                            get_register_string(d_next8(d)),
                            d_next32(d),
                            get_register_string(d_next8(d)),
                            get_mode_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, MEMORY, tmp);
                return;
            }

            case GET_MEM_REG_MODE:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == MEMORY &&
                    d_next8(d) == REGISTER &&
                    d_next8(d) == MODE)
                {
                    printf("GET\t\t[%s]\t%s\t%s\n",
                        get_register_string(d_next8(d)),
                        get_register_string(d_next8(d)),
                        get_mode_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, MEMORY, tmp);
                return;
            }

            case MOV_REG_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == NUMBER)
                {
                    printf("MOV\t\t%s\t%d\n", get_register_string(d_next8(d)), d_next32(d));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case MOV_REG_MEM_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == MEMORY &&
                    d_next8(d) == NUMBER)
                {
                    if (read32(d->source, d->ip + 1) >= 0)
                        printf("MOV\t\t%s\t[%s]+%d\n",
                            get_register_string(d_next8(d)),
                            get_register_string(d_next8(d)),
                            d_next32(d));
                    else
                        printf("MOV\t\t%s\t[%s]%d\n",
                            get_register_string(d_next8(d)),
                            get_register_string(d_next8(d)),
                            d_next32(d));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case MOV_REG_MEM:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == MEMORY)
                {
                    printf("MOV\t\t%s\t[%s]\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case MOV_REG_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == REGISTER)
                {
                    printf("MOV\t\t%s\t%s\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case ADD_STACK:
            {
                printf("ADD\t\t<stack>\t<stack>\n");
                break;
            }

            case ADD_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("ADD\t\t%s\t<stack>\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case ADD_REG_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == NUMBER)
                {
                    printf("ADD\t\t%s\t%d\n", get_register_string(d_next8(d)), d_next32(d));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case ADD_REG_MEM_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == MEMORY &&
                    d_next8(d) == NUMBER)
                {
                    if (read32(d->source, d->ip + 1) >= 0)
                        printf("ADD\t\t%s\t[%s]+%d\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)), d_next32(d));
                    else
                        printf("ADD\t\t%s\t[%s]%d\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)), d_next32(d));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case ADD_REG_MEM:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == MEMORY)
                {
                    printf("ADD\t\t%s\t[%s]\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case ADD_REG_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == REGISTER)
                {
                    printf("ADD\t\t%s\t%s\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case SUB_STACK:
            {
                printf("SUB\t\t<stack>\t<stack>\n");
                break;
            }

            case SUB_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("SUB\t\t%s\t<stack>\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case SUB_REG_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == NUMBER)
                {
                    printf("SUB\t\t%s\t%d\n", get_register_string(d_next8(d)), d_next32(d));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case SUB_REG_MEM_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == MEMORY &&
                    d_next8(d) == NUMBER)
                {
                    if (read32(d->source, d->ip + 1) >= 0)
                        printf("SUB\t\t%s\t[%s]+%d\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)), d_next32(d));
                    else
                        printf("SUB\t\t%s\t[%s]%d\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)), d_next32(d));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case SUB_REG_MEM:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == MEMORY)
                {
                    printf("SUB\t\t%s\t[%s]\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case SUB_REG_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == REGISTER)
                {
                    printf("SUB\t\t%s\t%s\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }
            
            case MUL_STACK:
            {
                printf("MUL\t\t<stack>\t<stack>\n");
                break;
            }

            case MUL_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("MUL\t\t%s\t<stack>\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case MUL_REG_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == NUMBER)
                {
                    printf("MUL\t\t%s\t%d\n", get_register_string(d_next8(d)), d_next32(d));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case MUL_REG_MEM_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == MEMORY &&
                    d_next8(d) == NUMBER)
                {
                    if (read32(d->source, d->ip + 1) >= 0)
                        printf("MUL\t\t%s\t[%s]+%d\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)), d_next32(d));
                    else
                        printf("MUL\t\t%s\t[%s]%d\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)), d_next32(d));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case MUL_REG_MEM:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == MEMORY)
                {
                    printf("MUL\t\t%s\t[%s]\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case MUL_REG_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == REGISTER)
                {
                    printf("MUL\t\t%s\t%s\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }
            
            case DIV_STACK:
            {
                printf("DIV\t\t<edx:eax>\t<stack>\n");
                break;
            }

            case DIV_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("DIV\t\t<edx:eax>\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case DIV_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("DIV\t\t<edx:eax>\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case INC_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("INC\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case DEC_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("DEC\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }
            
            case JMP_STACK:
            {
                printf("JMP\t\t<stack>\n");
                break;
            }

            case JMP_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JMP\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JMP_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JMP\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case JZ_STACK:
            {
                printf("JZ\t\t<stack>\n");
                break;
            }

            case JZ_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JZ\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JZ_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JZ\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JNZ_STACK:
            {
                printf("JNZ\t\t<stack>\n");
                break;
            }

            case JNZ_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JNZ\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JNZ_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JNZ\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case JG_STACK:
            {
                printf("JG\t\t<stack>\n");
                break;
            }

            case JG_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JG\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JG_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JG\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case JL_STACK:
            {
                printf("JL\t\t<stack>\n");
                break;
            }

            case JL_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JL\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JL_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JL\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case JGQ_STACK:
            {
                printf("JGQ\t\t<stack>\n");
                break;
            }

            case JGQ_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JGQ\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JGQ_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JGQ\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case JLQ_STACK:
            {
                printf("JLQ\t\t<stack>\n");
                break;
            }

            case JLQ_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JLQ\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JLQ_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JLQ\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }
            
            case JR_STACK:
            {
                printf("JR\t\t<stack>\n");
                break;
            }

            case JR_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JR\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JR_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JR\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case JRZ_STACK:
            {
                printf("JRZ\t\t<stack>\n");
                break;
            }

            case JRZ_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JRZ\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JRZ_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JRZ\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case JRNZ_STACK:
            {
                printf("JRNZ\t\t<stack>\n");
                break;
            }

            case JRNZ_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JRNZ\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JRNZ_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JRNZ\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case JRG_STACK:
            {
                printf("JRG\t\t<stack>\n");
                break;
            }

            case JRG_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JRG\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JRG_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JRG\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case JRL_STACK:
            {
                printf("JRL\t\t<stack>\n");
                break;
            }

            case JRL_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JRL\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JRL_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JRL\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case JRGQ_STACK:
            {
                printf("JRGQ\t\t<stack>\n");
                break;
            }

            case JRGQ_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JRGQ\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JRGQ_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JRGQ\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case JRLQ_STACK:
            {
                printf("JRLQ\t\t<stack>\n");
                break;
            }

            case JRLQ_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("JRLQ\t\t%d\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case JRLQ_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("JRLQ\t\t%s\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case CMP_STACK:
            {
                printf("CMP\t\t<stack>\t<stack>\n");
                break;
            }

            case CMP_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER)
                {
                    printf("CMP\t\t%s\t<stack>\n", get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case CMP_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == NUMBER)
                {
                    printf("CMP\t\t%d\t<stack>\n", d_next32(d));
                    break;
                }
                disassembler_error(d, NUMBER, tmp);
                return;
            }

            case CMP_REG_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == NUMBER)
                {
                    printf("CMP\t\t%s\t%d\n", get_register_string(d_next8(d)), d_next32(d));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case CMP_REG_MEM_CONST:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == MEMORY &&
                    d_next8(d) == NUMBER)
                {
                    if (read32(d->source, d->ip + 1) >= 0)
                        printf("CMP\t\t%s\t[%s]+%d\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)), d_next32(d));
                    else
                        printf("CMP\t\t%s\t[%s]%d\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)), d_next32(d));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case CMP_REG_MEM:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == MEMORY)
                {
                    printf("CMP\t\t%s\t[%s]\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case CMP_REG_REG:
            {
                TokenType tmp = d->source[d->ip];
                if (d_next8(d) == REGISTER &&
                    d_next8(d) == REGISTER)
                {
                    printf("CMP\t\t%s\t%s\n", get_register_string(d_next8(d)), get_register_string(d_next8(d)));
                    break;
                }
                disassembler_error(d, REGISTER, tmp);
                return;
            }

            case TEST_OP:
            {
                int indent = 2;
                if (d_next8(d) != NUMBER)
                {
                    TokenType tmp = d->source[d->ip - 1];
                    disassembler_error(d, NUMBER, tmp);
                    return;
                }
                printf("TEST\t%d", d_next32(d));
                while (d->source[d->ip] != INST_END)
                {
                    for (int tabs = 3-indent; tabs > 0; --tabs)
                        printf("\t");
                    indent = 0;

                    TokenType tmp = d->source[d->ip];
                    if (d_next8(d) == REGISTER &&
                        d_next8(d) == NUMBER)
                    {
                        printf("%s\t%d\n", get_register_string(d_next8(d)), d_next32(d));
                    }
                    else if (d_next8(d) == REGISTER &&
                        d_next8(d) == INST)
                    {
                        printf("%s\t%s\n", get_register_string(d_next8(d)), get_token_inst_string(d_next8(d)));
                    }
                    else
                    {
                        printf("\n");
                        disassembler_error(d, REGISTER, tmp);
                        return;
                    }
                }
                d_next8(d);
                break;
            }

            case RESET:
            {
                printf("RESET\n");
                break;
            }

            case HLT_OP:
            {
                printf("HLT\n");
                break;
            }

            default:
            {
                printf("Unknown instruction '0x%02x'\n", d->source[d->ip]);
                d->status = DISASSEMBLER_ERROR;
                return;
            }
        }
    }
    
    printf("Exited at: 0x%04x\n", d->ip - 1);
}



int8_t d_next8(Disassembler* d)
{
    int8_t tmp = read8(d->source, d->ip);
    d->ip++;
    return tmp;
}

int16_t d_next16(Disassembler* d)
{
    int16_t tmp = read16(d->source, d->ip);
    d->ip += 2;
    return tmp;
}

int32_t d_next32(Disassembler* d)
{
    int32_t tmp = read32(d->source, d->ip);
    d->ip += 4;
    return tmp;
}
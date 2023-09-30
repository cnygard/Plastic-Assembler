#include "../include/assembler.h"

void assembler_start(Assembler* c, Parser* p)
{
    c->bytecode = byte_buffer_create(1);

    for (int i = 0; i < c->tokens->ptr; i++)
    {
        Token* t = token_list_get(c->tokens, i);

        if (t->type == INST)
        {
            switch (t->data)
            {
                // NOP
                case NOP:
                {
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, NOP_OP);
                    break;
                }

                // PUSH
                case PUSH:
                {
                    // PUSH N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, PUSH_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // PUSH R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, PUSH_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // PUSH S
                    else if (token_list_get(c->tokens, i + 1)->type == STRING)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, PUSH_STR);
                        byte_buffer_write8(c->bytecode, STRING);
                        byte_buffer_write_string(c->bytecode, token_list_get(c->tokens, i + 1)->data, p->strings);
                        i++;
                        break;
                    }
                    printf("%d: Bad PUSH inst\n", t->line);
                    c->status = ASSEMBLER_ERROR;
                    return;
                    break;
                }

                // POP
                case POP:
                {
                    // POP R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, POP_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    printf("%d: Bad POP inst\n", t->line);
                    c->status = ASSEMBLER_ERROR;
                    return;
                    break;
                }

                // SET
                case SET:
                {
                    // SET [M]+-N N
                    if (token_list_get(c->tokens, i + 1)->type == MEMORY && 
                        token_list_get(c->tokens, i + 2)->type == NUMBER && 
                        token_list_get(c->tokens, i + 3)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, SET_MEM_CONST_CONST);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 3)->data);
                        i += 3;
                        break;
                    }
                    // SET [M]+-N R
                    else if (token_list_get(c->tokens, i + 1)->type == MEMORY && 
                        token_list_get(c->tokens, i + 2)->type == NUMBER && 
                        token_list_get(c->tokens, i + 3)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, SET_MEM_CONST_REG);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 3)->data);
                        i += 3;
                        break;
                    }
                    // SET [M]+-N S
                    else if (token_list_get(c->tokens, i + 1)->type == MEMORY && 
                        token_list_get(c->tokens, i + 2)->type == NUMBER && 
                        token_list_get(c->tokens, i + 3)->type == STRING)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, SET_MEM_CONST_STR);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, STRING);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        byte_buffer_write_string(c->bytecode, token_list_get(c->tokens, i + 3)->data, p->strings);
                        i += 3;
                        break;
                    }
                    // SET [M] N
                    else if (token_list_get(c->tokens, i + 1)->type == MEMORY && 
                        token_list_get(c->tokens, i + 2)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, SET_MEM_CONST);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // SET [M] R
                    else if (token_list_get(c->tokens, i + 1)->type == MEMORY && 
                        token_list_get(c->tokens, i + 2)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, SET_MEM_REG);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // SET [M] S
                    else if (token_list_get(c->tokens, i + 1)->type == MEMORY && 
                        token_list_get(c->tokens, i + 2)->type == STRING)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, SET_MEM_STR);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, STRING);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write_string(c->bytecode, token_list_get(c->tokens, i + 2)->data, p->strings);
                        i += 2;
                        break;
                    }
                    printf("%d: Bad SET inst\n", t->line);
                    c->status = ASSEMBLER_ERROR;
                    return;
                    break;
                }

                // GET
                case GET:
                {
                    // GET [M]+-N N T
                    if (token_list_get(c->tokens, i + 1)->type == MEMORY && 
                        token_list_get(c->tokens, i + 2)->type == NUMBER && 
                        token_list_get(c->tokens, i + 3)->type == REGISTER &&
                        token_list_get(c->tokens, i + 4)->type == MODE)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, GET_MEM_CONST_REG_MODE);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, MODE);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 3)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 4)->data);
                        i += 4;
                        break;
                    }
                    // GET [M] N T
                    if (token_list_get(c->tokens, i + 1)->type == MEMORY && 
                        token_list_get(c->tokens, i + 2)->type == REGISTER &&
                        token_list_get(c->tokens, i + 3)->type == MODE)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, GET_MEM_REG_MODE);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, MODE);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 3)->data);
                        i += 3;
                        break;
                    }
                    printf("%d: Bad GET inst\n", t->line);
                    c->status = ASSEMBLER_ERROR;
                    return;
                    break;
                }

                // MOV
                case MOV:
                {
                    // MOV R N
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type ==  NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, MOV_REG_CONST);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // MOV R [M]+-N
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == MEMORY &&
                        token_list_get(c->tokens, i + 3)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, MOV_REG_MEM_CONST);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 3)->data);
                        i += 3;
                        break;
                    }
                    // MOV R [M]
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == MEMORY)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, MOV_REG_MEM);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // MOV R R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, MOV_REG_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    printf("%d: Bad MOV inst\n", t->line);
                    c->status = ASSEMBLER_ERROR;
                    return;
                    break;
                }

                // ADD
                case ADD:
                {
                    // ADD R N
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, ADD_REG_CONST);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // ADD R [M]+-N
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == MEMORY &&
                        token_list_get(c->tokens, i + 3)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, ADD_REG_MEM_CONST);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 3)->data);
                        i += 3;
                        break;
                    }
                    // ADD R [M]
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == MEMORY)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, ADD_REG_MEM);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // ADD R R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, ADD_REG_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // ADD R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, ADD_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, ADD_STACK);
                    break;
                }

                // SUB
                case SUB:
                {
                    // SUB R N
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, SUB_REG_CONST);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // SUB R [M]+-N
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == MEMORY &&
                        token_list_get(c->tokens, i + 3)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, SUB_REG_MEM_CONST);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 3)->data);
                        i += 3;
                        break;
                    }
                    // SUB R [M]
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == MEMORY)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, SUB_REG_MEM);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // SUB R R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, SUB_REG_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // SUB R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, SUB_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, SUB_STACK);
                    break;
                }

                // MUL
                case MUL:
                {
                    // MUL R N
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, MUL_REG_CONST);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // MUL R [M]+N
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == MEMORY &&
                        token_list_get(c->tokens, i + 3)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, MUL_REG_MEM_CONST);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 3)->data);
                        i += 3;
                        break;
                    }
                    // MUL R [M]
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == MEMORY)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, MUL_REG_MEM);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // MUL R R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, MUL_REG_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // MUL R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, MUL_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, MUL_STACK);
                    break;
                }

                // DIV R
                case DIV:
                {
                    // DIV N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, DIV_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // DIV R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, DIV_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, DIV_STACK);
                    break;
                }

                // INC R
                case INC:
                {
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, INC_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    printf("%d: Bad INC inst\n", t->line);
                    c->status = ASSEMBLER_ERROR;
                    return;
                    break;
                }

                // DEC R
                case DEC:
                {
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, DEC_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    printf("%d: Bad DEC inst\n", t->line);
                    c->status = ASSEMBLER_ERROR;
                    return;
                    break;
                }

                // JMP
                case JMP:
                {
                    // JMP N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JMP_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JMP R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JMP_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JMP_STACK);
                    break;
                }

                // JZ
                case JZ:
                {
                    // JZ N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JZ_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JZ R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JZ_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JZ_STACK);
                    break;
                }

                // JNZ
                case JNZ:
                {
                    // JNZ N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JNZ_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JNZ R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JNZ_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JNZ_STACK);
                    break;
                }

                // JG
                case JG:
                {
                    // JG N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JG_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JG R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JG_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JG_STACK);
                    break;
                }

                // JL
                case JL:
                {
                    // JL N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JL_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JL R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JL_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JL_STACK);
                    break;
                }

                // JGQ
                case JGQ:
                {
                    // JGQ N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JGQ_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JGQ R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JGQ_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JGQ_STACK);
                    break;
                }

                // JLQ
                case JLQ:
                {
                    // JLQ N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JLQ_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JLQ R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JLQ_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JLQ_STACK);
                    break;
                }

                // JR
                case JR:
                {
                    // JR N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JR_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JR R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JR_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JR_STACK);
                    break;
                }

                // JRZ
                case JRZ:
                {
                    // JRZ N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JRZ_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JRZ R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JRZ_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JRZ_STACK);
                    break;
                }

                // JRNZ
                case JRNZ:
                {
                    // JRNZ N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JRNZ_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JRNZ R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JRNZ_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JRNZ_STACK);
                    break;
                }

                // JRG
                case JRG:
                {
                    // JRG N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JRG_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JRG R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JRG_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JRG_STACK);
                    break;
                }

                // JRL
                case JRL:
                {
                    // JRL N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JRL_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JRL R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JRL_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JRL_STACK);
                    break;
                }

                // JRGQ
                case JRGQ:
                {
                    // JRGQ N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JRGQ_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JRGQ R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JRGQ_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JRGQ_STACK);
                    break;
                }

                // JRLQ
                case JRLQ:
                {
                    // JRLQ N
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JRLQ_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // JRLQ R
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, JRLQ_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, JRLQ_STACK);
                    break;
                }

                // CMP
                case CMP:
                {
                    // CMP R N
                    if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, CMP_REG_CONST);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // CMP R [M]+N
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == MEMORY &&
                        token_list_get(c->tokens, i + 3)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, CMP_REG_MEM_CONST);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 3)->data);
                        i += 3;
                        break;
                    }
                    // CMP R [M]+-N
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == MEMORY)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, CMP_REG_MEM);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, MEMORY);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // CMP R R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                        token_list_get(c->tokens, i + 2)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, CMP_REG_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                        i += 2;
                        break;
                    }
                    // CMP R
                    else if (token_list_get(c->tokens, i + 1)->type == REGISTER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, CMP_REG);
                        byte_buffer_write8(c->bytecode, REGISTER);
                        byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    // CMP N
                    else if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, CMP_CONST);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        break;
                    }
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, CMP_STACK);
                    break;
                }

                // TEST N R N ... R N
                case TEST:
                {
                    if (token_list_get(c->tokens, i + 1)->type == NUMBER)
                    {
                        byte_buffer_write8(c->bytecode, INST);
                        byte_buffer_write8(c->bytecode, TEST_OP);
                        byte_buffer_write8(c->bytecode, NUMBER);
                        byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                        i++;
                        while (token_list_get(c->tokens, i + 1)->type != INST_END)
                        {
                            // R N
                            if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                                token_list_get(c->tokens, i + 2)->type == NUMBER)
                            {
                                byte_buffer_write8(c->bytecode, REGISTER);
                                byte_buffer_write8(c->bytecode, NUMBER);
                                byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                                byte_buffer_write32(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                                i += 2;
                            }
                            // R INST
                            else if (token_list_get(c->tokens, i + 1)->type == REGISTER &&
                                token_list_get(c->tokens, i + 2)->type == INST)
                            {
                                byte_buffer_write8(c->bytecode, REGISTER);
                                byte_buffer_write8(c->bytecode, INST);
                                byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 1)->data);
                                byte_buffer_write8(c->bytecode, token_list_get(c->tokens, i + 2)->data);
                                i += 2;
                            }
                            else
                            {
                                printf("%d: Bad TEST inst\n", t->line);
                                c->status = ASSEMBLER_ERROR;
                                return;
                                break;
                            }
                        }
                        byte_buffer_write8(c->bytecode, INST_END);
                        break;
                    }
                    printf("%d: Bad TEST inst\n", t->line);
                    c->status = ASSEMBLER_ERROR;
                    return;
                    break;
                }

                // RESET
                case RESET:
                {
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, RESET);
                    break;
                }

                // HLT
                case HLT:
                {
                    byte_buffer_write8(c->bytecode, INST);
                    byte_buffer_write8(c->bytecode, HLT_OP);
                    break;
                }

                default:
                {
                    printf("%d: Unknown instruction token 'type: %d data: %d'\n", token_list_get(c->tokens, i)->line, token_list_get(c->tokens, i)->type, token_list_get(c->tokens, i)->data);
                    c->status = ASSEMBLER_ERROR;
                    return;
                }
            }
        }
    }

    c->status = ASSEMBLER_SUCCESS;

    printf("Exited with code %d\n", c->status);
}
#include "../include/runtime.h"

#ifndef RELEASE
void runtime_test(uint32_t id, uint32_t regIx, uint32_t val, uint32_t expected)
{
    if (val != expected)
    {
        if (regIx == EIR_REGISTER)
        {
            printf("Test %d, %s failed: Value %s, expected %s\n", id, get_register_string(regIx), get_token_inst_string(val), get_token_inst_string(expected));
            return;
        }
        printf("Test %d, %s failed: Value %d, expected %d\n", id, get_register_string(regIx), val, expected);
        return;
    }
    else
    {
        // printf ("Test %d, %s success!\n", id, get_register_string(regIx));
    }
}
#else
void runtime_test(uint32_t id, uint32_t regIx, uint32_t val, uint32_t expected) {}
#endif

void runtime_error(Runtime* r, TokenType expected, TokenType recieved)
{
    printf("0x%04x Error: Expected %s, recieved %s %d (0x%x)\n", r->regs[EIP_REGISTER] - 1, get_token_type_string(expected), get_token_type_string(recieved), recieved, recieved);
        r->running = false;
    r->status = RUNTIME_ERROR;
    return;
}

void runtime_init(Runtime* runtime, uint8_t* code)
{
    for (int i = 0; i < runtime->regCount; i++)
    {
        runtime->regs[i] = 0;
    }
    for (int i = 0; i < STACK_SIZE; i++)
    {
        runtime->stack[i] = '\0';
    }
    runtime->alu = 1;
    runtime->code = code;
    runtime->regs[ESP_REGISTER] = -1;
    runtime->exit = 123;
    runtime->regs[EIP_REGISTER] = 0;
}

void runtime_start(Runtime* runtime)
{
    runtime->running = true;

    while (runtime->running)
    {
        if (r_next8(runtime) != INST)
        {
            TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER] - 1];
            runtime_error(runtime, INST, tmp);
        }

        // Stores previous instruction in highest order 16 bits and current instruction in lowest order 16 bits
        runtime->regs[EIR_REGISTER] <<= 16;
        runtime->regs[EIR_REGISTER] |= runtime->code[runtime->regs[EIP_REGISTER]];

        // printf("%s\n", get_inst_string(runtime->code[runtime->regs[EIP_REGISTER]]));

        // printf("alu: %d\n", runtime->alu);

        switch ((uint8_t)r_next8(runtime))
        {
            case NOP_OP:
            {
                break;
            }
            
            // PUSH N
            case PUSH_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    uint32_t a = r_next32(runtime);
                    push32(runtime, a);
                    runtime->alu = (int32_t)a;
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // PUSH R
            case PUSH_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                { 
                    uint8_t regIx = r_next8(runtime);
                    push32(runtime, runtime->regs[regIx]);
                    runtime->alu = (int32_t)runtime->regs[regIx];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // PUSH S
            case PUSH_STR:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == STRING)
                {
                    char c;
                    while ((c = r_next8(runtime)) != '\0')
                    {
                        push8(runtime, c);
                    }
                    runtime->alu = peak8(runtime, runtime->regs[ESP_REGISTER] - 1);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // POP R
            case POP_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    uint32_t a = pop32(runtime);
                    uint8_t regIx = r_next8(runtime);
                    runtime->regs[regIx] = a;
                    runtime->alu = (int32_t)runtime->regs[regIx];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // SET [M]+-N N
            case SET_MEM_CONST_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == NUMBER &&
                    r_next8(runtime) == NUMBER)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t addr = r_next32(runtime);
                    uint32_t data = r_next32(runtime);
                    set32(runtime, data, runtime->regs[regIx] + addr);
                    runtime->alu = data;
                    break;
                }
                runtime_error(runtime, MEMORY, tmp);
                return;
            }
            
            // SET [M]+-N R
            case SET_MEM_CONST_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == NUMBER &&
                    r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint32_t addr = r_next32(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    set32(runtime, runtime->regs[regIx2], runtime->regs[regIx1] + addr);
                    runtime->alu = runtime->regs[regIx2];
                    break;
                }
                runtime_error(runtime, MEMORY, tmp);
                return;
            }
            
            // SET [M]+-N S
            case SET_MEM_CONST_STR:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == NUMBER &&
                    r_next8(runtime) == STRING)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t addr = r_next32(runtime);
                    uint32_t count = runtime->regs[regIx] + addr;
                    while (runtime->code[runtime->regs[EIP_REGISTER]] != '\0')
                        set8(runtime, r_next8(runtime), count++);
                    r_next8(runtime);
                    runtime->alu = runtime->code[runtime->regs[EIP_REGISTER] - 2];
                    break;
                }
                runtime_error(runtime, MEMORY, tmp);
                return;
            }

            // SET [M] N
            case SET_MEM_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == NUMBER)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t data = r_next32(runtime);
                    set32(runtime, data, runtime->regs[regIx]);
                    runtime->alu = data;
                    break;
                }
                runtime_error(runtime, MEMORY, tmp);
                return;
            }
            
            // SET [M] R
            case SET_MEM_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    set32(runtime, runtime->regs[regIx2], runtime->regs[regIx1]);
                    runtime->alu = runtime->regs[regIx2];
                    break;
                }
                runtime_error(runtime, MEMORY, tmp);
                return;
            }
            
            // SET [M] S
            case SET_MEM_STR:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == STRING)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t count = runtime->regs[regIx];
                    while (runtime->code[runtime->regs[EIP_REGISTER]] != '\0')
                        set8(runtime, r_next8(runtime), count++);
                    r_next8(runtime);
                    runtime->alu = runtime->code[runtime->regs[EIP_REGISTER] - 2];
                    break;
                }
                runtime_error(runtime, MEMORY, tmp);
                return;
            }

            // GET [M]+-N R T
            case GET_MEM_CONST_REG_MODE:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == NUMBER &&
                    r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == MODE)
                {
                    uint8_t memReg = r_next8(runtime);
                    uint32_t addr = r_next32(runtime);
                    uint8_t destReg = r_next8(runtime);
                    uint8_t mode = r_next8(runtime);
                    if (mode == INT_MODE)
                        runtime->regs[destReg] = peak32(runtime, runtime->regs[memReg] + addr + 3);
                    else if (mode == STR_MODE)
                        runtime->regs[destReg] = peak8(runtime, runtime->regs[memReg] + addr);
                    else
                    {
                        runtime_error(runtime, MEMORY, tmp);
                        return;
                    }
                    runtime->alu = runtime->regs[destReg];
                    break;
                }
                runtime_error(runtime, MEMORY, tmp);
                return;
            }

            // GET [M] R T
            case GET_MEM_REG_MODE:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == MODE)
                {
                    uint8_t memReg = r_next8(runtime);
                    uint8_t destReg = r_next8(runtime);
                    uint8_t mode = r_next8(runtime);
                    if (mode == INT_MODE)
                        runtime->regs[destReg] = peak32(runtime, runtime->regs[memReg] + 3);
                    else if (mode == STR_MODE)
                        runtime->regs[destReg] = peak8(runtime, runtime->regs[memReg]);
                    else
                    {
                        runtime_error(runtime, MEMORY, tmp);
                        return;
                    }
                    runtime->alu = runtime->regs[destReg];
                    break;
                }
                runtime_error(runtime, MEMORY, tmp);
                return;
            }

            // MOV R N
            case MOV_REG_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == NUMBER)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t a = r_next32(runtime);
                    runtime->regs[regIx] = a;
                    runtime->alu = (int32_t)runtime->regs[regIx];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // MOV R [M]+-N
            case MOV_REG_MEM_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == NUMBER)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    int32_t a = r_next32(runtime);
                    runtime->regs[regIx1] = peak32(runtime, runtime->regs[regIx2] + a);
                    runtime->alu = (int32_t)runtime->regs[regIx1];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // MOV R [M]
            case MOV_REG_MEM:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == MEMORY)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    runtime->regs[regIx1] = peak32(runtime, runtime->regs[regIx2]);
                    runtime->alu = (int32_t)runtime->regs[regIx1];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // MOV R R
            case MOV_REG_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    runtime->regs[regIx1] = runtime->regs[regIx2];
                    runtime->alu = (int32_t)runtime->regs[regIx1];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // ADD
            case ADD_STACK:
            {
                uint32_t a = pop32(runtime);
                uint32_t b = pop32(runtime);
                push32(runtime, a + b);
                runtime->alu = (int32_t)(a + b);
                break;
            }

            // ADD R
            case ADD_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t a = pop32(runtime);
                    runtime->regs[regIx] += a;
                    runtime->alu = (int32_t)runtime->regs[regIx];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // ADD R N
            case ADD_REG_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == NUMBER)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t a = r_next32(runtime);
                    runtime->regs[regIx] += a;
                    runtime->alu = (int32_t)runtime->regs[regIx];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // ADD R [M]+-N
            case ADD_REG_MEM_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == NUMBER)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    int32_t a = r_next32(runtime);
                    runtime->regs[regIx1] += peak32(runtime, runtime->regs[regIx2] + a);
                    runtime->alu = (int32_t)runtime->regs[regIx1];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // ADD R [M]
            case ADD_REG_MEM:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == MEMORY)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    runtime->regs[regIx1] += peak32(runtime, runtime->regs[regIx2]);
                    runtime->alu = (int32_t)runtime->regs[regIx1];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // ADD R R
            case ADD_REG_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    runtime->regs[regIx1] += runtime->regs[regIx2];
                    runtime->alu = (int32_t)runtime->regs[regIx1];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // SUB
            case SUB_STACK:
            {
                uint32_t a = pop32(runtime);
                uint32_t b = pop32(runtime);
                push32(runtime, b - a);
                runtime->alu = (int32_t)(b - a);
                break;
            }

            // SUB R
            case SUB_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t a = pop32(runtime);
                    runtime->regs[regIx] -= a;
                    runtime->alu = (int32_t)runtime->regs[regIx];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // SUB R N
            case SUB_REG_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == NUMBER)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t a = r_next32(runtime);
                    runtime->regs[regIx] -= a;
                    runtime->alu = (int32_t)runtime->regs[regIx];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // SUB R [M]+-N
            case SUB_REG_MEM_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == NUMBER)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    int32_t a = r_next32(runtime);
                    runtime->regs[regIx1] -= peak32(runtime, runtime->regs[regIx2] + a);
                    runtime->alu = (int32_t)runtime->regs[regIx1];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // SUB R [M]+-N
            case SUB_REG_MEM:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == MEMORY)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    runtime->regs[regIx1] -= peak32(runtime, runtime->regs[regIx2]);
                    runtime->alu = (int32_t)runtime->regs[regIx1];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // SUB R R
            case SUB_REG_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    runtime->regs[regIx1] -= runtime->regs[regIx2];
                    runtime->alu = (int32_t)runtime->regs[regIx1];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // MUL
            case MUL_STACK:
            {
                uint32_t a = pop32(runtime);
                uint32_t b = pop32(runtime);
                push32(runtime, a * b);
                runtime->alu = (int32_t)(a * b);
                break;
            }

            // MUL R
            case MUL_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t a = pop32(runtime);
                    runtime->regs[regIx] *= a;
                    runtime->alu = (int32_t)runtime->regs[regIx];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }
            
            // MUL R N
            case MUL_REG_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == NUMBER)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t a = r_next32(runtime);
                    runtime->regs[regIx] *= a;
                    runtime->alu = (int32_t)runtime->regs[regIx];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // MUL R [M]+-N
            case MUL_REG_MEM_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == NUMBER)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    int32_t a = r_next32(runtime);
                    runtime->regs[regIx1] *= peak32(runtime, runtime->regs[regIx2] + a);
                    runtime->alu = (int32_t)runtime->regs[regIx1];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // MUL R [M]
            case MUL_REG_MEM:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == MEMORY)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    runtime->regs[regIx1] *= peak32(runtime, runtime->regs[regIx2]);
                    runtime->alu = (int32_t)runtime->regs[regIx1];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // MUL R R
            case MUL_REG_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    runtime->regs[regIx1] *= runtime->regs[regIx2];
                    runtime->alu = (int32_t)runtime->regs[regIx1];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // DIV
            case DIV_STACK:
            {
                uint32_t a = pop32(runtime);
                if (a == 0)
                {
                    printf("0x%04x: Cannot divide by 0\n", runtime->regs[EIP_REGISTER]);
                    runtime->status = RUNTIME_ERROR;
                    return;
                    break;
                }
                uint64_t numerator = combine_registers(runtime, EAX_REGISTER, EDX_REGISTER);
                runtime->regs[EAX_REGISTER] = numerator / a;
                runtime->regs[EDX_REGISTER] = numerator % a;
                runtime->alu = (int32_t)runtime->regs[EAX_REGISTER];
                break;
            }

            // DIV N
            case DIV_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    uint32_t a = r_next32(runtime);
                    if (a == 0)
                    {
                        printf("0x%04x: Cannot divide by 0\n", runtime->regs[EIP_REGISTER]);
                        runtime->status = RUNTIME_ERROR;
                        return;
                        break;
                    }
                    uint64_t numerator = combine_registers(runtime, EAX_REGISTER, EDX_REGISTER);
                    runtime->regs[EAX_REGISTER] = numerator / a;
                    runtime->regs[EDX_REGISTER] = numerator % a;
                    runtime->alu = (int32_t)runtime->regs[EAX_REGISTER];
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // DIV R
            case DIV_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx = r_next8(runtime);
                    if (runtime->regs[regIx] == 0)
                    {
                        printf("0x%04x: Cannot divide by 0\n", runtime->regs[EIP_REGISTER]);
                        runtime->running = false;
                        runtime->status = RUNTIME_ERROR;
                        return;
                        break;
                    }
                    uint64_t numerator = combine_registers(runtime, EAX_REGISTER, EDX_REGISTER);
                    runtime->regs[EAX_REGISTER] = numerator / runtime->regs[regIx];
                    runtime->regs[EDX_REGISTER] = numerator % runtime->regs[regIx];
                    runtime->alu = (int32_t)runtime->regs[EAX_REGISTER];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // INC
            case INC_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx = r_next8(runtime);
                    runtime->regs[regIx]++;
                    runtime->alu = (int32_t)runtime->regs[regIx];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // DEC
            case DEC_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx = r_next8(runtime);
                    runtime->regs[regIx]--;
                    runtime->alu = (int32_t)runtime->regs[regIx];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JMP
            case JMP_STACK:
            {
                uint32_t a = pop32(runtime);
                runtime->regs[EIP_REGISTER] = a;
                runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                break;
            }

            // JMP N
            case JMP_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    uint32_t a = r_next32(runtime);
                    runtime->regs[EIP_REGISTER] = a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JMP R
            case JMP_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx = r_next8(runtime);
                    runtime->regs[EIP_REGISTER] = runtime->regs[regIx];
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JZ
            case JZ_STACK:
            {
                if (runtime->alu == 0)
                {
                    uint32_t a = pop32(runtime);
                    runtime->regs[EIP_REGISTER] = a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                }
                break;
            }

            // JZ N
            case JZ_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    if (runtime->alu == 0)
                    {
                        uint32_t a = r_next32(runtime);
                        runtime->regs[EIP_REGISTER] = a;
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next32(runtime);
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JZ R
            case JZ_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    if (runtime->alu == 0)
                    {
                        uint8_t regIx = r_next8(runtime);
                        runtime->regs[EIP_REGISTER] = runtime->regs[regIx];
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JNZ
            case JNZ_STACK:
            {
                if (runtime->alu != 0)
                {
                    uint32_t a = pop32(runtime);
                    runtime->regs[EIP_REGISTER] = a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                }
                break;
            }

            // JNZ N
            case JNZ_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    if (runtime->alu != 0)
                    {
                        uint32_t a = r_next32(runtime);
                        runtime->regs[EIP_REGISTER] = a;
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JNZ R
            case JNZ_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    if (runtime->alu != 0)
                    {
                        uint8_t regIx = r_next8(runtime);
                        runtime->regs[EIP_REGISTER] = runtime->regs[regIx];
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JG
            case JG_STACK:
            {
                if (runtime->alu > 0)
                {
                    uint32_t a = pop32(runtime);
                    runtime->regs[EIP_REGISTER] = a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                }
                break;
            }

            // JG N
            case JG_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    if (runtime->alu > 0)
                    {
                        uint32_t a = r_next32(runtime);
                        runtime->regs[EIP_REGISTER] = a;
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next32(runtime);
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JG R
            case JG_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    if (runtime->alu > 0)
                    {
                        uint8_t regIx = r_next8(runtime);
                        runtime->regs[EIP_REGISTER] = runtime->regs[regIx];
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JL
            case JL_STACK:
            {
                if (runtime->alu < 0)
                {
                    uint32_t a = pop32(runtime);
                    runtime->regs[EIP_REGISTER] = a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                }
                break;
            }

            // JL N
            case JL_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    if (runtime->alu < 0)
                    {
                        uint32_t a = r_next32(runtime);
                        runtime->regs[EIP_REGISTER] = a;
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next32(runtime);
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JL R
            case JL_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    if (runtime->alu < 0)
                    {
                        uint8_t regIx = r_next8(runtime);
                        runtime->regs[EIP_REGISTER] = runtime->regs[regIx];
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JGQ
            case JGQ_STACK:
            {
                if (runtime->alu >= 0)
                {
                    uint32_t a = pop32(runtime);
                    runtime->regs[EIP_REGISTER] = a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                }
                break;
            }

            // JGQ N
            case JGQ_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    if (runtime->alu >= 0)
                    {
                        uint32_t a = r_next32(runtime);
                        runtime->regs[EIP_REGISTER] = a;
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next32(runtime);
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JGQ R
            case JGQ_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    if (runtime->alu >= 0)
                    {
                        uint8_t regIx = r_next8(runtime);
                        runtime->regs[EIP_REGISTER] = runtime->regs[regIx];
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JLQ
            case JLQ_STACK:
            {
                if (runtime->alu <= 0)
                {
                    uint32_t a = pop32(runtime);
                    runtime->regs[EIP_REGISTER] = a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                }
                break;
            }

            // JLQ N
            case JLQ_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    if (runtime->alu <= 0)
                    {
                        uint32_t a = r_next32(runtime);
                        runtime->regs[EIP_REGISTER] = a;
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next32(runtime);
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JLQ R
            case JLQ_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    if (runtime->alu <= 0)
                    {
                        uint8_t regIx = r_next8(runtime);
                        runtime->regs[EIP_REGISTER] = runtime->regs[regIx];
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JR
            case JR_STACK:
            {
                uint32_t a = pop32(runtime);
                runtime->regs[EIP_REGISTER] += a;
                runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                break;
            }

            // JR N
            case JR_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    uint32_t a = r_next32(runtime);
                    runtime->regs[EIP_REGISTER] += a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JR R
            case JR_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx = r_next8(runtime);
                    runtime->regs[EIP_REGISTER] += runtime->regs[regIx];
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JRZ
            case JRZ_STACK:
            {
                if (runtime->alu == 0)
                {
                    uint32_t a = pop32(runtime);
                    runtime->regs[EIP_REGISTER] += a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                }
                break;
            }

            // JRZ N
            case JRZ_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    if (runtime->alu == 0)
                    {
                        uint32_t a = r_next32(runtime);
                        runtime->regs[EIP_REGISTER] += a;
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next32(runtime);
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JRZ R
            case JRZ_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    if (runtime->alu == 0)
                    {
                        uint8_t regIx = r_next8(runtime);
                        runtime->regs[EIP_REGISTER] += runtime->regs[regIx];
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JRNZ
            case JRNZ_STACK:
            {
                if (runtime->alu != 0)
                {
                    uint32_t a = pop32(runtime);
                    runtime->regs[EIP_REGISTER] += a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                }
                break;
            }

            // JRNZ N
            case JRNZ_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    if (runtime->alu != 0)
                    {
                        uint32_t a = r_next32(runtime);
                        runtime->regs[EIP_REGISTER] += a;
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next32(runtime);
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JRNZ R
            case JRNZ_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    if (runtime->alu != 0)
                    {
                        uint8_t regIx = r_next8(runtime);
                        runtime->regs[EIP_REGISTER] += runtime->regs[regIx];
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JRG
            case JRG_STACK:
            {
                if (runtime->alu > 0)
                {
                    uint32_t a = pop32(runtime);
                    runtime->regs[EIP_REGISTER] += a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                }
                break;
            }

            // JRG N
            case JRG_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    if (runtime->alu > 0)
                    {
                        uint32_t a = r_next32(runtime);
                        runtime->regs[EIP_REGISTER] += a;
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next32(runtime);
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JRG R
            case JRG_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    if (runtime->alu > 0)
                    {
                        uint8_t regIx = r_next8(runtime);
                        runtime->regs[EIP_REGISTER] += runtime->regs[regIx];
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JRL
            case JRL_STACK:
            {
                if (runtime->alu < 0)
                {
                    uint32_t a = pop32(runtime);
                    runtime->regs[EIP_REGISTER] += a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                }
                break;
            }

            // JRL N
            case JRL_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    if (runtime->alu < 0)
                    {
                        uint32_t a = r_next32(runtime);
                        runtime->regs[EIP_REGISTER] += a;
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next32(runtime);
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JRL R
            case JRL_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    if (runtime->alu < 0)
                    {
                        uint8_t regIx = r_next8(runtime);
                        runtime->regs[EIP_REGISTER] += runtime->regs[regIx];
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JRGQ
            case JRGQ_STACK:
            {
                if (runtime->alu >= 0)
                {
                    uint32_t a = pop32(runtime);
                    runtime->regs[EIP_REGISTER] += a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                }
                break;
            }

            // JRGQ N
            case JRGQ_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    if (runtime->alu >= 0)
                    {
                        uint32_t a = r_next32(runtime);
                        runtime->regs[EIP_REGISTER] += a;
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next32(runtime);
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JRGQ R
            case JRGQ_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    if (runtime->alu >= 0)
                    {
                        uint8_t regIx = r_next8(runtime);
                        runtime->regs[EIP_REGISTER] += runtime->regs[regIx];
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // JRLQ
            case JRLQ_STACK:
            {
                if (runtime->alu <= 0)
                {
                    uint32_t a = pop32(runtime);
                    runtime->regs[EIP_REGISTER] += a;
                    runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                }
                break;
            }

            // JRLQ N
            case JRLQ_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    if (runtime->alu <= 0)
                    {
                        uint32_t a = r_next32(runtime);
                        runtime->regs[EIP_REGISTER] += a;
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next32(runtime);
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // JRLQ R
            case JRLQ_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    if (runtime->alu <= 0)
                    {
                        uint8_t regIx = r_next8(runtime);
                        runtime->regs[EIP_REGISTER] += runtime->regs[regIx];
                        runtime->alu = (int32_t)runtime->regs[EIP_REGISTER];
                        break;
                    }
                    r_next8(runtime);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // CMP
            case CMP_STACK:
            {
                uint32_t a = pop32(runtime);
                uint32_t b = pop32(runtime);
                runtime->alu = b - a;
                break;
            }

            // CMP R
            case CMP_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t a = pop32(runtime);
                    runtime->alu = runtime->regs[regIx] - a;
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // CMP N
            case CMP_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == NUMBER)
                {
                    uint32_t a = r_next32(runtime);
                    uint32_t b = pop32(runtime);
                    runtime->alu = a - b;
                    break;
                }
                runtime_error(runtime, NUMBER, tmp);
                return;
            }

            // CMP R N
            case CMP_REG_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == NUMBER)
                {
                    uint8_t regIx = r_next8(runtime);
                    uint32_t a = r_next32(runtime);
                    runtime->alu = runtime->regs[regIx] - a;
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // CMP R [M]+-N
            case CMP_REG_MEM_CONST:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == MEMORY &&
                    r_next8(runtime) == NUMBER)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    int32_t a = r_next32(runtime);
                    runtime->regs[EIP_REGISTER] += 4;
                    runtime->alu = runtime->regs[regIx1] - peak32(runtime, runtime->regs[regIx2] + a);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // CMP R [M]
            case CMP_REG_MEM:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == MEMORY)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    runtime->alu = runtime->regs[regIx1] - peak32(runtime, runtime->regs[regIx2]);
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // CMP R R
            case CMP_REG_REG:
            {
                TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                if (r_next8(runtime) == REGISTER &&
                    r_next8(runtime) == REGISTER)
                {
                    uint8_t regIx1 = r_next8(runtime);
                    uint8_t regIx2 = r_next8(runtime);
                    runtime->alu = runtime->regs[regIx1] - runtime->regs[regIx2];
                    break;
                }
                runtime_error(runtime, REGISTER, tmp);
                return;
            }

            // TEST N R N ... R N &
            case TEST_OP:
            {
                if (r_next8(runtime) != NUMBER)
                {
                    TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                    runtime_error(runtime, REGISTER, tmp);
                    return;
                }
                uint32_t tempIP = runtime->regs[EIP_REGISTER] - 1;
                uint32_t testID = r_next32(runtime);
                while (runtime->code[runtime->regs[EIP_REGISTER]] != 0xFF)
                {
                    uint32_t regIx, a;
                    TokenType tmp = runtime->code[runtime->regs[EIP_REGISTER]];
                    int8_t arg1 = r_next8(runtime);
                    int8_t arg2 = r_next8(runtime);
                    // printf("%d,%d\n", runtime->code[runtime->regs[EIP_REGISTER]], runtime->code[runtime->regs[EIP_REGISTER] + 2]);
                    if (arg1 == REGISTER &&
                        arg2 == NUMBER)
                    {
                        // printf("0x%04x: ", runtime->regs[EIP_REGISTER]);
                        regIx = r_next8(runtime);
                        a = r_next32(runtime);
                    }
                    else if (arg1 == REGISTER &&
                        arg2 == INST)
                    {
                        regIx = r_next8(runtime);
                        a = r_next32(runtime);
                        // printf("%s\n", get_token_inst_string(a));
                    }
                    else
                    {
                        runtime_error(runtime, REGISTER, tmp);
                        return;
                    }
                    if (regIx == EIR_REGISTER)
                        runtime_test(testID, regIx, opcode_to_token_inst(runtime->regs[EIR_REGISTER] >> 16), a);
                    else if (regIx == ALU_TEST)
                        runtime_test(testID, regIx, runtime->alu, a);
                    else if (regIx == EIP_REGISTER)
                        runtime_test(testID, regIx, tempIP, a);
                    else if (regIx == ESP_REGISTER)
                        runtime_test(testID, regIx, runtime->regs[ESP_REGISTER], a);
                    else if (regIx == SV_TEST)
                        runtime_test(testID, regIx, peak32(runtime, runtime->regs[ESP_REGISTER]), a);
                    else
                        runtime_test(testID, regIx, runtime->regs[regIx], a);
                }
                r_next8(runtime);
                break;
            }

            // RESET
            case RESET_OP:
            {
                uint32_t tempIP = runtime->regs[EIP_REGISTER];
                uint32_t tempIR = runtime->regs[EIR_REGISTER];
                runtime_init(runtime, runtime->code);
                runtime->regs[EIP_REGISTER] = tempIP;
                runtime->regs[EIR_REGISTER] = tempIR;
                break;
            }

            // HLT
            case HLT_OP:
            {
                // runtime->exit = runtime->regs[ESP_REGISTER] == -1 ? 0 : (uint32_t) pop32(runtime);
                runtime->exit = runtime->regs[EIP_REGISTER];
                runtime->status = RUNTIME_SUCCESS;
                runtime->running = false;
                break;
            }

            default:
            {
                printf("0x%04x: Unknown instruction '0x%x'\n", runtime->regs[EIP_REGISTER], r_next8(runtime));
                runtime->running = false;
                runtime->status = RUNTIME_ERROR;
                return;
                break;
            }
        }

        update_flags(runtime);
    }
    printf("Exited with code %d at: 0x%04x\n", runtime->status, runtime->exit);
}

int8_t r_next8(Runtime* r)
{
    uint8_t tmp = read8(r->code, r->regs[EIP_REGISTER]);
    r->regs[EIP_REGISTER]++;
    return tmp;
}

int16_t r_next16(Runtime* r)
{
    uint16_t tmp = read16(r->code, r->regs[EIP_REGISTER]);
    r->regs[EIP_REGISTER] += 2;
    return tmp;
}

int32_t r_next32(Runtime* r)
{
    uint32_t tmp = read32(r->code, r->regs[EIP_REGISTER]);
    r->regs[EIP_REGISTER] += 4;
    return tmp;
}

void push8(Runtime* r, uint8_t data)
{
    r->stack[++r->regs[ESP_REGISTER]] = data;
}

void push16(Runtime* r, uint16_t data)
{
    push8(r, (data & 0xFF00) >> 8);
    push8(r, (data & 0x00FF));
}

void push32(Runtime* r, uint32_t data)
{
    push8(r, (data & 0xFF000000) >> 24);
    push8(r, (data & 0x00FF0000) >> 16);
    push8(r, (data & 0x0000FF00) >> 8);
    push8(r, (data & 0x000000FF));
}

void set8(Runtime* r, uint8_t data, uint32_t addr)
{
    r->stack[addr] = data;
}

void set16(Runtime* r, uint16_t data, uint32_t addr)
{
    set8(r, (data & 0xFF00) >> 8, addr);
    set8(r, (data & 0x00FF), addr + 1);
}

void set32(Runtime* r, uint32_t data, uint32_t addr)
{
    set8(r, (data & 0xFF000000) >> 24, addr);
    set8(r, (data & 0x00FF0000) >> 16, addr + 1);
    set8(r, (data & 0x0000FF00) >> 8, addr + 2);
    set8(r, (data & 0x000000FF), addr + 3);
}

int8_t pop8(Runtime* r)
{
    if (r->regs[ESP_REGISTER] == -1) {
        printf("0x%04x: Stack is empty\n", r->regs[EIP_REGISTER] - 1);
        r->running = false;
        r->status = RUNTIME_ERROR;
        abort();
    }
    return r->stack[r->regs[ESP_REGISTER]--];
}

int16_t pop16(Runtime* r)
{
    if (r->regs[ESP_REGISTER] == -1) {
        printf("0x%04x: Stack is empty\n", r->regs[EIP_REGISTER] - 1);
        r->running = false;
        r->status = RUNTIME_ERROR;
        abort();
    }
    uint8_t b = pop8(r);
    uint8_t a = pop8(r);
    return (a << 8) | b;
}

int32_t pop32(Runtime* r)
{
    if (r->regs[ESP_REGISTER] == -1) {
        printf("0x%04x: Stack is empty\n", r->regs[EIP_REGISTER] - 1);
        r->running = false;
        r->status = RUNTIME_ERROR;
        abort();
    }
    uint8_t d = pop8(r);
    uint8_t c = pop8(r);
    uint8_t b = pop8(r);
    uint8_t a = pop8(r);
    return (a << 24) | (b << 16) | (c << 8) | d;
}

int8_t peak8(Runtime* r, uint32_t addr)
{
    if (addr > STACK_SIZE)
    {
        printf("0x%04x: Stack is empty\n", r->regs[EIP_REGISTER] - 1);
        r->running = false;
        r->status = RUNTIME_ERROR;
        abort();
    }
    return r->stack[addr];
}

int16_t peak16(Runtime* r, uint32_t addr)
{
    if (addr > STACK_SIZE)
    {
        printf("0x%04x: Stack is empty\n", r->regs[EIP_REGISTER] - 1);
        r->running = false;
        r->status = RUNTIME_ERROR;
        abort();
    }
    uint8_t b = peak8(r, addr);
    uint8_t a = peak8(r, addr - 1);
    return (a << 8) | b;
}

int32_t peak32(Runtime* r, uint32_t addr)
{
    if (addr > STACK_SIZE)
    {
        printf("0x%04x: Stack is empty\n", r->regs[EIP_REGISTER] - 1);
        r->running = false;
        r->status = RUNTIME_ERROR;
        abort();
    }
    uint8_t d = peak8(r, addr);
    uint8_t c = peak8(r, addr - 1);
    uint8_t b = peak8(r, addr - 2);
    uint8_t a = peak8(r, addr - 3);
    return (a << 24) | (b << 16) | (c << 8) | d;
}

uint64_t combine_registers(Runtime* r, int reg1, int reg2)
{
    return ((uint64_t)r->regs[reg2] << 32) | (uint64_t)r->regs[reg1];
}

void update_flags(Runtime* runtime)
{
    write_bit32(&runtime->regs[FLAG_REGISTER], FLAG_ZERO, runtime->alu == 0);
    write_bit32(&runtime->regs[FLAG_REGISTER], FLAG_SIGN, runtime->alu < 0);
}
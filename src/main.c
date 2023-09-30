#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/util.h"
#include "../include/parser.h"
#include "../include/assembler.h"
#include "../include/token.h"
#include "../include/runtime.h"
#include "../include/disassembler.h"

int main (int argc, char** argv)
{
    if (argc <3 && strcmp(argv[1], "test") != 0)
    {
        printf("Too few arguments\n");
        return 1;
    }

    // Assemble
    if (strcmp(argv[1], "-a") == 0)
    {
        char* source = read_ascii_file(argv[2]);
        TokenList tokens;
        token_list_create(&tokens, 1);

        Parser p;
        parser_create_string(&p);
        ParserStatus pstat = parser_start(&p, &tokens, source);
        if (pstat != PARSER_SUCCESS) {
            printf("Parser error\n");
            return 1;
        }

        Assembler a;
        a.tokens = &tokens;
        assembler_start(&a, &p);
        if (a.status != ASSEMBLER_SUCCESS) { return 1; }

        write_binary_file("out.pbc", a.bytecode);

        byte_buffer_destroy(a.bytecode);
        token_list_destroy(&tokens);
        free(source);

        return 0;
    }

    else if (strcmp(argv[1], "-r") == 0)
    {
        uint8_t* code = read_binary_file(argv[2]);

        Runtime runtime;
        runtime_init(&runtime, code);
        runtime_start(&runtime);

        if (runtime.status == RUNTIME_ERROR) { return 1; }
        // printf("eax: %d\nebx: %d\nedx: %d\nzero flag: %x\nalu: %d\nstack: %d\n",
        //                                             runtime.regs[EAX_REGISTER],
        //                                             runtime.regs[EBX_REGISTER],
        //                                             runtime.regs[EDX_REGISTER],
        //                                             runtime.regs[FLAG_REGISTER],
        //                                             runtime.alu,
        //                                             runtime.exit);

        return runtime.exit;
    }

    else if (strcmp(argv[1], "-d") == 0)
    {
        uint32_t len = get_binary_file_length(argv[2]);
        uint8_t* source = read_binary_file(argv[2]);
        
        Disassembler disassembler;
        if (argc > 3)
            disassembler_init(&disassembler, source, len, atoi(argv[3]));
        else
            disassembler_init(&disassembler, source, len, -1);
        disassembler_start(&disassembler);

        if (disassembler.status == DISASSEMBLER_ERROR) { return 1; }

        return 0;
    }

    else
    {
        printf("Unknown argument: %s\n", argv[1]);
        return -1;
    }

    return 1;
}
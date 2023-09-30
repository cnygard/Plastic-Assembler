#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <stdio.h>

struct _ByteBuffer
{
    char* buffer;
    int ptr;
    int size;
};
typedef struct _ByteBuffer ByteBuffer;

ByteBuffer* byte_buffer_create(int size);
void byte_buffer_write8(ByteBuffer* bb, uint8_t data);
void byte_buffer_write16(ByteBuffer* bb, uint16_t data);
void byte_buffer_write32(ByteBuffer* bb, uint32_t data);
void byte_buffer_write_string(ByteBuffer* bb, int data, char* strings);
void byte_buffer_destroy(ByteBuffer* bb);

void byte_buffer_itoa(char* arr, int x);
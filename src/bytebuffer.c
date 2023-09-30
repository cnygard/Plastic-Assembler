#include "../include/bytebuffer.h"

ByteBuffer* byte_buffer_create(int size)
{
    ByteBuffer* bb = (ByteBuffer*) malloc(sizeof(ByteBuffer));
    bb->buffer = (char*) malloc(sizeof(char) * size);
    bb->ptr = 0;
    bb->size = size;
    return bb;
}

void byte_buffer_write8(ByteBuffer* bb, uint8_t data)
{
    if (bb->ptr >= bb->size)
    {
        bb->size *= 2;
        bb->buffer = (char*) realloc(bb->buffer, sizeof(char) * bb->size);
    }
    bb->buffer[bb->ptr++] = data;
}

void byte_buffer_write16(ByteBuffer* bb, uint16_t data)
{
    char arr[4];
    byte_buffer_itoa(arr, data);

    byte_buffer_write8(bb, arr[0]);
    byte_buffer_write8(bb, arr[1]);
}

void byte_buffer_write32(ByteBuffer* bb, uint32_t data)
{
    char arr[4];
    byte_buffer_itoa(arr, data);

    byte_buffer_write8(bb, arr[0]);
    byte_buffer_write8(bb, arr[1]);
    byte_buffer_write8(bb, arr[2]);
    byte_buffer_write8(bb, arr[3]);
}

void byte_buffer_write_string(ByteBuffer* bb, int data, char* strings)
{
    for (int i = data; i < strlen(&strings[data]) + data; i++)
    {
        // printf("%c\n", strings[i]);
        byte_buffer_write8(bb, strings[i]);
    }
    byte_buffer_write8(bb, '\0');
}

void byte_buffer_destroy(ByteBuffer* bb)
{
    free(bb->buffer);
    free(bb);
}

void byte_buffer_itoa(char* arr, int x)
{
    arr[0] = (x >> 24) & 0xFF;
    arr[1] = (x >> 16) & 0xFF;
    arr[2] = (x >> 8)  & 0xFF;
    arr[3] = (x >> 0)  & 0xFF;
}
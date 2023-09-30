#include "../include/util.h"

char* read_ascii_file(const char* path)
{
    // Create a file
    FILE* file = fopen(path, "r");
    if (!file)
    {
        printf("Could not open file '%s'\n", path);
        return NULL;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read file
    char* buf = (char*) malloc(sizeof(char) * (size + 1));
    if (!buf)
    {
        printf("Could not allocate memory for file!\n");
        return NULL;
    }
    fread(buf, 1, size, file);
    buf[size] = '\0';
    fclose(file);

    // Return contents
    return buf;
}

uint8_t* read_binary_file(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file)
    {
        printf("Could not open file '%s'\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t* buffer = (uint8_t*) malloc(sizeof(uint8_t) * size);
    fread(buffer, 1, size, file);
    fclose(file);

    return buffer;
}

uint32_t get_binary_file_length(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file)
    {
        printf("Could not open file '%s'\n", path);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);

    return (uint32_t)size;
}

void write_binary_file(const char* path, ByteBuffer* bb)
{
    FILE* file = fopen(path, "wb");
    if (!file)
    {
        printf("Could not write to file '%s'\n", path);
        return;
    }

    fwrite(bb->buffer, 1, bb->ptr, file);
    fclose(file);
}

int8_t read8(uint8_t* buffer, uint8_t index)
{
    return (buffer[index]);
}

int16_t read16(uint8_t* buffer, uint16_t index)
{
    return (buffer[index] << 8) | (buffer[index + 1]);
}

int32_t read32(uint8_t* buffer, uint32_t index)
{
    // uint8_t b1 = buffer[index];
    // uint8_t b2 = buffer[index+1];
    // uint8_t b3 = buffer[index+2];
    // uint8_t b4 = buffer[index+3];
    // uint32_t ret = b1<<24 | b2<<16 | b3<<8 | b4;
    // printf("b1:%d, b2:%d, b3:%d, b4:%d, ret:%u\n", b1, b2, b3, b4, ret);
    // return ret;
    
    // uint32_t ret = (buffer[index] << 24) | (buffer[index + 1] << 16) |
    //     (buffer[index + 2] << 8) | (buffer[index + 3]);
    // printf("ret: %d\n", ret);
    return (buffer[index] << 24) | (buffer[index + 1] << 16) |
        (buffer[index + 2] << 8) | (buffer[index + 3]);
}

uint8_t read_bit8(uint8_t* buffer, uint8_t index, uint8_t bit)
{
    return (1 << bit) & buffer[index];
}

uint16_t read_bit16(uint8_t* buffer, uint16_t index, uint8_t bit)
{
    return (1 << bit) & read16(buffer, index);
}

uint32_t read_bit32(uint32_t buffer, uint8_t bit)
{
    return (1 << bit) & buffer;
}

void write_bit8(uint8_t* buffer, uint8_t bit, uint8_t value)
{
    uint8_t bitval = 1 << (bit - 1);
    if (value == 0)
    {
        *buffer &= ~bitval;
    }
    else
    {
        *buffer |= bitval;
    }
}

void write_bit16(uint16_t* buffer, uint8_t bit, uint8_t value)
{
    uint16_t bitval = 1 << (bit - 1);
    if (value == 0)
    {
        *buffer &= ~bitval;
    }
    else
    {
        *buffer |= bitval;
    }
}

void write_bit32(uint32_t* buffer, uint8_t bit, uint8_t value)
{
    uint32_t bitval = 1 << (bit - 1);
    if (value == 0)
    {
        *buffer &= ~bitval;
    }
    else
    {
        *buffer |= bitval;
    }
}

bool isNumber(const char* buf)
{
    int len = strlen(buf);
    int index = 0;
    if (buf[0] == '-')
        index = 1;
    for (int i = index; i < len; i++)
    {
        if ((buf[i] < '0' || buf[i] > '9') && (buf[i] < 'A' || buf[i] > 'F'))
            return false;
    }
    return true;
}

bool isHex(const char* buf)
{
    if (buf[0] == '0' && buf[1] == 'x')
    {
        int len = strlen(buf);
        for (int i = 2; i < len; i++)
        {
            if ((buf[i] < '0' || buf[i] > '9') && (buf[i] < 'a' || buf[i] > 'f'))
                return false;
        }
        return true;
    }
    else
        return false;
}
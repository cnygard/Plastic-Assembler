#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bytebuffer.h"

char* read_ascii_file(const char* path);

uint8_t* read_binary_file(const char* path);
uint32_t get_binary_file_length(const char* path);
void write_binary_file(const char* path, ByteBuffer* bb);

signed char* itoa(int x);

int8_t read8(uint8_t* buffer, uint8_t index);
int16_t read16(uint8_t* buffer, uint16_t index);
int32_t read32(uint8_t* buffer, uint32_t index);
uint8_t read_bit8(uint8_t* buffer, uint8_t index, uint8_t bit);
uint16_t read_bit16(uint8_t* buffer, uint16_t index, uint8_t bit);
uint32_t read_bit32(uint32_t buffer, uint8_t bit);
void write_bit8(uint8_t* buffer, uint8_t bit, uint8_t value);
void write_bit16(uint16_t* buffer, uint8_t bit, uint8_t value);
void write_bit32(uint32_t* buffer, uint8_t bit, uint8_t value);

bool isNumber(const char* buf);
bool isHex(const char* buf);
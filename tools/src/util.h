#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	uint8_t* data;
	size_t len;
} FileData;

FileData readFile(const char* filename);
bool     writeFile(const char* filename, uint8_t* data, size_t dataLen);

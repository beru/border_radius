#pragma once

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "trace.h"

inline
size_t GetFileSize(FILE* file)
{
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	return length;
}

template <typename T>
static inline
void OffsetPtr(T*& ptr, ptrdiff_t offsetBytes)
{
	ptr = (T*) ((const char*)ptr + offsetBytes);
}

#define STATIC_ASSERT(expr) { char dummy[(expr) ? 1 : 0]; }
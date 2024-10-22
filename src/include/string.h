// string.h
#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>  // For size_t

size_t strlen(const char *str);
int strcmp(const char *str1, const char *str2);
char *strcpy(char *dest, const char *src);
void *memcpy(void *dest, const void *src, size_t n);

#endif

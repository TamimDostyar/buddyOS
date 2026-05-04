#ifndef STRING_H
#define STRING_H

#include <stddef.h>

int strlen(const char *s);
int strcmp(const char *s1, const char *s2);
char *strtok(char *str, const char *delim);
int strcspn(const char *s1, const char *s2);
char *strstr(const char *haystack, const char *needle);

#endif
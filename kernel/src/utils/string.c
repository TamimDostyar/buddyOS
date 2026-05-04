#include "string.h"

int strlen(const char *s) {
    int len = 0;
    while (s && s[len]) len++;
    return len;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strcspn(const char *s1, const char *s2) {
    int len = 0;
    while (s1[len]) {
        for (int i = 0; s2[i]; i++) {
            if (s1[len] == s2[i]) return len;
        }
        len++;
    }
    return len;
}

static char *strtok_saved = NULL;
char *strtok(char *str, const char *delim) {
    if (!str) str = strtok_saved;
    if (!str) return NULL;

    // skip leading delimiters
    while (*str) {
        int is_delim = 0;
        for (int i = 0; delim[i]; i++) {
            if (*str == delim[i]) is_delim = 1;
        }
        if (!is_delim) break;
        str++;
    }

    if (!*str) {
        strtok_saved = NULL;
        return NULL;
    }

    char *start = str;
    while (*str) {
        int is_delim = 0;
        for (int i = 0; delim[i]; i++) {
            if (*str == delim[i]) is_delim = 1;
        }
        if (is_delim) {
            *str = '\0';
            strtok_saved = str + 1;
            return start;
        }
        str++;
    }
    strtok_saved = NULL;
    return start;
}

char *strstr(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;
    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;
        while (*h && *n && (*h == *n)) {
            h++;
            n++;
        }
        if (!*n) return (char *)haystack;
        haystack++;
    }
    return NULL;
}
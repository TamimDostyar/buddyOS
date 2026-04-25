#include "vga.h"
#include "debug.h"


void reverse(char *str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void intToStr(int num, char *str) {
    int isNegative = 0;
    int i = 0;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0) {
        isNegative = 1;
        if (num == -2147483648) {
            char minInt[] = "-2147483648";
            int j = 0;
            while (minInt[j] != '\0') {
                str[j] = minInt[j];
                j++;
            }
            str[j] = '\0';
            return;
        }
        num = -num;
    }


    while (num != 0) {
        int digit = num % 10;
        str[i++] = (char)(digit + '0');
        num /= 10;
    }
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';
    reverse(str, i);
}

void _kprintf_impl(Type type, unsigned *args) {
    char buff[12];
    
    switch (type) {
    case typeINT:
        intToStr(*(int*)args, buff);
        for (int i = 0; buff[i] != '\0'; i++) {
            vga_putchar(buff[i]);
        }
        break;
    case typeString: {
        char *str = *(char **)args;
        for (int i = 0; str[i] != '\0'; i++) {
            vga_putchar(str[i]);
        }
        break;
    }
    case typeBOOL: {
        int boolVal = *(unsigned char*)args;
        const char *str = boolVal ? "true" : "false";
        for (int i = 0; str[i] != '\0'; i++) {
            vga_putchar(str[i]);
        }
        break;
    }
    default:
        break;
    }
}
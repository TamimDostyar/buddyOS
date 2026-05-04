// #include <stdio.h>
// #include <stdlib.h>
#include "string.h"
#include "vga.h"
#include "parser.h"
#include "../builtin/builtins.h"
#include "debug.h"

void parser(char *input) {
    if (input == NULL || strlen(input) == 0) {
        kprintf_str("Error: empty input\n");
        return;
    }

    char *cmd = strtok(input, " ");
        
    char *args = strtok(NULL, "");
    
    is_builtin(cmd, args);
}


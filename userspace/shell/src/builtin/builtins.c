// #include <stdio.h>
#include "vga.h"
#include "builtins.h"
#include "string.h"
#include "debug.h"
#include "../executer/executer.h"
#include "../history/history.h"

typedef struct {
    char *name;
    void (*func)(char *args);
    char *description;
} builtin_t;


builtin_t builtins[] = {
    {"exit", quitApplication, "To exit shells"},
    {"cd", computerDirectory, "To move to a directory"},
    {"pwd", pathDirectory, "To see your file path"},
    {"ls", listDirectory, "To list your files in the directory"},
    {"clear", clearShell, "To clear shell"},
    {"history", history, "To show command history"},
    {NULL, NULL, NULL}
};

int is_builtin(char *cmd, char *args){
    // for help function
    if (cmd != NULL && strcmp(cmd, "help") == 0) {
        for (int i = 0; builtins[i].name != NULL; i++) {
            kprintf_str(builtins[i].name);
            kprintf_str(": ");
            kprintf_str(builtins[i].description);
            kprintf_str("\n");
        }
        (void)args;
        return 1;
    }

    // if commands are valid then we simply provide the output
    for (int i = 0; builtins[i].name != NULL; i ++){
        if (strcmp(builtins[i].name, cmd) == 0){
            if (strcmp("cd", cmd) == 0) {
                builtins[i].func(args);
                return 1;
            }
            builtins[i].func(args); // Using args instead of cmd as argument
            return 1;
        } 
    }
    kprintf_str("Not a valid command -> type help\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/syslimits.h>
#include <unistd.h>
#include <string.h>
#include "builtins.h"

typedef struct {
    char *name;
    void (*func)(void);
} builtin_t;


// parse functions
void quitApplication(){
    printf("Exiting...\n");
    exit(0);
}
void pathDirectory(){
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd) );
    printf("%s\n", cwd);
}

// i am thinking of using cwd to get the curren directory and when moving i can +/directory=distination

void moveDirectory(void){
    char *home = getenv("HOME");
    char cwd;

    printf("%s\n", home);

}

builtin_t builtins[] = {
    {"exit", quitApplication},
    {"cd", moveDirectory},
    {"pwd", pathDirectory},
    {NULL, NULL}
};

int is_builtin(char *cmd){
    for (int i = 0; builtins[i].name != NULL; i ++){
        if (strcmp(builtins[i].name, cmd) == 0){
            builtins[i].func();
            return 1;
        };
    }
    return 0;
}

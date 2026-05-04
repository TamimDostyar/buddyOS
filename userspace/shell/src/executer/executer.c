// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
#include "string.h"
#include "vga.h"
#include "fs.h"
#include "debug.h"


void quitApplication(char *args){
    (void)args;
    kprintf_str("Exiting is not supported in kernel mode...\n");
}

void pathDirectory(char *args){
    (void)args;
    // Just print root since we don't have deeply nested dirs yet
    kprintf_str("/\n");
}

void computerDirectory(char *args){
    char *path = args;
    (void)path;
    kprintf_str("CD not supported yet by kernel FS overlay\n");
}

void listDirectory(char *args){
    (void)args;
    // Link directly to our kernel filesystem
    fs_list();
    kprintf_str("\n");
}

void clearShell(char *args){
    (void)args;
    vga_clear();
}

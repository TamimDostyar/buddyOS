#include "vga.h"
#include "debug.h"

#define MAX_HISTORY 12
#define CMD_LEN 64

char cmd_history[MAX_HISTORY][CMD_LEN];
int hist_count = 0;
int hist_index = 0;

void history(char *args) {
    (void)args;
    if (hist_count == 0) {
        kprintf_str("No history\n");
        return;
    }
    
    int start = (hist_count < MAX_HISTORY) ? 0 : hist_index;
    for (int i = 0; i < hist_count; i++) {
        int idx = (start + i) % MAX_HISTORY;
        kprintf_int(i + 1);
        kprintf_str(": ");
        kprintf_str(cmd_history[idx]);
        kprintf_str("\n");
    }
}

void store_history(char *cmd) {
    if (cmd == 0) {
        return;
    }
    
    if (cmd[0] == '\0') {
        return;
    }
    
    int i = 0;
    while (i < CMD_LEN - 1 && cmd[i] != '\0') {
        cmd_history[hist_index][i] = cmd[i];
        i++;
    }
    cmd_history[hist_index][i] = '\0';
    
    hist_index = (hist_index + 1) % MAX_HISTORY;
    
    if (hist_count < MAX_HISTORY) {
        hist_count++;
    }
}




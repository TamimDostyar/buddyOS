#include "vga.h"
#include "builtins.h"
#include "string.h"
#include "debug.h"
#include "../executer/executer.h"
#include "../history/history.h"
#include "session.h"
#include "syscall.h"

typedef struct {
    char *name;
    void (*func)(char *args);
    char *description;
} builtin_t;

static void print_dec(uint32_t v) {
    char buf[12]; int i = 0;
    if (v == 0) { buf[i++] = '0'; }
    else { while (v) { buf[i++] = '0' + (v % 10); v /= 10; } }
    while (i--) vga_putchar(buf[i]);
}

static uint32_t parse_uint(const char *s) {
    uint32_t v = 0;
    while (s && *s >= '0' && *s <= '9') { v = v*10 + (*s - '0'); s++; }
    return v;
}

static void cmd_echo(char *args) {
    if (args) sys_write(1, args, strlen(args));
    sys_write(1, "\n", 1);
}

static void cmd_ps(char *args) {
    (void)args;
    sys_ps();
}

static void cmd_yield(char *args) {
    (void)args;
    sys_yield();
}

static void cmd_sleep(char *args) {
    uint32_t s = args ? parse_uint(args) : 1;
    sys_sleep(s * 1000);
}

static void cmd_getpid(char *args) {
    (void)args;
    vga_write("pid=");
    print_dec((uint32_t)sys_getpid());
    vga_putchar('\n');
}

static void demo_child_entry(void) {
    sys_write(1, "[child pid=", 11);
    char d[12]; int n = sys_getpid(); int i = 0;
    if (n == 0) d[i++] = '0';
    else { while (n) { d[i++] = '0'+(n%10); n /= 10; } }
    while (i--) vga_putchar(d[i]);
    sys_write(1, "] hello! sleeping 500ms, then exit.\n", 36);
    sys_sleep(500);
    sys_write(1, "[child] bye.\n", 13);
    sys_exit(0);
}

static void cmd_spawn(char *args) {
    (void)args;
    int pid = sys_fork(demo_child_entry);
    if (pid < 0) { vga_write("fork failed\n"); return; }
    vga_write("spawned pid=");
    print_dec((uint32_t)pid);
    vga_write(", waiting...\n");
    int status = 0;
    int reaped = sys_wait(&status);
    vga_write("reaped pid=");
    print_dec((uint32_t)reaped);
    vga_write(" status=");
    print_dec((uint32_t)status);
    vga_putchar('\n');
}

void whoAmI(char *args){
    (void)args;
    if (session_is_root()) { kprintf_str("root\n"); return; }
    if (session_is_authenticated()) {
        kprintf_str(session_username());
        kprintf_str("\n");
        return;
    }
    kprintf_str("guest\n");
}

builtin_t builtins[] = {
    {"exit",    quitApplication,    "Exit the shell"},
    {"cd",      computerDirectory,  "Change directory (stub)"},
    {"pwd",     pathDirectory,      "Print working directory"},
    {"ls",      listDirectory,      "List files"},
    {"whoami",  whoAmI,             "Print current user"},
    {"clear",   clearShell,         "Clear screen"},
    {"history", history,            "Show command history"},
    {"echo",    cmd_echo,           "echo TEXT — print TEXT"},
    {"ps",      cmd_ps,             "List processes"},
    {"sleep",   cmd_sleep,          "sleep N — sleep N seconds"},
    {"spawn",   cmd_spawn,          "Fork a child and wait for it"},
    {"yield",   cmd_yield,          "Yield the CPU"},
    {"getpid",  cmd_getpid,         "Print current pid"},
    {NULL, NULL, NULL}
};

int is_builtin(char *cmd, char *args){
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

    for (int i = 0; builtins[i].name != NULL; i ++){
        if (strcmp(builtins[i].name, cmd) == 0){
            builtins[i].func(args);
            return 1;
        }
    }
    kprintf_str("Not a valid command -> type help\n");
    return 0;
}

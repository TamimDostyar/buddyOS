#include "syscall.h"
#include "vga.h"
#include "keyboard.h"
#include "pit.h"
#include "process.h"
#include "scheduler.h"
#include "fs.h"
#include "debug.h"

int sys_write(int fd, const char *buf, int len) {
    if (fd != 1 && fd != 2) return -1;
    if (!buf || len < 0)    return -1;
    for (int i = 0; i < len; i++) vga_putchar(buf[i]);
    return len;
}

int sys_read(int fd, char *buf, int len) {
    if (fd != 0)        return -1;
    if (!buf || len < 1) return -1;
    char c;
    while ((c = keyboard_getchar()) == 0) {
        __asm__ volatile("sti; hlt");
    }
    buf[0] = c;
    return 1;
}

void sys_exit(int status) {
    task_exit(status);
}

int sys_fork(void (*child_entry)(void)) {
    if (!child_entry) return -1;
    return task_fork((task_entry_t)child_entry);
}

int sys_wait(int *out_status) {
    return task_wait(out_status);
}

int sys_getpid(void) {
    return task_current_pid();
}

void sys_sleep(uint32_t ms) {
    pit_sleep_ms(ms);
}

void sys_yield(void) {
    task_yield();
}

void sys_ps(void) {
    extern Process *__scheduler_lookup(int pid);
    vga_write("  PID  STATE      NAME\n");
    for (int i = 0; i < 64; i++) {
        Process *p = __scheduler_lookup(i);
        if (!p || p->processState == PROCESS_FREE) continue;
        const char *st;
        switch (p->processState) {
            case PROCESS_RUNNING: st = "RUNNING "; break;
            case PROCESS_READY:   st = "READY   "; break;
            case PROCESS_WAITING: st = "WAITING "; break;
            case PROCESS_ZOMBIE:  st = "ZOMBIE  "; break;
            default:              st = "?       "; break;
        }
        char d[6];
        int v = p->pid; int k = 0;
        if (v == 0) d[k++] = '0';
        else { char rev[6]; int rk = 0; while (v) { rev[rk++] = '0' + v%10; v/=10; }
              while (rk--) d[k++] = rev[rk]; }
        d[k] = '\0';
        vga_write("  ");
        vga_write(d);
        vga_write("   ");
        vga_write(st);
        vga_write("  ");
        vga_write(p->name[0] ? p->name : "(noname)");
        vga_putchar('\n');
    }
}

void sys_fs_list(void) {
    fs_list();
}


int syscall_dispatch(int num,
                     uint32_t arg1,
                     uint32_t arg2,
                     uint32_t arg3,
                     uint32_t arg4)
{
    (void)arg4;
    switch (num) {
        case SYS_WRITE:   return sys_write((int)arg1, (const char *)arg2, (int)arg3);
        case SYS_READ:    return sys_read((int)arg1, (char *)arg2, (int)arg3);
        case SYS_EXIT:    sys_exit((int)arg1); return 0;
        case SYS_FORK:    return sys_fork((void (*)(void))arg1);
        case SYS_EXEC:    return -1;     /* shell does fork(entry) directly */
        case SYS_WAIT:    return sys_wait((int *)arg1);
        case SYS_GETPID:  return sys_getpid();
        case SYS_SLEEP:   sys_sleep(arg1); return 0;
        case SYS_YIELD:   sys_yield(); return 0;
        case SYS_PS:      sys_ps(); return 0;
        case SYS_FS_LIST: sys_fs_list(); return 0;
        default:          return -1;
    }
}

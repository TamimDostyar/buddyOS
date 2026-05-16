#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

/* ABI: do not renumber — user wrappers hardcode these. */
#define SYS_WRITE   1
#define SYS_READ    2
#define SYS_EXIT    3
#define SYS_FORK    4
#define SYS_EXEC    5
#define SYS_WAIT    6
#define SYS_GETPID  7
#define SYS_SLEEP   8
#define SYS_YIELD   9
#define SYS_PS      10
#define SYS_FS_LIST 11

int syscall_dispatch(int num,
                     uint32_t arg1,
                     uint32_t arg2,
                     uint32_t arg3,
                     uint32_t arg4);

int  sys_write(int fd, const char *buf, int len);
int  sys_read (int fd, char *buf, int len);
void sys_exit (int status);
int  sys_fork (void (*child_entry)(void));
int  sys_wait (int *out_status);
int  sys_getpid(void);
void sys_sleep(uint32_t ms);
void sys_yield(void);
void sys_ps   (void);
void sys_fs_list(void);

#endif

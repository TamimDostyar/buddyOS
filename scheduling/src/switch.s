.code32

/* context_switch(uint32_t *save_old_esp, uint32_t new_esp)
 *
 * Stack frame on entry (low → high):
 *   [esp+0]  return addr
 *   [esp+4]  save_old_esp (arg1)
 *   [esp+8]  new_esp      (arg2)
 *
 * We push ebp/ebx/esi/edi + eflags, hand the old esp to the caller,
 * load the new esp, and pop the symmetric frame. task_create() lays
 * out a fake initial frame in the same shape. */

.global context_switch
context_switch:
    pushl %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    pushfl

    movl 24(%esp), %eax       /* save_old_esp */
    movl %esp, (%eax)

    movl 28(%esp), %eax       /* new_esp */
    movl %eax, %esp

    popfl
    popl %edi
    popl %esi
    popl %ebx
    popl %ebp
    ret

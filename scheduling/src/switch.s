.code32


.global context_switch
context_switch:
    /* C ABI: caller already pushed return address; ESP+4 = save_old_esp, ESP+8 = new_esp */
    pushl %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    pushfl

    /* save current esp into *save_old_esp */
    movl 24(%esp), %eax        /* eax = save_old_esp (arg1) */
    movl %esp, (%eax)

    /* load new esp */
    movl 28(%esp), %eax        /* eax = new_esp (arg2) */
    movl %eax, %esp

    popfl
    popl %edi
    popl %esi
    popl %ebx
    popl %ebp
    ret

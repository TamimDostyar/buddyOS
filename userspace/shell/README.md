Below is the sample shell with very minimal work:
    
    <OS>: pwd
        /Users/tamimdostyar/Documents/personal/operating-system/userspace/shell
    <OS>: ls
        .
        ..
        src
        mysh
        README.md
        remaining.txt
    <OS>: help
        exit: To exit shell
        cd: To move to a directory
        pwd: To see your file path
        ls: To list your files in the directory
        clear: To clear shell
    <OS>: cd ..
        Moving a folder back ..
        Type pwd to see your current path
    <OS>: ls
        .
        ..
        boot
        kernel
        userspace
        docs
        fs
        alloc
    <OS>: exit
        Exiting...


Run ./mysh and test on your own.
TO BE CONTINUED!

## Goals

- Evolve into BuddyShell — the AI-native shell where `ls` and "show me what I worked on yesterday" are the same kind of input
- Replace POSIX shell paradigm: every command typed is parsed by buddyGPT first, resolved to a syscall, a natural-language answer, or both
- Make shell history a semantic memory the model can reason over, not just a list of commands
- Remove the need for config files — the user tells the shell what they want, and it remembers at the kernel level

---

## Architecture

How the shell processes input today versus how BuddyShell will handle it.

```
  TODAY  (mysh)                           FUTURE  (BuddyShell)
  ──────────────────────────────          ────────────────────────────────────────

  <OS>: ls                                buddy> ls
      │  fork + exec /bin/ls                  │
      ▼                                       │  every input hits buddyGPT first
  output listed                               ▼
                                          buddyGPT  (kai/ subsystem)
  <OS>: cd ..                                 │
      │  chdir() syscall                      ├─ known command   ──► exec syscall
      ▼                                       │                      output shown
  directory changes                           │
  no AI involved                          buddy> open the file I was editing
                                                   about the networking bug
  history = list of raw strings               │
  ["ls", "cd ..", "make"]                     │  sys_ask()
                                              ▼
                                          buddyGPT reads semantic FS index
                                              │
                                              ▼
                                          returns exact file  —  no path needed
                                          editor opens it

                                          history = semantic memory
                                          the model can reason over weeks of work


  [FUTURE — network]

                                          buddy> install the tool for editing audio
                                              │
                                              ▼
                                          buddyGPT: command not found locally
                                              │  queries online command index
                                              ▼
                                          fetches correct package + install command
                                          runs it  —  shown at  buddy>
```

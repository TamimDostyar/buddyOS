# Light Project 1: Custom Shell (OS from scratch)

## Project Goal
Build a shell in C that runs in the **BuddyOS** environment and drives command execution through BuddyOS process and I/O primitives.

## What I Will Build

- command parser
- process launcher using BuddyOS spawn (or equivalent: create process, load image, run)
- built-in commands
- support for pipes and redirection

## Required Features

1. Basic command execution
- Parse input line into command and arguments.
- Run external programs using BuddyOS APIs to start a child and wait for it (foreground), not a host OS `fork`/`exec` pair unless that *is* your documented BuddyOS interface.
- Wait for foreground processes to complete before showing the next prompt.

2. Built-in commands
- cd
- exit
- history
- pwd

3. Redirection
- input redirection with <
- output redirection with >

4. Pipes
- Support at least one pipeline using |.
- Example: two small BuddyOS test programs (producer | consumer), or any pair of binaries your image provides.

5. Stability
- Handle empty input and invalid commands gracefully.
- Keep shell running until exit.

## Suggested Structure

- shell/src/manager/main.c
- shell/src/parser/parser.c
- shell/src/executer/executer.c
- shell/src/builtin/builtins.c
- shell/src/history/history.c

## Testing Checklist

- Built-ins work correctly.
- External commands run correctly under BuddyOS.
- Redirection creates/reads files correctly on BuddyOS storage.
- Single pipe flow works.
- Invalid command does not crash shell.

## OSTEP Connection

This project maps to process creation, execution, waiting, and basic shell behavior—the same ideas as “fork/exec/wait,” expressed through BuddyOS.

## Deliverable

A working BuddyOS shell executable that supports built-ins, pipes, and redirection in your target environment (serial console, framebuffer text, etc.).

# Light Project 2: UART Shell and Basic I/O (TamimOS Week 2)

## Project Overview
This project turns your bootable kernel into an interactive system.
You will build a UART-backed input path, command parsing, and a simple shell loop.

## Dependency on Project 1

- Reuse your UART initialization and transmit code from Week 1.
- Reuse stable kernel entry and startup flow.
- Your shell main loop should run only after successful boot banner.

## Goal
Build a minimal shell that accepts keyboard input via UART and executes built-in commands.

Required commands:

- help
- echo <text>
- clear
- info

The info command must print OS name and version.

## What You Should Have After This

1. Implement character input in a bare-metal environment.
2. Design a command buffer and parser without standard library parsing utilities.
3. Build a read-eval-print loop (REPL) in kernel space.
4. Handle malformed input gracefully.
5. Establish a command dispatch table that can scale in later projects.

## Functional Requirements

1. UART receive path
- Implement blocking UART RX for single character input.
- Echo typed characters back to terminal (local feedback).
- Handle carriage return/newline conversion consistently.

2. Input buffer
- Keep a fixed-size command buffer (for example 128 bytes).
- Support backspace behavior for user correction.
- Prevent buffer overflows.

3. Command parsing
- Split command name and argument text.
- Ignore leading/trailing whitespace.
- Handle empty command lines safely.

4. Shell loop
- Print prompt (for example tamimos> ).
- Read a full line.
- Parse and dispatch.
- Return prompt after command completes.

5. Built-in commands
- help: list all available commands with short descriptions.
- echo <text>: print argument text exactly once.
- clear: clear terminal using ANSI escape sequence or equivalent behavior.
- info: print TamimOS name and version (must include v0.1 or your updated internal version).

## What To Build

1. Shell source modules
- UART input handling.
- Line reader and parser.
- Command dispatcher.

2. Documentation
- Command grammar description.
- Error handling behavior for unknown commands.

3. Evidence
- Terminal transcript showing all required commands executed successfully.
- At least one unknown command test (example: foobar).

## Recommended Architecture

- uart.c / uart.h: low-level RX/TX.
- shell.c / shell.h: prompt, read loop, parse, execute.
- commands.c / commands.h: command handlers.

Use function pointers for command dispatch if possible.

## Robustness Expectations

- Unknown command prints friendly error and returns to prompt.
- Empty line should not crash or reprint noise.
- Long input beyond buffer limit should truncate or reject safely.
- Backspace updates screen and buffer consistently.

## Testing Matrix

1. Functional tests
- help returns command list.
- echo prints text payload.
- clear visibly resets terminal view.
- info reports expected OS metadata.

2. Edge tests
- Input with only spaces.
- Repeated backspace at start of line.
- Command with many arguments.
- Unrecognized command string.

3. Regression tests
- Week 1 boot banner still appears before shell starts.

## Done Criteria

- All required commands work in QEMU shell.
- Input stays stable with backspace, empty lines, and unknown commands.
- Week 1 boot banner still appears before shell starts.

## Forward Link to Week 3

The shell command framework will later call memory diagnostics and process controls.
Design your command table now so new commands can be added with minimal refactoring.

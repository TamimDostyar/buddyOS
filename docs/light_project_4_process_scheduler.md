# Light Project 4: Process Model and Round-Robin Scheduler (TamimOS Week 4)

## Project Overview
This project introduces concurrency and time-sharing in your kernel.
You will define process state, implement timer-driven context switching, and run multiple tasks.

## Dependency on Earlier Projects

- Boot path from Week 1 must be stable.
- Shell from Week 2 can launch or inspect processes.
- Memory manager from Week 3 must provide stack/space for each process.

## Goal
Implement process management and a round-robin scheduler that alternates execution between at least two runnable programs.

## What You Should Have After This

1. Design a process control block (PCB) for CPU state tracking.
2. Implement context switch save/restore mechanics.
3. Use timer interrupts to preempt running tasks.
4. Build and verify a fair round-robin scheduling policy.
5. Diagnose race and starvation issues in a minimal kernel.

## Functional Requirements

1. Process abstraction
- Define process struct with at least:
- pid
- state (NEW, READY, RUNNING, BLOCKED, TERMINATED)
- register snapshot
- stack pointer
- page-table or memory-space reference

2. Process creation
- Create API to initialize process metadata.
- Assign unique pid values.
- Allocate stack and initial CPU context.

3. Timer interrupt integration
- Configure periodic timer interrupt.
- Trigger scheduler decision on timer tick.

4. Round-robin scheduler
- Maintain runnable queue.
- Select next READY process in cyclic order.
- Perform context switch from current to next process.

5. Concurrent execution demonstration
- Load two simple programs.
- Each program prints a unique repeating message.
- Output must show alternating execution over time.

## What To Build

1. Source components
- process management module
- scheduler module
- timer interrupt handler updates
- context switch implementation (assembly likely required)

2. Validation artifacts
- Serial log showing interleaved outputs from two processes.
- Explanation of scheduling quantum or tick frequency.
- Short note on any known scheduling limitations.

3. Technical note
- Describe your context switch frame layout.
- Explain which registers are saved and why.

## Correctness Expectations

- No hard-coded switching between exactly two tasks only.
- Scheduler should handle at least N-ready process list conceptually.
- TERMINATED processes should not be selected again.
- System should continue operating when one process exits.

## Testing Requirements

1. Functional tests
- Two-process alternating print demo.
- Process creation and startup correctness.

2. Edge tests
- Single runnable process case.
- Empty run queue behavior.
- Process termination cleanup.

3. Regression tests
- Week 2 shell still operates when scheduler is active (if integrated).

## Done Criteria

- Two processes alternate output via timer preemption.
- Scheduler handles single-process and empty-queue edge cases.
- Terminated processes are not scheduled again.

## Forward Link to Week 5

Your file system and syscall layer will be consumed by scheduled user programs.
A reliable scheduler is required before exposing file I/O to processes.

# BuddyOS — top-level build (shell and future milestones)
CC       ?= cc
CFLAGS   ?= -Wall -Wextra -std=c11 -g
CPPFLAGS ?= -Ishell/src/builtin -Ishell/src/executer -Ishell/src/history -Ishell/src/parser -Ishell/src/manager
LDFLAGS  ?=
NASM     ?= nasm
QEMU     ?= qemu-system-i386

SHELL_BUILD := build/shell
SHELL_BIN   := build/mysh
BOOT_BIN    := build/boot.bin

SHELL_SRCS := $(sort $(wildcard shell/src/*/*.c))
SHELL_OBJS := $(patsubst shell/src/%.c,$(SHELL_BUILD)/%.o,$(SHELL_SRCS))

.PHONY: all shell boot run-boot clean

all: shell

shell: $(SHELL_BIN)

boot: $(BOOT_BIN)

run-boot: $(BOOT_BIN)
	$(QEMU) -drive format=raw,file=$(BOOT_BIN)

$(SHELL_BIN): $(SHELL_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BOOT_BIN): boot/boot.asm | build
	$(NASM) -f bin $< -o $@

$(SHELL_BUILD)/%.o: shell/src/%.c | $(SHELL_BUILD)
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(SHELL_BUILD):
	mkdir -p $@

build:
	mkdir -p $@

clean:
	rm -rf build

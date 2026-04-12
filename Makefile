# BuddyOS — top-level build (shell and future milestones)
CC       ?= cc
CFLAGS   ?= -Wall -Wextra -std=c11 -g
CPPFLAGS ?= -Isrc/shell
LDFLAGS  ?=

SHELL_BUILD := build/shell
SHELL_BIN   := build/mysh

SHELL_SRCS := $(sort $(wildcard src/shell/*.c))
SHELL_OBJS := $(patsubst src/shell/%.c,$(SHELL_BUILD)/%.o,$(SHELL_SRCS))

.PHONY: all shell clean

all: shell

shell: $(SHELL_BIN)

$(SHELL_BIN): $(SHELL_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(SHELL_BUILD)/%.o: src/shell/%.c | $(SHELL_BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(SHELL_BUILD):
	mkdir -p $@

clean:
	rm -rf build

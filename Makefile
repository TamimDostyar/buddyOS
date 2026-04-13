# BuddyOS — top-level build (shell and future milestones)
CC       ?= cc
CFLAGS   ?= -Wall -Wextra -std=c11 -g
CPPFLAGS ?= -Ishell/src/builtin -Ishell/src/executer -Ishell/src/history -Ishell/src/parser -Ishell/src/manager
LDFLAGS  ?=

SHELL_BUILD := build/shell
SHELL_BIN   := build/mysh

SHELL_SRCS := $(sort $(wildcard shell/src/*/*.c))
SHELL_OBJS := $(patsubst shell/src/%.c,$(SHELL_BUILD)/%.o,$(SHELL_SRCS))

.PHONY: all shell clean

all: shell

shell: $(SHELL_BIN)

$(SHELL_BIN): $(SHELL_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(SHELL_BUILD)/%.o: shell/src/%.c | $(SHELL_BUILD)
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(SHELL_BUILD):
	mkdir -p $@

clean:
	rm -rf build

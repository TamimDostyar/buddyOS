# BuddyOS — top-level build
#
# Targets:
#   all         : build the bootable image (default)
#   os-image    : build the OS bootable image
#   run         : build and run in QEMU
#   shell       : build the userspace shell
#   boot        : build only the bootloader
#   kernel      : build only the kernel
#   clean       : remove all generated files (build/)

CC       ?= cc
CFLAGS   ?= -Wall -Wextra -std=c11 -g
CPPFLAGS ?= -Iuserspace/shell/src/builtin -Iuserspace/shell/src/executer -Iuserspace/shell/src/history -Iuserspace/shell/src/parser -Iuserspace/shell/src/manager
LDFLAGS  ?=
NASM     ?= nasm
QEMU     ?= qemu-system-i386

CROSS_PREFIX := i686-elf-
CC_KERNEL    := $(CROSS_PREFIX)gcc
LD_KERNEL    := $(CROSS_PREFIX)ld
OBJCOPY      := $(CROSS_PREFIX)objcopy

# Source directories (never written to)
BOOT_DIR   := boot
KERNEL_SRC := kernel/src
ALLOC_SRC  := alloc/src
FS_SRC     := fs/src
SCHED_SRC  := scheduling/src

# Kernel include paths - used by all kernel compilation (defined AFTER source dirs)
KERNEL_INCLUDES := -I$(KERNEL_SRC)/utils \
	-I$(KERNEL_SRC)/manager \
	-I$(ALLOC_SRC) \
	-I$(FS_SRC) \
	-I$(SCHED_SRC) \
	-Iuserspace/shell/src/builtin \
	-Iuserspace/shell/src/executer \
	-Iuserspace/shell/src/history \
	-Iuserspace/shell/src/parser \
	-Iuserspace/shell/src/manager

# Kernel compiler flags
KERNEL_CFLAGS := -m32 -ffreestanding -nostdlib

# All generated output lives under build/
BUILD_DIR      := build
SHELL_BUILD    := $(BUILD_DIR)/shell
BOOT_BUILD     := $(BUILD_DIR)/boot
KERNEL_BUILD   := $(BUILD_DIR)/kernel
ALLOC_BUILD    := $(BUILD_DIR)/alloc
FS_BUILD       := $(BUILD_DIR)/fs
SCHED_BUILD    := $(BUILD_DIR)/sched

SHELL_BIN  := $(BUILD_DIR)/mysh
BOOT_BIN   := $(BUILD_DIR)/boot.bin
KERNEL_ELF := $(BUILD_DIR)/kernel.elf
KERNEL_BIN := $(BUILD_DIR)/kernel.bin
OS_IMAGE   := $(BUILD_DIR)/os_image.bin

# Shell sources (moved into kernel build)
SHELL_SRCS := $(sort $(wildcard userspace/shell/src/*/*.c) userspace/shell/src/main.c)
SHELL_OBJS := $(patsubst userspace/shell/src/%.c,$(SHELL_BUILD)/%.o,$(SHELL_SRCS))

# Kernel objects
KERNEL_ENTRY  := $(BOOT_BUILD)/entry.o
BOOT_ASM_OBJS := $(BOOT_BUILD)/isr_stubs.o
SCHED_ASM_OBJ := $(SCHED_BUILD)/switch.o
KERNEL_C_SRCS := $(wildcard $(KERNEL_SRC)/manager/*.c $(KERNEL_SRC)/utils/*.c)
ALLOC_C_SRCS  := $(wildcard $(ALLOC_SRC)/*.c)
FS_C_SRCS     := $(wildcard $(FS_SRC)/*.c)
SCHED_C_SRCS  := $(wildcard $(SCHED_SRC)/*.c)
KERNEL_OBJS   := $(KERNEL_ENTRY) \
	$(BOOT_ASM_OBJS) \
	$(SCHED_ASM_OBJ) \
	$(patsubst $(KERNEL_SRC)/%.c,$(KERNEL_BUILD)/%.o,$(KERNEL_C_SRCS)) \
	$(patsubst $(ALLOC_SRC)/%.c,$(ALLOC_BUILD)/%.o,$(ALLOC_C_SRCS)) \
	$(patsubst $(FS_SRC)/%.c,$(FS_BUILD)/%.o,$(FS_C_SRCS)) \
	$(patsubst $(SCHED_SRC)/%.c,$(SCHED_BUILD)/%.o,$(SCHED_C_SRCS)) \
	$(SHELL_OBJS)

all: os-image

shell: $(SHELL_BIN)

boot: $(BOOT_BIN)

kernel: $(KERNEL_BIN)

os-image: boot kernel
	@KSIZE=$$(wc -c < $(KERNEL_BIN)); \
	if [ $$KSIZE -gt 25088 ]; then \
	  echo "WARNING: kernel.bin is $$KSIZE bytes (>25088). Boot reads 50 sectors total. Bump 'mov al' in boot.asm."; \
	fi
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(OS_IMAGE)
	@SIZE=$$(wc -c < $(OS_IMAGE)); \
	if [ $$SIZE -lt 32256 ]; then \
	dd if=/dev/zero bs=1 count=$$((32256 - $$SIZE)) >> $(OS_IMAGE) 2>/dev/null; \
	fi

run: os-image
	$(QEMU) -drive format=raw,file=$(OS_IMAGE)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all shell boot kernel os-image run clean

$(SHELL_BIN): $(SHELL_OBJS) | $(BUILD_DIR)
	@echo "Shell sources are linked into the kernel image; build 'kernel' or 'os-image' for a runnable artifact."
	@touch $@

$(SHELL_BUILD)/%.o: userspace/shell/src/%.c | $(SHELL_BUILD)
	mkdir -p $(dir $@)
	$(CC_KERNEL) $(KERNEL_CFLAGS) $(KERNEL_INCLUDES) -c -o $@ $<

$(BOOT_BIN): $(BOOT_DIR)/boot.asm | $(BUILD_DIR)
	$(NASM) -f bin $< -o $@

$(BOOT_BUILD)/entry.o: $(BOOT_DIR)/entry.asm | $(BOOT_BUILD)
	$(NASM) -f elf32 $< -o $@

$(BOOT_BUILD)/isr_stubs.o: $(BOOT_DIR)/isr_stubs.s | $(BOOT_BUILD)
	$(CC_KERNEL) $(KERNEL_CFLAGS) -c -o $@ $<

$(SCHED_BUILD)/switch.o: $(SCHED_SRC)/switch.s | $(SCHED_BUILD)
	$(CC_KERNEL) $(KERNEL_CFLAGS) -c -o $@ $<

$(KERNEL_BUILD)/%.o: $(KERNEL_SRC)/%.c | $(KERNEL_BUILD)
	mkdir -p $(dir $@)
	$(CC_KERNEL) $(KERNEL_CFLAGS) $(KERNEL_INCLUDES) -c -o $@ $<

$(ALLOC_BUILD)/%.o: $(ALLOC_SRC)/%.c | $(ALLOC_BUILD)
	$(CC_KERNEL) $(KERNEL_CFLAGS) $(KERNEL_INCLUDES) -c -o $@ $<

$(FS_BUILD)/%.o: $(FS_SRC)/%.c | $(FS_BUILD)
	mkdir -p $(dir $@)
	$(CC_KERNEL) $(KERNEL_CFLAGS) $(KERNEL_INCLUDES) -c -o $@ $<

$(SCHED_BUILD)/%.o: $(SCHED_SRC)/%.c | $(SCHED_BUILD)
	$(CC_KERNEL) $(KERNEL_CFLAGS) $(KERNEL_INCLUDES) -c -o $@ $<

$(KERNEL_BIN): $(KERNEL_OBJS) | $(BUILD_DIR)
	$(LD_KERNEL) -m elf_i386 -e _start -Ttext 0x1000 -o $(KERNEL_ELF) $^
	$(OBJCOPY) -O binary $(KERNEL_ELF) $@

$(BUILD_DIR) $(SHELL_BUILD) $(BOOT_BUILD) $(KERNEL_BUILD) $(ALLOC_BUILD) $(FS_BUILD) $(SCHED_BUILD):
	mkdir -p $@

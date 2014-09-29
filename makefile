# When changing version here, update version in grub.cfg accordingly.
VERSION=camelot-0.1.0

# Whether or not the testing branches of code will be compiled.
TEST=false
# Whether or not the build will be setup for debugging.
DEBUG=true
# Whether or not GDB will debug with QEMU.
QEMU_DEBUG=false
# Whether or not the objects will be stripped of their symbols.
STRIP=false

# generic sources list (case sensitive, by directory)
########################################################################
SRC=\
boot/boot.c \
boot/boot.s \
boot/cpu.c \
boot/memory.c \
boot/memory_map.c \
\
boot/drivers/graphics/vga_color_text_mode.c \
\
boot/kernel/kernel.c \
boot/kernel/kernel_initialize.c \
\
boot/kernel/kernel_test.c \
\
boot/kernel/gdt/gdt.c \
boot/kernel/gdt/gdt.s \
\
boot/kernel/interrupts/idt.c \
boot/kernel/interrupts/idt.s \
boot/kernel/interrupts/isr.s \
\
\
boot/kernel/libc/ctype/ctype.c \
\
boot/kernel/libc/stdlib/stdlib.c \
\
boot/kernel/libc/string/string.c \
\
boot/ui/terminal.c \
########################################################################

BIN=$(VERSION).bin
ISO=$(BIN:.bin=.iso)
SYM=$(BIN:.bin=.sym)

# build root directory
BUILD_DIR=../build

# directories to export objects, binaries, ISOs, etc.
OBJ_DIR=$(BUILD_DIR)/obj
ASM_DIR=$(BUILD_DIR)/asm
BIN_DIR=$(BUILD_DIR)/bin
GRUB_DIR=$(BUILD_DIR)/grub
ISO_DIR=$(BUILD_DIR)/iso
RELEASE_DIR=$(BUILD_DIR)/release
DEBUG_DIR=$(TEST_DIR)/debug

# directories for testing
TEST_DIR=$(BUILD_DIR)/test
QEMU_DIR=$(TEST_DIR)/qemu

# objects list made from generic sources list
OBJ=$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC)))

# assembly list made from generic sources list
ASM=$(addprefix $(ASM_DIR)/, $(addsuffix .s, $(SRC)))

# additional include paths
INCLUDE_PATH=\
. \
globals \
includes

# script for adding specified include paths
INCLUDE_SCRIPT=$(addprefix -I, $(INCLUDE_PATH))

# additional library paths
LIBRARY_PATH=\
../lib

# terminal emulator to execute new windows in
TERMINAL=xterm

# compiler settings
export PATH:=$(PATH)
TARGET=i586-elf
OPTIMIZE=-O2
AS=$(TARGET)-as
ASFLAGS=
CC=$(TARGET)-gcc
CCFLAGS=\
	$(INCLUDE_SCRIPT) \
	$(OPTIMIZE) \
	-std=gnu11 \
	-ffreestanding \
	-Wall \
	-Wextra
CPP=$(TARGET)-g++
CPPFLAGS=\
	$(INCLUDE_SCRIPT) \
	$(OPTIMIZE) \
	-std=gnu++11 \
	-ffreestanding \
	-Wall \
	-Wextra \
	-fno-exceptions \
	-fno-rtti
LD=$(CC)
LDFLAGS=$(OPTIMIZE) -ffreestanding -nostdlib -lgcc

# defines TEST for compilers if makefile variable TEST is defined as "true"
ifeq ($(TEST), true)
CCFLAGS+=-DTEST
CPPFLAGS+=-DTEST
endif

# dependencies for testing
TEST_DEPENDENCIES=$(OBJ) \
	$(BIN_DIR)/$(BIN) \
	$(ISO_DIR)/$(ISO)

# adds additional compiler flags if not stripping symbols
ifeq ($(STRIP), false)
ASFLAGS+=-gdwarf-2
CCFLAGS+=-gdwarf-2
CPPFLAGS+=-gdwarf-2
endif

# adds additional compiler flags if stripping symbols
ifeq ($(STRIP), true)
CCFLAGS+=-s
CPPFLAGS+=-s
endif

# adds debug dependencies if debugging
ifeq ($(DEBUG), true)
TEST_DEPENDENCIES+=\
	$(DEBUG_DIR)/$(BIN) \
	$(DEBUG_DIR)/$(SYM) \
	$(DEBUG_DIR)/$(ISO_DIR)/$(ISO)
else
CCFLAGS+=-DNDEBUG
CPPFLAGS+=-DNDEBUG
endif

# QEMU disk settings
QEMU_DISK_FORMAT=qcow2
QEMU_DISK=disk.img
QEMU_DISK_SIZE=30M

#QEMU flags settings
QEMU_MEMORY=32
QEMU_CPU=1

#QEMU flags
QEMU_FLAGS=\
	-boot d \
	-m $(QEMU_MEMORY) \
	-smp $(QEMU_CPU) \
	-soundhw pcspk,sb16

# build objects, binary, and disk image
.PHONY: all
all: obj bin iso

# remove all output files and folders
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# target to build all objects
.PHONY: obj
obj: $(OBJ)

# build output binary
$(BIN_DIR)/$(BIN): $(OBJ)
	mkdir -p $(BIN_DIR)
	$(LD) -T linker.ld -o $(BIN_DIR)/$(BIN) $(LDFLAGS) $(OBJ) -lgcc

# strip binary if desired
ifeq ($(STRIP), true)
	strip --strip-all $(BIN_DIR)/$(BIN)
endif

# target to build output binary
.PHONY: bin
bin: $(OBJ) $(BIN_DIR)/$(BIN)

# target to export assembled source files
.PHONY: asm
asm: $(ASM)

# create bootable disk image using GRUB bootloader
$(ISO_DIR)/$(ISO): $(OBJ) $(BIN_DIR)/$(BIN)
	mkdir -p $(GRUB_DIR)
	mkdir -p $(GRUB_DIR)/iso
	mkdir -p $(GRUB_DIR)/iso/boot
	cp $(BIN_DIR)/$(BIN) $(GRUB_DIR)/iso/boot/$(BIN)
	mkdir -p $(GRUB_DIR)/iso/boot/grub
	cp grub.cfg $(GRUB_DIR)/iso/boot/grub/grub.cfg
	mkdir -p $(ISO_DIR)
	grub-mkrescue \
		--directory=/usr/lib/grub/i386-pc \
		--output=$(ISO_DIR)/$(ISO) $(GRUB_DIR)/iso

# target to create bootable disk image using GRUB bootloader
iso: $(OBJ) $(BIN_DIR)/$(BIN) $(ISO_DIR)/$(ISO)

# export debug symbols
$(DEBUG_DIR)/$(SYM): $(OBJ) $(BIN_DIR)/$(BIN)
	mkdir -p $(DEBUG_DIR)
	objcopy --only-keep-debug \
		$(BIN_DIR)/$(BIN) $(DEBUG_DIR)/$(SYM)

# export debug binary
$(DEBUG_DIR)/$(BIN): $(OBJ) $(BIN_DIR)/$(BIN)
	mkdir -p $(DEBUG_DIR)
	cp $(BIN_DIR)/$(BIN) $(DEBUG_DIR)/$(BIN)
	objcopy --strip-debug $(DEBUG_DIR)/$(BIN)

# export bootable debug disk
$(DEBUG_DIR)/$(ISO_DIR)/$(ISO): $(OBJ) $(BIN_DIR)/$(BIN) \
	$(DEBUG_DIR)/$(SYM) $(DEBUG_DIR)/$(BIN)

	mkdir -p $(DEBUG_DIR)/$(GRUB_DIR)
	mkdir -p $(DEBUG_DIR)/$(GRUB_DIR)/iso
	mkdir -p $(DEBUG_DIR)/$(GRUB_DIR)/iso/boot
	cp $(BIN_DIR)/$(BIN) $(DEBUG_DIR)/$(GRUB_DIR)/iso/boot/$(BIN)
	mkdir -p $(DEBUG_DIR)/$(GRUB_DIR)/iso/boot/grub
	cp grub.cfg $(DEBUG_DIR)/$(GRUB_DIR)/iso/boot/grub/grub.cfg
	mkdir -p $(DEBUG_DIR)/$(ISO_DIR)
	grub-mkrescue \
		--directory=/usr/lib/grub/i386-pc \
		--output=$(DEBUG_DIR)/$(ISO_DIR)/$(ISO) \
			$(DEBUG_DIR)/$(GRUB_DIR)/iso

# run bootable disk image using QEMU
.PHONY: qemu
qemu: $(TEST_DEPENDENCIES)
	mkdir -p $(QEMU_DIR)
	qemu-img create \
		-f $(QEMU_DISK_FORMAT) \
		$(QEMU_DIR)/$(QEMU_DISK) \
		$(QEMU_DISK_SIZE)
ifeq ($(QEMU_DEBUG), true)
	$(TERMINAL) -e \
		qemu-system-i386 \
			-monitor stdio \
			-s \
			-S \
			-hda $(QEMU_DIR)/$(QEMU_DISK) \
			-cdrom $(DEBUG_DIR)/$(ISO_DIR)/$(ISO) \
			$(QEMU_FLAGS) \
	& \
	gdb \
		--tui \
		--quiet \
		-ex "set confirm off" \
		-ex "target remote localhost:1234" \
		-ex "symbol-file $(DEBUG_DIR)/$(SYM)"
else
	$(TERMINAL) -e \
		qemu-system-i386 \
			-monitor stdio \
			-S \
			-hda $(QEMU_DIR)/$(QEMU_DISK) \
			-cdrom $(ISO_DIR)/$(ISO) \
			$(QEMU_FLAGS)
endif

# assemble assembly files, generic
$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(@D)
	$(AS) $< -o $@ $(ASFLAGS)

# compile C files, generic
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CCFLAGS)

# compile C++ files, generic
$(OBJ_DIR)/%.cpp.o: %.cpp
	mkdir -p $(@D)
	$(CPP) -c $< -o $@ $(CPPFLAGS)

# copy assembly files, generic
$(ASM_DIR)/%.s.s: %.s
	mkdir -p $(@D)
	cp $< $@

# assemble C files, generic
$(ASM_DIR)/%.c.s: %.c
	mkdir -p $(@D)
	$(CC) -S $< -o $@ $(CCFLAGS)

# assemble C++ files, generic
$(ASM_DIR)/%.cpp.s: %.cpp
	mkdir -p $(@D)
	$(CPP) -S $< -o $@ $(CPPFLAGS)

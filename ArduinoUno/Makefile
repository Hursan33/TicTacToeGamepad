# Project Configuration
PROJECT_NAME := my_nano_project
TARGET_DEVICE := atmega4809

# Toolchain (assuming in PATH)
CC := avr-gcc
OBJCOPY := avr-objcopy

# Output directories
dir_build := build
OUTPUT_ELF := $(dir_build)/$(PROJECT_NAME).elf
OUTPUT_HEX := $(dir_build)/$(PROJECT_NAME).hex

# AVRDUDE Configuration
PROGRAMMER_TYPE := pkobn_updi
PROGRAMMER_PORT := usb

# Source Files and Compilation Rules
SRC_DIR := src
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(dir_build)/%.o,$(SOURCES))

# Compiler and Linker Flags
CFLAGS := -g -O2 -mmcu=atmega328P -DF_CPU=20000000 -D__AVR_ATmega328P__ -ffunction-sections -fdata-sections
LDFLAGS := -O2 -mmcu=atmega328P -Wl,--gc-sections

.PHONY: all clean flash

# Default target
all: $(OUTPUT_HEX) flash

# Ensure build directory exists
$(dir_build):
	mkdir $(dir_build)

# Compile each source to build/<name>.o
$(dir_build)/%.o: $(SRC_DIR)/%.c | $(dir_build)
	$(CC) $(CFLAGS) -c $< -o $@

# Link ELF from objects
$(OUTPUT_ELF): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Create HEX from ELF
$(OUTPUT_HEX): $(OUTPUT_ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Clean build artifacts
clean:
	rmdir /s /q $(dir_build)

# Flash via on-board UPDI
flash: $(OUTPUT_HEX)
	avrdude -p "$(TARGET_DEVICE)" -c "$(PROGRAMMER_TYPE)" -P "$(PROGRAMMER_PORT)" -U flash:w:"$(OUTPUT_HEX)":i -v

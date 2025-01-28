# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Directories
SRCDIR = src
LIBDIR = libs
BUILDDIR = build

# Files
SRC = $(wildcard $(SRCDIR)/*.c)
LIBS = $(wildcard $(LIBDIR)/*.c)
DEPS = $(BUILDDIR)/mica.o $(patsubst $(LIBDIR)/%.c, $(BUILDDIR)/%.o, $(LIBS))
TARGET = $(BUILDDIR)/mica.exe

# Default target
all: $(TARGET)

# Rule to build the final executable
$(TARGET): $(DEPS)
	@ $(CC) $(CFLAGS) -o $@ $^

# Rule to compile .c files into .o files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	@ $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(LIBDIR)/%.c | $(BUILDDIR)
	@ $(CC) $(CFLAGS) -c $< -o $@

# Create the obj directory if it doesn't exist
$(BUILDDIR):
	@ if not exist $(BUILDDIR) mkdir $(BUILDDIR)

# Clean target to remove compiled files and the executable
clean:
	@ if exist $(BUILDDIR) rmdir /s /q $(BUILDDIR)

run : $(TARGET)
	@ $(TARGET)

# Phony targets
.PHONY: all clean run

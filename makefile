# Makefile for Pancracio Compiler
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
SRCDIR = src
OBJDIR = obj
LIBS = -lgdi32 -luser32 -lkernel32 -lshell32 -lcomctl32 -lole32

# Source files
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/interpreter.cpp $(SRCDIR)/command_executor.cpp \
          $(SRCDIR)/window_manager.cpp $(SRCDIR)/expressions.cpp $(SRCDIR)/utils.cpp \
          $(SRCDIR)/book_manager.cpp $(SRCDIR)/dependency_manager.cpp

# Object files
OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/interpreter.o $(OBJDIR)/command_executor.o \
          $(OBJDIR)/window_manager.o $(OBJDIR)/expressions.o $(OBJDIR)/utils.o \
          $(OBJDIR)/book_manager.o $(OBJDIR)/dependency_manager.o

# Target executable
TARGET = pan_core.exe

# Default target
all: $(TARGET)

# Create obj directory if it doesn't exist
$(OBJDIR):
	@if not exist $(OBJDIR) mkdir $(OBJDIR)

# Compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Compile installer if it exists
installer: pancracio.exe

pancracio.exe: $(SRCDIR)/installer.cpp
	@if exist $(SRCDIR)/installer.rc (windres -i $(SRCDIR)/installer.rc -o $(OBJDIR)/installer_res.o --input-format=rc --output-format=coff)
	@if exist $(OBJDIR)/installer_res.o ($(CXX) $(CXXFLAGS) -mwindows -o $@ $< $(OBJDIR)/installer_res.o -lcomctl32 -luxtheme -ldwmapi -lmsimg32 -lgdi32 -luser32 -lkernel32 -lole32 -lshell32 -luuid) else ($(CXX) $(CXXFLAGS) -mwindows -o $@ $< -lcomctl32 -luxtheme -ldwmapi -lmsimg32 -lgdi32 -luser32 -lkernel32 -lole32 -lshell32 -luuid)

# Clean build files
clean:
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	@if exist $(TARGET) del $(TARGET)
	@if exist pancracio.exe del pancracio.exe

# Test the built executable
test: $(TARGET)
	./$(TARGET) --version

# Install dependencies info
deps:
	@echo Required dependencies for compilation:
	@echo - MinGW-w64 with g++ compiler
	@echo - Windows SDK headers
	@echo - Make utility (optional, for using this Makefile)

.PHONY: all clean test deps installer
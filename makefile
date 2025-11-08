# Compiler and compiler flags
CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -fopenmp
LFLAGS   := -fopenmp -lnlopt -lm

# Directories
SRCDIR   := src
OBJDIR   := obj

# Sources and objects
SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS  := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Executable
EXECUTABLE := psd.out

# Build type: default = release
BUILD ?= release

# Adjust flags based on build type
ifeq ($(BUILD),debug)
  CXXFLAGS += -g -O0
else ifeq ($(BUILD),release)
  CXXFLAGS += -O3
else
  $(error Unknown build type '$(BUILD)'. Use BUILD=debug or BUILD=release)
endif

# Default target
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LFLAGS)

# Ensure obj directory exists before compiling
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(EXECUTABLE)

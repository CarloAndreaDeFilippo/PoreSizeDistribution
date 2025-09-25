
# Compiler and compiler flags
CC = g++ -std=c++11
CFLAGS = -c -g -Wall -Wextra -O3 -fopenmp
LFLAGS = -pthread -fopenmp -lnlopt -lm

# Directory containing the source files
SRCDIR = src

# Directory to place the object files
OBJDIR = obj

# List of source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

# List of object files to be created
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# The final executable file
EXECUTABLE = psd.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LFLAGS)

# Ensure obj directory exists before compiling any .o
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(EXECUTABLE)

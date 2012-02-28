# Compiler flags go here.
CFLAGS = -g -Wall -O2 -I . -MMD

# Linker flags go here.
LDFLAGS = -lm -lpthread -lcomedi -g

# list of sources
ELEVSRC = elev.c io.c main.c elevator_ctrl.c elevator_io.c elevator_sm.c elevator_ui.c elevator_timer.c

# program executable file name.
TARGET = heis

# top-level rule, to compile everything.
all: $(TARGET)

# Define dependencies
DEPS = $(shell find -name "*.d")

# Define all object files.
ELEVOBJ = $(ELEVSRC:.c=.o)

# rule to link the program
$(TARGET): $(ELEVOBJ)
	gcc $(LDFLAGS) $^ -o $@

# Compile: create object files from C source files.
%.o : %.c
	gcc $(CFLAGS) -c  $< -o $@ 

# Include dependencies, if any.
-include $(DEPS) foo

# rule for cleaning re-compilable files.
clean:
	rm -f $(TARGET) $(ELEVOBJ) $(DEPS)

rebuild:	clean all

.PHONY: all rebuild clean


#Martin Korsgaard, 2006

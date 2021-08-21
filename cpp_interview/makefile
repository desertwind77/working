INCDIR = include
SRCDIR = src
OBJDIR = obj
BINDIR = bin

CC = gcc
CPP = g++
CFLAGS = -I$(INCDIR)
LFLAGS = -lm

SOURCES := $(wildcard $(SRCDIR)/*.c)
INCLUDE := $(wildcard $(INCDIR)/*.h)
OBJECT  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET  := $(patsubst $(SRCDIR)/%.c, %, $(SOURCES))
DEPS = $(INCLUDE)

all: $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	mkdir -p $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

# There is a problem here. each binary depends on all objects.
$(TARGET): $(OBJECT) 
	mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$@ $(OBJDIR)/$@.o $(LFLAGS)

.PHONY: clean

clean:
	rm -rf core *~ $(OBJDIR) $(INCDIR)/*~ $(BINDIR)

SRCDIR   := src
BINDIR   := bin
INCLUDES := -Iinclude/
OBJDIR   := .build
TESTDIR  := $(BINDIR)/tests

CC       := g++
CFLAGS   :=-Wall -Wextra -g -fno-stack-protector -z execstack -pthread -std=gnu++11 $(INCLUDES) -m32
LFLAGS   := -lm
SRC      :=                      \
	$(wildcard src/services/authorization/*.cpp) \
	$(wildcard src/services/commands/*.cpp) \
	$(wildcard src/configuration/*.cpp) \
	$(wildcard src/network/*.cpp)      \
	$(wildcard src/user/*.cpp)				\
	$(wildcard src/grass.cpp)         \
	$(wildcard src/error.cpp)					\
	$(wildcard tests/*.cpp)

OBJECTS := $(SRC:%.cpp=$(OBJDIR)/%.o)

all: build $(BINDIR)/server $(BINDIR)/client $(TESTDIR)

tests: refresh $(TESTDIR)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(BINDIR)/server: $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LFLAGS) $(INCLUDES) -o $(BINDIR)/server $(SRCDIR)/server.cpp $(OBJECTS)

$(BINDIR)/client: $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LFLAGS) $(INCLUDES) -o $(BINDIR)/client $(SRCDIR)/client.cpp $(OBJECTS)

$(BINDIR)/tests: $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LFLAGS) $(INCLUDES) -o $(BINDIR)/tests $(SRCDIR)/tests.cpp $(OBJECTS)


.PHONY: all build clean debug release refresh tests

build:
	@mkdir -p $(BINDIR)
	@mkdir -p $(OBJDIR)

debug: CFLAGS += -DDEBUG -g
debug: all

release: CFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJDIR)/*
	-@rm -rvf $(BINDIR)/*

refresh: clean all

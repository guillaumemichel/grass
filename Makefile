SRCDIR   := src
BINDIR   := bin
INCLUDES := -Iinclude/
OBJDIR   := .build

CC       := g++
CFLAGS   :=-Wall -Wextra -g -fno-stack-protector -z execstack -pthread -std=gnu++11 $(INCLUDES) -m32
LFLAGS   := -lm
SRC      :=                      \
	$(wildcard src/services/authorization/*.cpp) \
	$(wildcard src/services/commands/*.cpp) \
	$(wildcard src/network/*.cpp)      \
	$(wildcard src/user/*.cpp)				\
	$(wildcard src/grass.cpp)         \
	$(wildcard src/error.cpp)					\

OBJECTS := $(SRC:%.cpp=$(OBJDIR)/%.o)

all: build $(BINDIR)/server $(BINDIR)/client

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(BINDIR)/server: $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LFLAGS) $(INCLUDES) -o $(BINDIR)/server $(SRCDIR)/server.cpp $(OBJECTS)

$(BINDIR)/client: $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LFLAGS) $(INCLUDES) -o $(BINDIR)/client $(SRCDIR)/client.cpp $(OBJECTS)


.PHONY: all build clean debug release

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

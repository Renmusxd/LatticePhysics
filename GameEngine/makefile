# Makefile for GameEngine library

TARGET = lib/libgameEngine.a
CC=g++

SRCDIR = src
OBJDIR = obj
LIBS = lib
IDIR = include

HEADERS = $(wildcard $(IDIR)/*.h)

SRCFILES	= $(wildcard $(SRCDIR)/*.cpp)
SRCS 		:= $(subst $(SRCDIR)/,,$(SRCFILES))
SRCS 		:= $(subst .cpp,,$(SRCS))
OBJECTS 	:= $(patsubst %, $(OBJDIR)/%.o, $(SRCS))
SDL_LIB     := -L$(ENGINE_LIB) -l$(ENGINE_NAME) -lSDL2 -lSDL2_ttf -lSDL2_mixer -framework OpenGL
INCPATH 	:= -I/usr/local/include -I$(IDIR) -I$(ENGINE_INC)
LDFLAGS     := $(SDL_LIB)OBJECTS 	= $(patsubst %, $(OBJDIR)/%.o, $(SRCS))
SDL_INCLUDE = /usr/local/include

CFLAGS =-c -Wall -std=c++11 -I$(SDL_INCLUDE) -I$(IDIR) -O3

.PHONY: clean all default

default: $(TARGET)
all: default

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJECTS)
	ar rcs $@ $^

clean:
	rm obj/* $(TARGET)
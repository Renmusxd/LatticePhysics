
TARGET = physics
CC=g++

SRCDIR = src
OBJDIR = obj
LIBS = lib
IDIR = include

ENGINE_NAME = gameEngine
ENGINE_PATH = GameEngine
ENGINE_LIB = $(ENGINE_PATH)/lib
ENGINE_INC = $(ENGINE_PATH)/include

HEADERS = $(wildcard $(IDIR)/*.h) $(wildcard $(ENGINE_INC)/*.h)

SRCFILES	= $(wildcard $(SRCDIR)/*.cpp)
SRCS 		:= $(subst $(SRCDIR)/,,$(SRCFILES))
SRCS 		:= $(subst .cpp,,$(SRCS))
OBJECTS 	:= $(patsubst %, $(OBJDIR)/%.o, $(SRCS))
SDL_LIB     := -L$(ENGINE_LIB) -l$(ENGINE_NAME) -lSDL2 -lSDL2_ttf -lSDL2_mixer -framework OpenGL
INCPATH 	:= -I/usr/local/include -I$(IDIR) -I$(ENGINE_INC)
LDFLAGS     := $(SDL_LIB)

CFLAGS :=-c -Wall -std=c++11 $(INCPATH) -O3

.PHONY: clean all default

default: $(TARGET)
all: default

debug: CFLAGS += -DDEBUG -g
debug: $(TARGET)


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJECTS)
	cd GameEngine && $(MAKE)
	cd ..
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm obj/* $(TARGET)

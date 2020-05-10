CC := gcc
CXX := g++
LINKER := g++
ASM := nasm
SRCDIR := ./Source
NET_SRCDIR := ./Source/Net
BINDIR := ./objs
NET_BINDIR := ./objs/Net
EXE := $(BINDIR)/blackshades #$(BINDIR)/bse_viewer

CFLAGS := -O2 -fstrict-aliasing -Wall -g $(shell sdl-config --cflags) -I$(SRCDIR) -I$(NET_SRCDIR) -include Source/Support.h
CXXFLAGS := $(CFLAGS)
LDFLAGS := $(shell sdl-config --libs) -lGL -lGLU -lopenal -lalut # -framework QuickTime

# Don't want ogg?
#CFLAGS += -DNOOGG
# Got ogg?
LDFLAGS += -lvorbisfile -lvorbis

SRCS := Button.cpp \
  Camera.cpp \
	Config.cpp \
	Decals.cpp \
	Fog.cpp \
	Font.cpp \
	Frustum.cpp \
	Game.cpp \
	GameDraw.cpp \
	GameInitDispose.cpp \
	GameLoop.cpp \
	GameTick.cpp \
	Globals.cpp \
	Image.cpp \
	Label.cpp \
	MacInput.cpp \
	Main.cpp \
	Maths.cpp \
	Models.cpp \
	Person.cpp \
	Quaternions.cpp \
	SDL_funcs.cpp \
	Serialize.cpp \
	Skeleton.cpp \
	Sprites.cpp \
	Support.cpp \
	TGALoader.cpp \
	Text.cpp \
	Weapon.cpp \
	Window.cpp

NET_SRCS := \
	Client.cpp \
	NetCommon.cpp \
	Server.cpp
#	Viewer.cpp \

OBJS1 := $(SRCS:.c=.o)
OBJS2 := $(OBJS1:.cpp=.o)
OBJS3 := $(OBJS2:.asm=.o)
NET_OBJS1 := $(NET_SRCS:.cpp=.o)
OBJS := $(foreach f,$(OBJS3),$(BINDIR)/$(f))
SRCS := $(foreach f,$(SRCS),$(SRCDIR)/$(f))
NET_OBJS := $(foreach f,$(NET_OBJS1),$(NET_BINDIR)/$(f))

CLEANUP = $(wildcard *.exe) $(wildcard *.obj) \
	  $(wildcard $(BINDIR)/*.exe) $(wildcard $(BINDIR)/*.obj) \
	  $(wildcard *~) $(wildcard *.err) \
	  $(wildcard .\#*) core $(EXE)

# Rules for compiling individual source files...

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(NET_BINDIR)/%.o: $(NET_SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

.PHONY: all bindir blackshades
all: $(EXE)

$(EXE): $(BINDIR) $(OBJS) $(NET_OBJS)
	$(LINKER) -o $(EXE) $(OBJS) $(NET_OBJS) $(LDFLAGS)

$(BINDIR) :
	$(MAKE) bindir

bindir:
	mkdir -p $(BINDIR)
	mkdir -p $(NET_BINDIR)

distclean: clean

clean:
	rm -f $(CLEANUP)
	rm -rf $(BINDIR)

# end of Makefile.



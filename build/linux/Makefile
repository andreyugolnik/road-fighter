EXE = roadfighter
SRC = src
PREFIX = /usr
EXTRADIR = ./build/linux

GAMEDIR = $(PREFIX)/games
STARTUP = $(GAMEDIR)/$(EXE)
BINDIR = $(PREFIX)/share/games/$(EXE)
ICNDIR = $(PREFIX)/share/pixmaps
APPDIR = $(PREFIX)/share/applications

DATA = fonts graphics maps sound readme.txt
ICON = roadfighter.png
DESKTOP = roadfighter.desktop

OBJS = \
	   $(SRC)/sge/sge_blib.o                  \
	   $(SRC)/sge/sge_bm_text.o               \
	   $(SRC)/sge/sge_collision.o             \
	   $(SRC)/sge/sge_misc.o                  \
	   $(SRC)/sge/sge_primitives.o            \
	   $(SRC)/sge/sge_rotation.o              \
	   $(SRC)/sge/sge_shape.o                 \
	   $(SRC)/sge/sge_surface.o               \
	   $(SRC)/sge/sge_textpp.o                \
	   $(SRC)/sge/sge_tt_text.o               \
	   $(SRC)/Objects/CarObject.o             \
	   $(SRC)/Objects/EnemyCarObject.o        \
	   $(SRC)/Objects/EnemyFastCarObject.o    \
	   $(SRC)/Objects/EnemyNormalCarObject.o  \
	   $(SRC)/Objects/EnemyRacerCarObject.o   \
	   $(SRC)/Objects/EnemySlidderCarObject.o \
	   $(SRC)/Objects/EnemyTruckObject.o      \
	   $(SRC)/Objects/ExplosionObject.o       \
	   $(SRC)/Objects/FuelObject.o            \
	   $(SRC)/Game.o                          \
	   $(SRC)/Objects/Object.o                \
	   $(SRC)/Objects/PlayerCarObject.o       \
	   $(SRC)/RoadFighter.o                   \
	   $(SRC)/Objects/SemaphoreObject.o       \
	   $(SRC)/Tile.o                          \
	   $(SRC)/Auxiliar.o                      \
	   $(SRC)/Configuration.o                 \
	   $(SRC)/Debug.o                         \
	   $(SRC)/FileHandling.o                  \
	   $(SRC)/States/GameoverState.o          \
	   $(SRC)/States/InterlevelState.o        \
	   $(SRC)/States/KonamiState.o            \
	   $(SRC)/LoadMG2.o                       \
	   $(SRC)/States/MenuState.o              \
	   $(SRC)/States/PlayingState.o           \
	   $(SRC)/States/PresentationState.o      \
	   $(SRC)/QuickTables.o                   \
	   $(SRC)/Sound.o                         \
	   $(SRC)/main.o

CC = g++
CFLAGS = -std=c++11 -g3 -O0 `sdl-config --cflags` -I/usr/X11R6/include
LDFLAGS = `sdl-config --libs` -L/usr/X11R6/lib/ -lSDL_image -lSDL_mixer -lSDL_sound -lSDL_ttf
RM = rm -f
CP = cp -r
MD = mkdir -p
ECHO = echo
CHMOD = chmod
STRIP = strip

all: $(EXE)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
	# @$(STRIP) $@
	@$(ECHO) " o If there are no errors, the game compiled succesfully"

clean:
	@$(ECHO) " o Cleaning up source directory"
	@$(RM) $(SRC)/*.o $(SRC)/sge/*.o $(SRC)/*.bak core $(EXE)

install: all
	@$(ECHO) " o Creating install directory $(BINDIR)"
	@$(MD) "$(BINDIR)"
	@$(ECHO) " o Installing game and data to $(BINDIR)"
	@$(CP) "$(EXE)" $(DATA) "$(BINDIR)"
	@$(ECHO) " o Creating startup script $(STARTUP)"
	@$(MD) "$(GAMEDIR)"
	@$(ECHO) "cd \"$(BINDIR)\"; ./$(EXE); cd -" >"$(STARTUP)"
	@$(CHMOD) 755 "$(STARTUP)"
	@$(ECHO) " o Creating application menu entry"
	@$(MD) "$(ICNDIR)"
	@$(CP) "$(EXTRADIR)/$(ICON)" "$(ICNDIR)"
	@$(MD) "$(APPDIR)"
	@$(CP) "$(EXTRADIR)/$(DESKTOP)" "$(APPDIR)"
	@$(ECHO) ""

uninstall:
	@$(ECHO) " o Removing game and data from $(BINDIR)"
	@$(RM) -r "$(BINDIR)"
	@$(ECHO) " o Removing startup script $(STARTUP)"
	@$(RM) "$(STARTUP)"
	@$(ECHO) " o Removing application menu entry"
	@$(RM) "$(ICNDIR)/$(ICON)" "$(APPDIR)/$(DESKTOP)"
	@$(ECHO) ""


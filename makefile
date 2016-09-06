SOURCES = main.c
EXEC = movingSquare
CC = gcc
PSX_CC = CCPSX.EXE
PSX_EXEC = movsq.exe
PSX_CPE2X = CPE2X.EXE
CFLAGS = `pkg-config --cflags glfw3` -c
LDFLAGS = `pkg-config --static --libs glfw3 glew`
WINE = WINEPREFIX=/opt/wine/wine32 wine
DOSEMU = echo 'D:\r cd "$(CURDIR:/%=%)/"\r $(PSX_CPE2X) $(CPE)\r exitemu\r' | dosemu -dumb
PSX_CFLAGS = -O3 -Dpsx -c
PSX_ADDRESS = 0x80010000
PSX_LDFLAGS =  -l libpad -Xo$(PSX_ADDRESS)
OBJECTS = $(SOURCES:.c=.o)
PSX_OBJECTS = $(SOURCES:.c=.obj)
CPE = $(PSX_EXEC:.exe=.cpe)
SYM = $(PSX_EXEC:.exe=.sym)
MAP = $(PSX_EXEC:.exe=.map)

all:
$(info ******** PICK FROM 3 DIFFERENT BUILDS ********)
$(info LINUX_BUILD    ... build using GLFW and GLEW)
$(info PSX_WINE_BUILD ... build using WINE and PSYQ library)
$(info PSX_BUILD      ... build using PSYQ library in windows)

LINUX_BUILD : $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 
	rm -rf $(OBJECTS)
	
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

PSX_WINE_BUILD: PSX_CC := $(WINE) $(PSX_CC)
PSX_WINE_BUILD: PSX_CPE2X := $(DOSEMU)
PSX_WINE_BUILD: PSX_BUILD
	
PSX_BUILD: $(SOURCES) $(PSX_EXEC)

$(PSX_EXEC): $(CPE)
	$(PSX_CPE2X)
	rm -rf $(PSX_OBJECTS) $(CPE) $(SYM) $(MAP)

$(CPE): $(PSX_OBJECTS)
	$(PSX_CC) $(PSX_OBJECTS) $(PSX_LDFLAGS) -o$(CPE),$(SYM),$(MAP)
	
%.obj: %.c
	$(PSX_CC) $< $(PSX_CFLAGS) -o $@

	
clean:
	rm -f $(EXEC) $(PSX_EXEC) $(CPE) $(SYM) $(MAP) $(OBJECTS) $(PSX_OBJECTS)
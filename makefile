CC = gcc
CFLAGS = `pkg-config --cflags glfw3` -c
LDFLAGS = `pkg-config --static --libs glfw3 glew`
PSX_CC = CCPSX.EXE
WINE = WINEPREFIX=/opt/wine/wine32 wine
DOSEMU = dosbox -c C: -c "cd home\jconve~1\Docume~1\Programs\PlaySt~1\CrossC~1" -c "CPE2X.EXE movsq.cpe" -c exit
PSX_CFLAGS = -O3 -Dpsx -c
PSX_ADDRESS = 0x80010000
PSX_LDFLAGS =  -l libpad -Xo$(PSX_ADDRESS)
PSX_CPE2X = CPE2X.EXE
SOURCES = main.c
OBJECTS = main.o
PSX_OBJECTS = main.obj
EXEC = movingSquare
PSX_EXEC = MOVSQ.EXE
CPE = movsq.cpe
SYM = movsq.sym
MAP = mem.map

all: PSX_BUILD LINUX_BUILD 

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
	$(PSX_CPE2X) $(CPE)
	rm -rf $(PSX_OBJECTS) $(CPE) $(SYM) $(MAP)

$(CPE): $(PSX_OBJECTS)
	$(PSX_CC) $(PSX_OBJECTS) $(PSX_LDFLAGS) -o$(CPE),$(SYM),$(MAP)
	
%.obj: %.c
	$(PSX_CC) $< $(PSX_CFLAGS) -o $@

	
clean:
	rm -f $(EXEC) $(PSX_EXEC) $(CPE) $(SYM) $(MAP) $(OBJECTS) $(PSX_OBJECTS)
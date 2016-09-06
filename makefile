CC = gcc
CFLAGS = `pkg-config --cflags glfw3`
LDFLAGS = `pkg-config --static --libs glfw3 glew`
PSX_CC = ccpsx
PSX_CFLAGS = -O3 -Dpsx
PSX_ADDRESS = 0x80010000
PSX_LDFLAGS =  -l libpad
PSX_CPE2X = cpe2x
SOURCES = main.c
OBJECTS = main.o
PSX_OBJECTS = main.obj
EXEC = movingSquare
PSX_EXEC = movsq.exe
CPE = movsq.cpe
SYM = movsq.sym
MAP = mem.map

all: PSX_BUILD LINUX_BUILD 

LINUX_BUILD : $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 
	


PSX_BUILD: $(SOURCES) $(PSX_EXEC)

$(PSX_EXEC): $(CPE)
	$(PSX_CPE2X) $(CPE)

$(CPE): $(PSX_OBJECTS)
	$(PSX_CC) $(PSX_OBJECTS) $(PSX_LDFLAGS) -Xo$(PSX_ADDRESS) -o$(CPE),$(SYM),$(MAP)
	
%.obj: %.c
	$(PSX_CC) $< $(PSX_CFLAGS) -c -o $@
	
clean:
	rm -f $(EXEC) $(PSX_EXEC) $(CPE) $(SYM) $(MAP) $(OBJECTS) $(PSX_OBJECTS)
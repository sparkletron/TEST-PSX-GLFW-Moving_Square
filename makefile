CC = gcc
CFLAGS = `pkg-config --cflags glfw3` -c
LDFLAGS = `pkg-config --static --libs glfw3 glew`
PSX_CC = ccpsx
PSX_CFLAGS = -O3 -Dpsx -c
PSX_ADDRESS = 0x80010000
PSX_LDFLAGS =  -l libpad -Xo$(PSX_ADDRESS)
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
	rm -rf $(OBJECTS)
	
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

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
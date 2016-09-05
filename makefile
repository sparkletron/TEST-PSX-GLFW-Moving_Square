CC = gcc
CFLAGS = `pkg-config --cflags glfw3`
LDFLAGS = `pkg-config --static --libs glfw3 glew`
PSX_CC = ccpsx
PSX_CFLAGS = -O3 -Dpsx
PSX_LDFLAGS = -l libpad -Xo$80010000
SOURCES = main.c
OBJECTS = $(SOURCES: .c=.o)
EXEC = movingSquare
PSX_EXEC = movingSquare.exe
CPE = movingSquare.cpe
SYM = movingSquare.sym
MAP = movingSquare.map

all: PSX_BUILD LINUX_BUILD 

LINUX_BUILD : $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 
	
%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

PSX_BUILD: $(SOURCES) $(PSX_EXEC)

$(PSX_EXEC): $(OBJECTS)
	$(PSX_CC) $(PSX_LDFLAGS) $< -o $@
	
clean:
	rm -f $(EXEC) $(PSX_EXEC) $(CPE) $(SYM) $(MAP)
## Test application that compiles on Linux and the PlayStation.

Moving squares is a simple application that has a square that moves from side to side, up and down. It will also change color when the correct button is pressed.

### Controls
* UP
 * LINUX: Keyboard UP
 * PSX: D-Pad up
* DOWN
 * LINUX: Keyboard DOWN
 * PSX: D-Pad down
* LEFT
 * LINUX: Keyboard LEFT
 * PSX: D-Pad left
* RIGHT
 * LINUX: Keyboard RIGHT
 * PSX: D-Pad right
* COLOR CHANGE
 * LINUX: Keyboard x
 * PSX: x(cross) button

### Requirements

PlayStation (WINDOWS XP SP3)

	Psy-Q SDK from psxdev.net
	MinGW
	
PlayStation (KUBUNTU 14.04)

	Psy-Q SDK from psxdev.net
	WINE 32bit prefix setup
	dosemu
	
Linux (KUBUNTU 14.04)

	GLEW (repository)
	GLFW v3.2.1 (download and compile from source)

### Library Install Directions

	COMING SOON
	
### Make Commands

	make 		... builds all

	make clean	... removes all compiled files

	make PSX_BUILD 	... builds for the PlayStation only

	make LINUX_BUILD... builds for Linux only.

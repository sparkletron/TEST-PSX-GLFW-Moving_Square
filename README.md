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
	WINE
	dosemu
	BUILD_ESSENTIALS
	
Linux (KUBUNTU 14.04)

	GLEW (repository)
	GLFW v3.2.1 (download and compile from source)
	BUILD_ESSENTIALS

### Library Install Directions

#### DOWNLOAD LINKS
   1. [PlayStation Dev Kit ... psxdev.net](http://www.psxdev.net/downloads.html)

   2. [Windows Reg File for PlayStation Dev Kit ... psxdev.net/forum](http://www.psxdev.net/forum/viewtopic.php?f=49&t=206)

   3. [GLFW library ... glfw.org](http://www.glfw.org/download.html)

Linux using WINE

        1. Install WINE
        2. Install dosemu 
        3. Initialize WINE (winecfg works well for this)
        4. Initialize dosemu (just run dosemu)
        5. Copy psyq directory to wine drive_c directory
        6. Use the command "wine regedit" to import reg file
        7. Edit dosemu's autoexec.bat and alter the file to mount your linux drive as root for drive D:.
           Then add psyq folder in wine as a path.
        8. Install pcsx
        9. Execute pcsx
        10. Add a bios file to pcsx
        11. Tell pcsx to use the file
        
Linux using GLFW

        1. Download, Compile, and install GLFW v3.2.1 
           (requires cmake, and will inform you of missing dependcies)
        2. Install GLEW (repository version in Ubuntu 14.04 is fine)
	
### Make Commands

	make                ... builds nothing, displays info

	make clean          ... removes all compiled files

	make PSX_BUILD      ... builds for the PlayStation only
	
	make PSX_WINE_BUILD ... builds for the PlayStation using WINE 

	make LINUX_BUILD    ... builds for Linux only.

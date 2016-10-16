## Test application that compiles on Linux and the PlayStation.

Moving squares is a simple application that has a square that moves from side to side, up and down. It will also change color when the correct button is pressed.

### Controls
* UP
 * PC: Keyboard UP
 * PSX: D-Pad up
* DOWN
 * PC: Keyboard DOWN
 * PSX: D-Pad down
* LEFT
 * PC: Keyboard LEFT
 * PSX: D-Pad left
* RIGHT
 * PC: Keyboard RIGHT
 * PSX: D-Pad right
* COLOR CHANGE
 * PC: Keyboard x
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
	
Windows (10 PRO)

	GLEW
	GLFW v3.2.1 (download prebuilt binary)
	cmake for windows
	Visual Studio

### Library Install Directions

#### DOWNLOAD LINKS
   1. [PlayStation Dev Kit ... psxdev.net](http://www.psxdev.net/downloads.html)

   2. [Windows Reg File for PlayStation Dev Kit ... psxdev.net/forum](http://www.psxdev.net/forum/viewtopic.php?f=49&t=206)

   3. [GLFW library ... glfw.org](http://www.glfw.org/download.html)
   
   4. [GLEW library ... glew.sourceforge.net](http://glew.sourceforge.net/)
   
   5. [CMAKE ... cmake.org](https://cmake.org/download/)
   
##### FIRST

	Download this repo.
	
###### Windows using Psy-Q

  1. [Visit this link ... psxdev.net](http://www.psxdev.net/help/psyq_install.html)

###### Linux using WINE with Psy-Q

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
	10. Add a bios file to pcsx (not needed HLE bios will work fine)
	11. Tell pcsx to use the file
	12. Leave pcsx open
	12. Run cmake . -DCMAKE_TOOLCHAIN_FILE=psx_build/toolchain/psx.cmake
	13. Run make to compile
	14. Executable will be located in bin_psx
	15. Use pcsx "run exe" to navigate to the bin_psx folder
	16. Open the executable located in that folder.
        
###### Linux using GLFW

	1. Download, Compile, and install GLFW v3.2.1 
	    (requires cmake, and will inform you of missing dependcies)
	2. Install GLEW (repository version in Ubuntu 14.04 is fine)
	3. Run cmake . to create makefile
	4. Run make, to compile
	5. Executable will be located in bin_nix
	
###### Windows using GLFW

	1. Copy the contents of the GLFW download to C:\libraries\GLFW.
	   * You will have to create the C:\libraries\GLFW folder.
	   * Make sure you are in the GLFW download where it contains include and such.
	2. Copy the contents of the GLEW download to C:\libraries\GLEW
	   * You will have to create the C:\libraries\GLEW folder.
	   * Make sure you are in the GLEW download where it contains include and such.
	3. Install cmake for windows, follow its directions.
	4. Use cmake for windows to configure the project
	5. Using cmake for windows again, generate project files (for microsoft visual studio).
	6. Open the project in microsoft visual studio and build.
	   * Visual Studio can not find the executable at this time, not sure why, didn't look into it.
	   * Go to the repo folder, and look for bin_win
	   * Enter the folder and double click movsqr.exe
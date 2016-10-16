/*
 * PROJECT-GLFW-PSX-MOVING_SQUARE
 *
 *  Created on: Sep 5, 2016
 *      Author: John Convertino
 *		email: electrobs@gmail.com
 *		
 *		Linux and PlayStation cross platform test application.
 *		This application is just a square that can move either via
 *		keyboard in linux (UP, DOWN, LEFT, RIGHT, x for color change)
 *		or via how ever the pcsx emulator is mapped for d-pad directions
 *		and the x(cross) button (color change).
 *
 *		Version: 0.1
 *		Sep 5, 2016 0.1	First version
 *
 *		TODO
 */


#include <stdlib.h>

//includes based on platform
#ifdef psx
  #include <libgte.h>
  #include <libgpu.h>
  #include <libgs.h>
  #include <libpad.h>
  #include <libetc.h>
#else
  #include <unistd.h>
  #include <stdio.h>
  #include <GL/glew.h>
  #include <GLFW/glfw3.h>
#endif

//size of the square on screen, will always be 50 pixels of the total screen size
#define SQUARE_SIZE	50
//screen defines based on platform
#ifdef psx
  #define SCREEN_HEIGHT	240
  #define SCREEN_WIDTH	320
#else
  #define SCREEN_HEIGHT	480
  #define SCREEN_WIDTH	640
  #define WINDOW_NAME	"MOVING SQUARE"
#endif

//define polyF4 for openGL, this is the struct the psx uses
struct s_polyF4 {
  unsigned long *tag;
  unsigned char r0, g0, b0;
  unsigned char code;
  short x0,y0;
  short x1,y1;
  short x2,y2;
  short x3,y3;
};
//global game pad buffer, this is updated by a callback buried in the psyq library
#ifdef psx
  static u_char g_gamePad[2][34];
#endif

//to help translate input into a common type
enum inputTypes {UP, DOWN, LEFT, RIGHT, CHANGE_COLOR, NONE};
enum inputTypes g_decodedInput = NONE;


//prototypes
//initializes graphics
void *initGraphics();
//initialize input
void initInput(void *window);
//get translated input
enum inputTypes getInput();
//move the primitive passed based on input from keyboard or controller
void movePrimitive(struct s_polyF4 *f4, enum inputTypes decodedInput);
//generate primite, a square in this case
struct s_polyF4 genPrimitive();
//update display window with primitive
void display(void *window, struct s_polyF4 *f4);
//check if the display is still live (psx is infinite)
int displayLive(void *window);
//cleanup for glfw, does nothing for psx
void cleanup(void *window);



#ifdef psx
  //create a callback based on the vsync to translate input
  static void inputCallback();
#else
  //define a DramPrim for linux using openGL (already defined for psx)
  void DrawPrim(struct s_polyF4 *f4);
  //callback to decode input
  static void inputCallback(GLFWwindow *window, int key, int scanCode, int action, int mods);
void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

#endif

//application, takes no arguments at this point
int main(int argc, char *argv[])
{
  struct s_polyF4 polyF4;

  void *window = initGraphics();

  initInput(window);

  polyF4 = genPrimitive();

  while(displayLive(window))
  {
    movePrimitive(&polyF4, getInput());
    display(window, &polyF4);
  }

  cleanup(window);

  return 0;
}

//initialize graphics for either psx or linux
void *initGraphics()
{
  #ifdef psx
    //playstation init
    //check memory location to see if the unit is PAL or NTSC
    switch(*(char *)0xbfc7ff52)
    {
      case 'E':
	SetVideoMode(MODE_PAL);
	break;
      default:
	SetVideoMode(MODE_NTSC);
	break;
    }

    //initialize playstation graphics hardware
    GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER | GsOFSGPU, 1, 0);
    //setup display buffer
    GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);
  #else
    //glfw init 
    int returnValue = 0;
    GLenum glewReturnValue = 0;
    GLFWwindow *window = NULL;

    glfwSetErrorCallback(error_callback);


    returnValue = glfwInit();

    if(returnValue == GL_FALSE)
    {
      fprintf(stderr, "Failed to initialize GLFW.\n");
      exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME, NULL, NULL);

    if(window == NULL)
    {
      fprintf(stderr, "Failed to create window\n");
      glfwTerminate();
      exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    //vsync?
    glfwSwapInterval(1);

    glewReturnValue = glewInit();

    if(glewReturnValue != GLEW_OK)
    {
      fprintf(stderr, "Failed to initialize GLEW... %s\n", glewGetErrorString(glewReturnValue));
      glfwTerminate();
      exit(EXIT_FAILURE);
    }

    //setup area using openGL calls
    //set background color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //change drawing area to one that matches the playstation
    glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, -1.0f);
    //used to time the color change button press (set to allow initial press to go regardless of time)
    glfwSetTime(1.0f);

    return window;
  #endif

  return NULL;
}

//setup input based on arch
void initInput(void *window)
{
  #ifdef psx
    //playstation
    //initialize gamePad buffers which will hold the result of the callback function internal to the library.
    PadInitDirect(g_gamePad[0], g_gamePad[1]);
    //start communication
    PadStartCom();
    //set are own callback to update are control input buffer based on the playstations control buffer
    VSyncCallback(inputCallback);
  #else
    //glfw
    if(window != NULL)
    {
      glfwSetKeyCallback((GLFWwindow *)window, inputCallback);
    }
  #endif
}

//get the input and reset it to NONE
enum inputTypes getInput()
{
  enum inputTypes temp = g_decodedInput;
  g_decodedInput = NONE;
  return temp;
}

//universal method of moving the primitive on screen (at this point does a warp to the other side if you touch it)
void movePrimitive(struct s_polyF4 *f4, enum inputTypes decodedInput)
{
  #ifdef psx
  static int previousTime = -60;
  #endif
  switch(decodedInput)
  {
    case UP:
      f4->y0--;
      f4->y1--;
      f4->y2--;
      f4->y3--;
      if(f4->y0 < 0)
      {
	f4->y0 = SCREEN_HEIGHT - SQUARE_SIZE;
	f4->y1 = SCREEN_HEIGHT - SQUARE_SIZE;
	f4->y2 = SCREEN_HEIGHT;
	f4->y3 = SCREEN_HEIGHT;
      }
      break;
    case DOWN:
      f4->y0++;
      f4->y1++;
      f4->y2++;
      f4->y3++;
      if(f4->y2 > SCREEN_HEIGHT)
      {
	f4->y0 = 0;
	f4->y1 = 0;
	f4->y2 = SQUARE_SIZE;
	f4->y3 = SQUARE_SIZE;
      }
      break;
    case LEFT:
      f4->x0--;
      f4->x1--;
      f4->x2--;
      f4->x3--;
      if(f4->x0 < 0)
      {
	f4->x0 = SCREEN_WIDTH - SQUARE_SIZE;
	f4->x1 = SCREEN_WIDTH;
	f4->x2 = SCREEN_WIDTH - SQUARE_SIZE;
	f4->x3 = SCREEN_WIDTH;
      }
      break;
    case RIGHT:
      f4->x0++;
      f4->x1++;
      f4->x2++;
      f4->x3++;
      if(f4->x1 > SCREEN_WIDTH)
      {
	f4->x0 = 0;
	f4->x1 = SQUARE_SIZE;
	f4->x2 = 0;
	f4->x3 = SQUARE_SIZE;
      }
      break;
    case CHANGE_COLOR:
    #ifdef psx
      //if time passed is greater than 60 vsync counts(frame rate of 1/60 means this is 1 second)
      if((VSync(-1) - previousTime) > 60)
      {
	previousTime = VSync(-1);
    #else
      //check if it has been greater than 1 second since glfw time has been reset
      if(glfwGetTime() > 1.0f)
      {
	glfwSetTime(0.0f);
    #endif
	f4->r0 = rand()%256;
	f4->g0 = rand()%256;
	f4->b0 = rand()%256;
      }
      break;
    default:
      break;
  }
}

//generate primitive
struct s_polyF4 genPrimitive()
{
  struct s_polyF4 temp;

  #ifdef psx
    SetPolyF4((POLY_F4 *)&temp);
  #endif

  temp.r0 = 0;
  temp.g0 = 0;
  temp.b0 = 255;

  temp.x0 = 0;
  temp.y0 = 0;
  temp.x1 = SQUARE_SIZE;
  temp.y1 = 0;
  temp.x2 = 0;
  temp.y2 = SQUARE_SIZE;
  temp.x3 = SQUARE_SIZE;
  temp.y3 = SQUARE_SIZE;

  return temp;

}

//display primitive, window is only used for glfw, pass NULL for psx
void display(void *window, struct s_polyF4 *f4)
{

  #ifdef psx
    //playstation
    //clear drawing area
    GsClearDispArea(0, 0, 0);
    //draw primitive to the drawing area (blocks waiting in queue, non-blocking once its up to draw)
    DrawPrim(f4);
    //wait till all drawing is done
    DrawSync(0);
    //wait for vsync (1/60)
    VSync(0);
    //swap display and draw buffer
    GsSwapDispBuff();
  #else
    //GLFW
    //draw primitive function based on openGL calls
    DrawPrim(f4);
    if(window != NULL)
    {
      //swap draw and display buffer (back and front buffer)
      glfwSwapBuffers((GLFWwindow *)window);
      //poll events in the winow system
      glfwPollEvents();
    }
  #endif
}

//only needed for glfw, does nothing for the psx
void cleanup(void *window)
{
  #ifndef psx
    if(window != NULL)
    {
      glfwDestroyWindow((GLFWwindow *)window);
    }
  #endif
}

//check if the window is still open, since the PSX does not have a window, it returns a 1 to run infinitely
int displayLive(void *window)
{
  #ifdef psx
    return 1;
  #else
    return (glfwWindowShouldClose((GLFWwindow *)window) == 0 ? 1 : 0);
  #endif
}

#ifdef psx
  //decode gamePad buffer on vsync callback
  static void inputCallback()
  {
    switch((g_gamePad[0][2] << 8) | g_gamePad[0][3])
    {
      case 0xFFBF:
	//x press
	g_decodedInput = CHANGE_COLOR;
	break;
      case 0xEFFF:
	//up
	g_decodedInput = UP;
	break;
      case 0xDFFF:
	//right
	g_decodedInput = RIGHT;
	break;
      case 0xBFFF:
	//down
	g_decodedInput = DOWN;
	break;
      case 0x7FFF:
	//left
	g_decodedInput = LEFT;
	break;
      default:
	g_decodedInput = NONE;
	break;
    }
  }
#else
  //openGL DrawPrim function, mimics what the PSX version does (theirs is native aka, build in)
  void DrawPrim(struct s_polyF4 *f4)
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(f4->r0, f4->g0, f4->b0);
    glBegin(GL_QUADS);
    glVertex2i(f4->x0, f4->y0);
    glVertex2i(f4->x1, f4->y1);
    glVertex2i(f4->x3, f4->y3);
    glVertex2i(f4->x2, f4->y2);
    glEnd();
    glFlush();
  }

  //keyboard input callback to decode key pressed into are decoded type.
  static void inputCallback(GLFWwindow *window, int key, int scanCode, int action, int mods)
  {
    if(action == GLFW_PRESS || action == GLFW_REPEAT)
    {
      switch(key)
      {
	case GLFW_KEY_UP:
	  g_decodedInput = UP;
	  break;
	case GLFW_KEY_DOWN:
	  g_decodedInput = DOWN;
	  break;
	case GLFW_KEY_LEFT:
	  g_decodedInput = LEFT;
	  break;
	case GLFW_KEY_RIGHT:
	  g_decodedInput = RIGHT;
	  break;
	case GLFW_KEY_X:
	  g_decodedInput = CHANGE_COLOR;
	  break;
	default:
	  g_decodedInput = NONE;
	  break;
      }
    }
  }
#endif

/*
 * PROJECT-GLFW-PSX-MOVING_SQUARE
 *
 *  Created on: Sep 5, 2016
 *      Author: John Convertino
 *		email: electrobs@gmail.com
 *		
 *		DESCRIPTION
 *
 *		Version: ADD_HERE
 *		Sep 5, 2016 *VERSION*	*NOTES*
 *
 *		TODO
 */


#include <stdlib.h>

//includes passed on platform
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

#define SQUARE_SIZE		50
#define PROCESS_FAIL	-1
#define PROCESS_SUCCESS	 0
//screen defines based on platform
#ifdef psx
	#define SCREEN_HEIGHT	240
	#define SCREEN_WIDTH	320
#else
	#define SCREEN_HEIGHT	480
	#define SCREEN_WIDTH	640
	#define WINDOW_NAME		"MOVING SQUARE"
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
#ifdef psx
	static u_char g_gamePad[2][34];
#endif

enum inputTypes {UP, DOWN, LEFT, RIGHT, CHANGE_COLOR, NONE};
enum inputTypes g_decodedInput = NONE;

//prototypes
//initializes graphics
void *initGraphics();
//initialize input
void initInput(void *window);
//get input
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
//define a DramPrim for linux using openGL (already defined for psx)
#ifdef psx
	static void inputCallback();
#else
	void DrawPrim(struct s_polyF4 *f4);
	static void inputCallback(GLFWwindow *window, int key, int scanCode, int action, int mods);
#endif

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

void *initGraphics()
{
	#ifdef psx
		//playstation init
		switch(*(char *)0xbfc7ff52)
		{
			case 'E':
				SetVideoMode(MODE_PAL);
				break;
			default:
				SetVideoMode(MODE_NTSC);
				break;
		}

		GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER | GsOFSGPU, 1, 0);
		GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);
	#else
		//glfw init
		int returnValue = 0;
		GLenum glewReturnValue = 0;
		GLFWwindow *window = NULL;

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
		glfwSwapInterval(1);

		glewReturnValue = glewInit();

		if(glewReturnValue != GLEW_OK)
		{
			fprintf(stderr, "Failed to initialize GLEW... %s\n", glewGetErrorString(glewReturnValue));
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, -1.0f);
		glfwSetTime(1.0f);

		return window;
	#endif

	return NULL;
}

void initInput(void *window)
{
	#ifdef psx
		//playstation
		PadInitDirect(g_gamePad[0], g_gamePad[1]);
		PadStartCom();
		VSyncCallback(inputCallback);
	#else
		//glfw
		if(window != NULL)
		{
			glfwSetKeyCallback((GLFWwindow *)window, inputCallback);
		}
	#endif
}

enum inputTypes getInput()
{
	enum inputTypes temp = g_decodedInput;
	g_decodedInput = NONE;
	return temp;
}

void movePrimitive(struct s_polyF4 *f4, enum inputTypes decodedInput)
{
	#ifdef psx
		static int previousTime = VSync(1) + 60;
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
			if((VSync(-1) - previousTime) > 60)
			{
				previousTime = VSync(-1);
		#else
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
void display(void *window, struct s_polyF4 *f4)
{

	#ifdef psx
		//playstation
		GsClearDispArea(0, 0, 0);
		DrawPrim(f4);
		DrawSync(0);
		VSync(0);
		GsSwapDispBuff();
	#else
		//GLFW
		DrawPrim(f4);
		if(window != NULL)
		{
			glfwSwapBuffers((GLFWwindow *)window);
			glfwPollEvents();
		}
	#endif
}

void cleanup(void *window)
{
	#ifndef psx
		if(window != NULL)
		{
			glfwDestroyWindow((GLFWwindow *)window);
		}
	#endif
}

int displayLive(void *window)
{
	#ifdef psx
		return 1;
	#else
		return (glfwWindowShouldClose((GLFWwindow *)window) == 0 ? 1 : 0);
	#endif
}

#ifdef psx
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

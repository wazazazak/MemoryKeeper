#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include "main.h"

#include <iterator>
#include <functional>
#include <utility>
#include <time.h>
#include <iostream>


#define MEMSIZE 1024 * 10

static bool mouseLeftDown = false;
static HANDLE handle;
static int* points;

void init(void)
{
	void* startMem;
	handle = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MEMSIZE, "TestSharedMemory");
	startMem = (void *)::MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, MEMSIZE);
	points = (int*) startMem;
	points[MEMSIZE / sizeof(int) - 1] = 0;
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	{
		for(unsigned i(0); i < points[MEMSIZE / sizeof(int) - 1]; ++i)
		{
			glVertex2f(points[i * 4 + 0], points[i * 4 + 1]);
			glVertex2f(points[i * 4 + 2], points[i * 4 + 3]);
		}
	}
	glEnd();

	glFlush();
}

void SetupRC(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ChangeSize(int w, int h)
{
	GLfloat aspectRatio;
	if(h == 0) h =1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		gluOrtho2D(0, 500, 0 , 500*(float)h/(float)w);
	else
		gluOrtho2D(0, 500*(float)w/(float)h, 0, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouseButton(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			double viewport[4];
			glGetDoublev(GL_VIEWPORT, viewport);
			points[points[MEMSIZE / sizeof(int) - 1] * 4 + 0] = x / (float)viewport[2] * 500;
			points[points[MEMSIZE / sizeof(int) - 1] * 4 + 1] = (viewport[3] - y) / (float)viewport[3] * 500;
			points[points[MEMSIZE / sizeof(int) - 1] * 4 + 2] = points[points[MEMSIZE / sizeof(int) - 1] * 4];
			points[points[MEMSIZE / sizeof(int) - 1] * 4 + 3] = points[points[MEMSIZE / sizeof(int) - 1] * 4 + 1];
			++points[MEMSIZE / sizeof(int) - 1];
			mouseLeftDown = true;
		}
		else if(state == GLUT_UP)
		{
			if(mouseLeftDown) mouseLeftDown = false;
		}
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{

		}
		else if(state == GLUT_UP)
		{

		}
	}
	glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
	static int count = 0;
	if(mouseLeftDown)
	{
		double viewport[4];
		glGetDoublev(GL_VIEWPORT, viewport);
		if(count % 2 == 0)
		{
			points[points[MEMSIZE / sizeof(int) - 1] * 4] = points[(points[MEMSIZE / sizeof(int) - 1] - 1) * 4 + 2];
			points[points[MEMSIZE / sizeof(int) - 1] * 4 + 1] = points[(points[MEMSIZE / sizeof(int) - 1] - 1) * 4 + 3];
			++points[MEMSIZE / sizeof(int) - 1];
		}  

		points[(points[MEMSIZE / sizeof(int) - 1] - 1) * 4 + 2] = x / (float)viewport[2] * 500;
		points[(points[MEMSIZE / sizeof(int) - 1] - 1) * 4 + 3] = (viewport[3] - y) / (float)viewport[3] * 500;
		count++;
	}
	glutPostRedisplay();
}


void rmenu(int item)
{
	if(item == 1)
	{
		FILE *fp;
		fopen_s(&fp, "../../database", "r");
		char sdb[MEMSIZE];
		fread(sdb, 1, MEMSIZE, fp);
		fclose(fp);
		memcpy(points, sdb, MEMSIZE);
		glutPostRedisplay();
	}
}

void main(int argc, char* argv[])
{
	init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("PaintSample");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);

	glutCreateMenu(rmenu);
	glutAddMenuEntry("열기", 1);
	glutAddMenuEntry("자유선", 3);
	glutAddMenuEntry("원", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	SetupRC();
	glutMainLoop();
}

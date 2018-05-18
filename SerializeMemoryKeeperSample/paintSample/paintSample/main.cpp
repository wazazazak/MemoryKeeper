#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include "main.h"
#include "db.h"

#include <iterator>
#include <functional>
#include <utility>
#include <time.h>
#include <iostream>
#include <string>
#include "line.h"
#include "entity.h"
#include <thread>
#include "circle.h"

//using namespace boost::interprocess; 


#define MEMSIZE (10000 * 4 + 1) * 4

static bool mouseLeftDown = false;
static DB* mainDB;
static HANDLE handle;
char* startMem;
bool isDbWriting = false;
int curType = Entity::TYPE::kLIne;
std::thread t1;

void init(void)
{
	handle = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MEMSIZE, "TestSharedMemory");
	startMem = (char *)::MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, MEMSIZE);
	mainDB = new DB(startMem);
	t1 = std::thread(AutoSaver);
	t1.detach();
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 0, 0);
	for(unsigned i(0); i < mainDB->numEnts(); ++i)
	{
		Entity* ent = NULL;
		mainDB->getEntityAt(i, ent);
		ent->draw();
	}
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
	while(isDbWriting){}
	isDbWriting = true;
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			double viewport[4];
			glGetDoublev(GL_VIEWPORT, viewport);
			int ptX = x / (float)viewport[2] * 500;
			int ptY = (viewport[3] - y) / (float)viewport[3] * 500;
			if(curType == Entity::TYPE::kLIne)
			{
				Line* tempLine = new Line();
				tempLine->setStartPoint(ptX, ptY);
				tempLine->setEndPoint(ptX, ptY);
				mainDB->appendEntity(tempLine);
			}
			else if(curType == Entity::TYPE::kCircle)
			{
				Circle* pCircle = new Circle();
				pCircle->setCenterPoint(ptX, ptY);
				mainDB->appendEntity(pCircle);
			}
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
	isDbWriting = false;
	glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
	if(mouseLeftDown)
	{
		while(isDbWriting){}
		isDbWriting = true;
		double viewport[4];
		glGetDoublev(GL_VIEWPORT, viewport);
		int ptX = x / (float)viewport[2] * 500;
		int ptY = (viewport[3] - y) / (float)viewport[3] * 500;
		if(curType == Entity::TYPE::kLIne)
		{
			Entity* pEnt = NULL;
			static int count = 0;
			mainDB->getEntityAt(mainDB->numEnts() - 1, pEnt);
			if(pEnt->type() != Entity::TYPE::kLIne) return;
			Line* pLine = (Line*)pEnt;
			if(count % 2 == 0)
			{
				Line* newLine = new Line();
				int endX, endY;
				pLine->getEndPoint(endX, endY);
				newLine->setStartPoint(endX, endY);
				mainDB->appendEntity(newLine);
				pLine = newLine;
			}  
			pLine->setEndPoint(ptX, ptY);
			count++;
		}
		else if(curType == Entity::TYPE::kCircle)
		{
			Entity* pEnt = NULL;
			mainDB->getEntityAt(mainDB->numEnts() - 1, pEnt);
			if(pEnt->type() != Entity::TYPE::kCircle) return;
			Circle* pCircle = (Circle*)pEnt;
			int centerX, centerY;
			pCircle->getCenterPoint(centerX, centerY);
			int radius = sqrt((centerX - ptX) * (centerX - ptX) + (centerY - ptY) * (centerY - ptY));
			pCircle->setRadius(radius);
		}
		isDbWriting = false;
	}
	glutPostRedisplay();
}

void menu(int item)
{
	if(item == 1)
	{
		FILE *fp;
		fopen_s(&fp, "../../database", "r");
		char sdb[MEMSIZE];
		fread(sdb, 1, MEMSIZE, fp);
		fclose(fp);
		mainDB->read(sdb);
		glutPostRedisplay();
	}
	else if(item == 3)
	{
		curType = Entity::TYPE::kLIne;
	}
	else if(item == 4)
	{
		curType = Entity::TYPE::kCircle;
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

	glutCreateMenu(menu);
	glutAddMenuEntry("열기", 1);
	glutAddMenuEntry("자유선", 3);
	glutAddMenuEntry("원", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	SetupRC();
	glutMainLoop();
}

void AutoSaver()
{
	while(true)
	{
		Sleep(1000);
		if(!isDbWriting)
		{
			isDbWriting = true;
			mainDB->refreshShaerdMemory();
			isDbWriting = false;
		}
	}
}

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "line.h"

void Line::setStartPoint(int stX, int stY)
{
	this->stX = stX;
	this->stY = stY;
}

void Line::getStartPoint(int& stX, int& stY)
{
	stX = this->stX;
	stY = this->stY;
}

void Line::setEndPoint(int endX, int endY)
{
	this->endX = endX;
	this->endY = endY;
}

void Line::getEndPoint(int& endX, int& endY)
{
	endX = this->endX;
	endY = this->endY;
}

void Line::draw()
{
	glBegin(GL_LINES);
	glVertex2f(stX, stY);
	glVertex2f(endX, endY);
	glEnd();
}

int Line::type()
{
	return Entity::TYPE::kLIne;
}

void Line::write(char *s, int& index)
{
	int linetype = Entity::TYPE::kLIne;
	memcpy(s + index, (void*)&linetype, sizeof(linetype));

	index += sizeof(linetype);
	memcpy(s + index, (void*)&stX, sizeof(stX));
	index += sizeof(stX);
	memcpy(s + index, (void*)&stY, sizeof(stY));
	index += sizeof(stY);
	memcpy(s + index, (void*)&endX, sizeof(endX));
	index += sizeof(endX);
	memcpy(s + index, (void*)&endY, sizeof(endY));
	index += sizeof(endY);

	int none = Entity::TYPE::kNone;
	memcpy(s + index, (void*)&none, sizeof(none));
}

void Line::read(char *s, int& index)
{
	memcpy(&stX, s + index, sizeof(stX));
	index += sizeof(stX);
	memcpy(&stY, s + index, sizeof(stY));
	index += sizeof(stY);
	memcpy(&endX, s + index, sizeof(endX));
	index += sizeof(endX);
	memcpy(&endY, s + index, sizeof(endY));
	index += sizeof(endY);
}


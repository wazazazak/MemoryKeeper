#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cmath>
#include "circle.h"

void Circle::setCenterPoint(int centerX, int centerY)
{
	this->centerX = centerX;
	this->centerY = centerY;
}

void Circle::getCenterPoint(int& centerX, int& centerY)
{
	centerX = this->centerX;
	centerY = this->centerY;
}

void Circle::setRadius(int radius)
{
	this->radius = radius;
}

void Circle::getRadius(int& radius)
{
	radius = this->radius;
}

void Circle::draw()
{
	glBegin(GL_LINES);
	for(int i=0;i<1000;++i)
	{
		glVertex2f(centerX + radius * cos(2 * 3.14159*i/1000.0), centerY + radius * sin(2 * 3.14159*i/1000.0));
		glVertex2f(centerX + radius * cos(2 * 3.14159*(i + 1)/1000.0), centerY + radius * sin(2 * 3.14159*(i + 1)/1000.0));
	}
	glEnd();
}

int Circle::type()
{
	return Entity::TYPE::kCircle;
}

void Circle::write(char *s, int& index)
{
	int linetype = Entity::TYPE::kCircle;
	memcpy(s + index, (void*)&linetype, sizeof(linetype));

	index += sizeof(linetype);
	memcpy(s + index, (void*)&centerX, sizeof(centerX));
	index += sizeof(centerX);
	memcpy(s + index, (void*)&centerY, sizeof(centerY));
	index += sizeof(centerY);
	memcpy(s + index, (void*)&radius, sizeof(radius));
	index += sizeof(radius);

	int none = Entity::TYPE::kNone;
	memcpy(s + index, (void*)&none, sizeof(none));
}

void Circle::read(char *s, int& index)
{
	memcpy(&centerX, s + index, sizeof(centerX));
	index += sizeof(centerX);
	memcpy(&centerY, s + index, sizeof(centerY));
	index += sizeof(centerY);
	memcpy(&radius, s + index, sizeof(radius));
	index += sizeof(radius);
}


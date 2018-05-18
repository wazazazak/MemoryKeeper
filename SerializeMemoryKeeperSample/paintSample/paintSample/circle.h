#include "entity.h"

class Circle : public Entity
{
public:
	Circle()
	{
		centerX = 0; centerY = 0; radius = 0;
	}
	void setCenterPoint(int centerX, int centerY);
	void getCenterPoint(int& centerX, int& centerY);
	void setRadius(int radius);
	void getRadius(int& radius);
	virtual void draw();
	virtual int type();
	virtual void write(char *s, int& index);
	virtual void read(char *s, int& index);
private:
	int centerX, centerY, radius;
};
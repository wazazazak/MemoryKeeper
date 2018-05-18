#include "entity.h"

class Line : public Entity
{
public:
	Line()
	{
		stX = 0; stY = 0; endX = 0; endY = 0;
	}
	void setStartPoint(int stX, int stY);
	void getStartPoint(int& stX, int& stY);
	void setEndPoint(int endX, int endY);
	void getEndPoint(int& endX, int& endY);
	virtual void draw();
	virtual int type();
	virtual void write(char *s, int& index);
	virtual void read(char *s, int& index);
private:
	int stX, stY, endX, endY;
};
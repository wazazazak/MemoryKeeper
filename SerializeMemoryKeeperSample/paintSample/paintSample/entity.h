#pragma once
class Entity
{
public:
	enum TYPE
	{
		kNone = 0,
		kLIne,
		kCircle
	};
	virtual int type();
	virtual void draw();
	virtual void write(char *s, int& index);
	virtual void read(char *s, int& index);
};
#include <vector>
#include "entity.h"

class DB
{
public:
	DB(char * sharedMem);
	int numEnts(){return m_arEntitys.size();}
	void getEntityAt(int index, Entity*& ent);
	void setEntityAt(int index, Entity* ent);
	void appendEntity(Entity* entity);
	void write(char* s);
	void read(char* s);
	void clearDB();
	void refreshShaerdMemory();
private:
	std::vector<Entity*> m_arEntitys;
	char* sharedMem;
};
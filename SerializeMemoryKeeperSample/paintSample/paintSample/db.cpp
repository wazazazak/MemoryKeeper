#include "db.h"
#include "line.h"
#include "circle.h"

DB::DB(char * sharedMem)
{
	this->sharedMem = sharedMem;
}

void DB::getEntityAt(int index, Entity*& ent)
{
	ent = m_arEntitys.at(index);
}

void DB::setEntityAt(int index, Entity* ent)
{
	delete m_arEntitys.at(index);
	m_arEntitys.at(index) = ent;
}

void DB::appendEntity(Entity* entity)
{
	m_arEntitys.push_back(entity);
}

void DB::write(char* s)
{
	int count = 0;
	int size = m_arEntitys.size();
	for(auto pEnt : m_arEntitys)
	{
		pEnt->write(s, count);
	}
// 	memcpy(s + count, (void*)&size, sizeof(size));
// 	count += sizeof(size);
// 	for (auto line : m_arLines)
// 	{
// 		memcpy(s + count, (void*)&line.stX, sizeof(line.stX));
// 		count += sizeof(line.stX);
// 		memcpy(s + count, (void*)&line.stY, sizeof(line.stY));
// 		count += sizeof(line.stY);
// 		memcpy(s + count, (void*)&line.endX, sizeof(line.endX));
// 		count += sizeof(line.endX);
// 		memcpy(s + count, (void*)&line.endY, sizeof(line.endY));
// 		count += sizeof(line.endY);
// 	}
}

void DB::read(char* s)
{
	m_arEntitys.clear();

	int count = 0;
	while(true)
	{
		int type;
		memcpy(&type, s + count, sizeof(type));
		count += sizeof(type);
		if(type == Entity::TYPE::kNone)
		{
			break;
		}
		else if(type == Entity::TYPE::kLIne)
		{
			Line* pLine = new Line();
			pLine->read(s, count);
			appendEntity(pLine);
		}
		else if(type == Entity::TYPE::kCircle)
		{
			Circle* pCircle = new Circle();
			pCircle->read(s, count);
			appendEntity(pCircle);
		}
	}

// 	m_arLines.clear();
// 	int count = 0;
// 	int size;
// 	memcpy(&size, s + count, sizeof(size));
// 	count += sizeof(size);
// 	for (unsigned i(0); i < size; ++i)
// 	{
// 		int stX, stY, endX, endY;
// 		memcpy(&stX, s + count, sizeof(stX));
// 		count += sizeof(stX);
// 		memcpy(&stY, s + count, sizeof(stY));
// 		count += sizeof(stY);
// 		memcpy(&endX, s + count, sizeof(endX));
// 		count += sizeof(endX);
// 		memcpy(&endY, s + count, sizeof(endY));
// 		count += sizeof(endY);
// 		Line line;
// 		line.stX = stX;
// 		line.stY = stY;
// 		line.endX = endX;
// 		line.endY = endY;
// 		m_arLines.push_back(line);
// 	}
}

void DB::clearDB()
{
	for(auto pEnt : m_arEntitys)
	{
		delete pEnt;
	}
	m_arEntitys.clear();
}

void DB::refreshShaerdMemory()
{
	write(sharedMem);
}


#include <vector>
class Line
{
public:
	Line()
	{
		stX = 0; stY = 0; endX = 0; endY = 0;
	}
public:
	int stX, stY, endX, endY;
};

class DB
{
public:
	DB()
	{
	}
	int numLines(){return m_arLines.size();}
	void getLineAt(int index, Line& line)
	{
		line = m_arLines.at(index);
	}
	void setLineAt(int index, const Line& line)
	{
		m_arLines.at(index).stX = line.stX;
		m_arLines.at(index).stY = line.stY;
		m_arLines.at(index).endX = line.endX;
		m_arLines.at(index).endY = line.endY;
	}
	void appendLine(const Line& line)
	{
		m_arLines.push_back(line);
	}
	void write(char* s)
	{
		int count = 0;
		int size = m_arLines.size();
		memcpy(s + count, (void*)&size, sizeof(size));
		count += sizeof(size);
		for (auto line : m_arLines)
		{
			memcpy(s + count, (void*)&line.stX, sizeof(line.stX));
			count += sizeof(line.stX);
			memcpy(s + count, (void*)&line.stY, sizeof(line.stY));
			count += sizeof(line.stY);
			memcpy(s + count, (void*)&line.endX, sizeof(line.endX));
			count += sizeof(line.endX);
			memcpy(s + count, (void*)&line.endY, sizeof(line.endY));
			count += sizeof(line.endY);
		}
	}
	void read(char* s)
	{
		m_arLines.clear();
		int count = 0;
		int size;
		memcpy(&size, s + count, sizeof(size));
		count += sizeof(size);
		for (unsigned i(0); i < size; ++i)
		{
			int stX, stY, endX, endY;
			memcpy(&stX, s + count, sizeof(stX));
			count += sizeof(stX);
			memcpy(&stY, s + count, sizeof(stY));
			count += sizeof(stY);
			memcpy(&endX, s + count, sizeof(endX));
			count += sizeof(endX);
			memcpy(&endY, s + count, sizeof(endY));
			count += sizeof(endY);
			Line line;
			line.stX = stX;
			line.stY = stY;
			line.endX = endX;
			line.endY = endY;
			m_arLines.push_back(line);
		}
	}
private:
	std::vector<Line> m_arLines;
};
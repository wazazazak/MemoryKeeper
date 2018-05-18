#include <windows.h>

#define SHAREDMEMORY_SIZE 20 * 1024 * 1024

void BaseMemoryBuf::open(std::string fileName)
{
	m_sFileName = fileName;
	std::string sharedMemoryName = fileName + std::to_wstring(getpid()).c_str();

	m_hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   
		FALSE,                 
		sharedMemoryName);     

	if (m_hMapFile == NULL)
	{
		m_hMapFile = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SHAREDMEMORY_SIZE, sharedMemoryName);

		if( m_hMapFile == NULL )
		{
			return;
		}
	}
	m_pBuf = (char*)::MapViewOfFile(m_hMapFile,   
		FILE_MAP_ALL_ACCESS, 
		0,
		0,
		SHAREDMEMORY_SIZE);
	if(m_pBuf == NULL)
	{
		CloseHandle(m_hMapFile);
	}
}

void BaseMemoryBuf::close()
{
	memcpy(m_pBuf, &m_size, sizeof(m_size));
}

int BaseMemoryBuf::length()
{
	return m_size;
}

int BaseMemoryBuf::seek(int offset)
{
	return m_count = offset + 4;
}

int BaseMemoryBuf::tell()
{
	return m_count - 4;
}

bool BaseMemoryBuf::isEof()
{
	return m_size == m_count - 4;
}

byte BaseMemoryBuf::getByte()
{
	byte b = m_pBuf[m_count];
	m_count++;
	return b;
}

void BaseMemoryBuf::getBytes(void* buffer, int nLen)
{
	memcpy(buffer, m_pBuf + m_count, nLen);
	m_count += nLen;
}

void BaseMemoryBuf::putByte(byte val)
{
	m_pBuf[m_count] = val;
	m_count ++;
	if(m_count - 4 > m_size) m_size = m_count - 4;
}

void BaseMemoryBuf::putBytes(const void* buffer, int nLen)
{
	memcpy(m_pBuf + m_count, buffer, nLen);
	m_count += nLen;
	if(m_count - 4 > m_size) m_size = m_count - 4;
}

void BaseMemoryBuf::truncate()
{
	m_size = m_count - 4;
}

void BaseMemoryBuf::copyDataTo(char* pDest, int nSrcStart, int nSrcEnd)
{
	memcpy(pDest, m_pBuf + nSrcStart + 4, nSrcEnd - nSrcStart);
}
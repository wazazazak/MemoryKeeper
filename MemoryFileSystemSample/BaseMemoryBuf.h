#include <string>

#ifndef MEMORYBUF_DEFINED
#define MEMORYBUF_DEFINED

class BaseMemoryBuf
{
protected:
  HANDLE    m_hMapFile;
  char*     m_pBuf;
  int		m_count;
  int		m_size;
  std::string	m_sFileName;

public:
  BaseMemoryBuf(const std::string& sFileName)
  {
	  m_count = 4;
	  m_size = 0;
	  m_sFileName = sFileName;
	  open(sFileName);
  }

public:
  virtual void open(std::string fileName);

  virtual void close();

  virtual std::string fileName() { return m_sFileName; }

  virtual int length();
  
  virtual int seek(int offset);

  virtual int tell();

  virtual bool isEof();

  virtual byte getByte();

  virtual void getBytes(void* buffer, int numBytes);

  virtual void putByte(byte value);

  virtual void putBytes(const void* buffer, int numBytes);

  virtual void truncate();

 virtual void copyDataTo(char* pDestination, int sourceStart, int sourceEnd);
};
#endif

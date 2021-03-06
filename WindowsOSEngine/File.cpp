#include "stdafx.h"
#include "File.h"

#include <fstream>
using namespace std;

CFile::CFile(const std::string& filename) : m_filename(filename), m_isModified(false)
{
  Load();
}

CFile::~CFile()
{
  Unload();
}

const std::string& CFile::GetFileName() const
{
  return m_filename;
}

size_t CFile::GetFileSize() const
{
  return m_data.size();
}

std::vector<unsigned char>& CFile::GetFileData()
{
  return m_data;
}

void CFile::Load()
{
  SetModified(false);

  ifstream inp;
  inp.open(m_filename, ios::binary | ios::ate);
  if (!inp.good())
    return;

  size_t size = (size_t)inp.tellg();
  if (size == 0)
    return;

  inp.seekg(0, ios::beg);

  m_data.clear();
  m_data.resize(size);
  inp.read((char*)&m_data[0], size);
}

void CFile::Unload()
{
  if (IsModified())
  {
    ofstream out;
    out.open(m_filename, ios::binary | ios::ate);

    size_t fsize = GetFileSize();
    if (fsize)
      out.write((const char*)&GetFileData()[0], fsize);
  }

  m_data.clear();
}

void CFile::SetModified(bool val)
{
  m_isModified = val;
}

bool CFile::IsModified() const
{
  return m_isModified;
}

void CFile::ResizeChunk(size_t startPos, size_t endPos, size_t newSize)
{
  int sizeDiff = ((int)newSize) - (((int)endPos) - ((int)startPos));
  int oldSize = m_data.size();

  if (sizeDiff == 0)
    return;

  std::vector<unsigned char> back;
  {
    size_t backSize = oldSize - endPos;
    if (backSize > 0)
    {
      back.resize(backSize);
      memcpy(&back[0], &m_data[endPos], backSize);
    }
  }

  Resize(oldSize + sizeDiff);

  if (!back.empty())
    memcpy(&m_data[endPos + sizeDiff], &back[0], back.size());
}

void CFile::Resize(size_t newSize)
{
  size_t oldSize = m_data.size();

  if (oldSize == newSize)
    return;

  m_data.resize(newSize);
  if (oldSize < newSize)
    memset(&m_data[oldSize], 0, newSize - oldSize);

  SetModified(true);
}

const std::vector<unsigned char>& CFile::GetFileData() const
{
  return m_data;
}

void CFile::Clear()
{
  m_data.clear();
}

void CFile::Write(unsigned char * data, int size)
{
  if (size <= 0 || !data)
    return;

  SetModified(true);

  size_t oldSize = GetFileSize();
  size_t newSize = oldSize + size;

  m_data.reserve(newSize);
  for (int i = 0; i < size; i++)
    m_data.push_back(data[i]);
}

void CFile::Destroy()
{
  delete this;
}

#include "stdafx.h"
#include "File.h"

#include <fstream>
using namespace std;

CFile::CFile(const char * filename) : m_data(nullptr), m_filename(nullptr), m_size(0)
{
  int nameLen = strlen(filename) + 1;
  m_filename = new char[nameLen];
  strcpy(m_filename, filename);

  Load();
}

CFile::~CFile()
{
  if (m_filename)
  {
    delete[] m_filename;
    m_filename = nullptr;
  }

  Unload();
}

const char * CFile::GetFileName() const
{
  return m_filename;
}

size_t CFile::GetFileSize() const
{
  return m_size;
}

unsigned char * CFile::GetFileData()
{
  return m_data;
}

void CFile::Load()
{
  ifstream inp;
  inp.open(m_filename, ios::binary | ios::ate);

  m_size = inp.tellg();
  inp.seekg(0, ios::beg);

  m_data = new unsigned char[m_size];
  inp.read((char*)m_data, m_size);
}

void CFile::Unload()
{
  if (m_data)
  {
    delete[] m_data;
    m_data = nullptr;
  }
  m_size = 0;
}

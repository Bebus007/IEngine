#pragma once

#include "IFile.h"

class CFile : public IFile
{
public:
  CFile(const char* filename);
  ~CFile();

  virtual const char* GetFileName() const override;
  virtual size_t GetFileSize() const override;

  virtual unsigned char* GetFileData() override;

protected:
  void Load();
  void Unload();

private:
  char* m_filename;
  size_t m_size;
  unsigned char* m_data;
};
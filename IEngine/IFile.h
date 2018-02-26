#pragma once

class IFile
{
public:
  virtual const char* GetFileName() const = 0;
  virtual size_t GetFileSize() const = 0;

  virtual unsigned char* GetFileData() = 0;
};
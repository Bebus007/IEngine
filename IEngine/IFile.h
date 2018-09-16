#pragma once

#include "IEngineObject.h"

#include <string>
#include <vector>

class IFile : public IEngineObject
{
public:
  virtual const std::string& GetFileName() const = 0;
  virtual size_t GetFileSize() const = 0;
  virtual const std::vector<unsigned char>& GetFileData() const = 0;

  virtual std::vector<unsigned char>& GetFileData() = 0;
  virtual void Clear() = 0;
  virtual void Write(unsigned char* data, int size) = 0;
};
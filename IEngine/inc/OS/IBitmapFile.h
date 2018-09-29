#pragma once

#include "IFile.h"

class IBitmapFile : virtual public IFile
{
public:
  virtual int GetBitmapWidth() const = 0;
  virtual int GetBitmapHeight() const = 0;
  virtual unsigned int GetBitmapColorBitCount() const = 0;
  virtual unsigned short GetBitmapPlanesCount() const = 0;

  virtual int GetBitmapDataSize() const = 0;
  virtual const void* GetBitmapData() const = 0;

  virtual void SetBitmapWidth(int) = 0;
  virtual void SetBitmapHeight(int) = 0;
  virtual void SetBitmapColorBitCount(unsigned int) = 0;
  virtual void SetBitmapPlanesCount(unsigned short) = 0;

  virtual void SetBitmapData(const void*) = 0;

};
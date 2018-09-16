#pragma once

#include "IEngineObject.h"

class IBitmap : public IEngineObject
{
public:
  virtual int GetWidth() const = 0;
  virtual int GetHeight() const = 0;
  virtual unsigned int GetColorBitCount() const = 0;

  virtual void Resize(int width, int height, int colorBitCount, const void* data = nullptr) = 0;
  virtual void HMirror() = 0;

  virtual int GetDataSize() const = 0;
  virtual const void* GetBits() const = 0;
  virtual void SetBits(const void*) = 0;

  virtual IBitmap* CreateRegionCopy(int x, int y, int width, int height) const = 0;

  virtual IBitmap* CreateCopy() const { return CreateRegionCopy(0, 0, GetWidth(), GetHeight()); }
};
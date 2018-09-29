#pragma once

#include "IEngineObject.h"

class IImage : public IEngineObject
{
public:
  enum class ImageFormat_e : unsigned char
  {
    IF_UNKNOWN,
    IF_R8G8B8A8,
    IF_B8G8R8A8
  };
  virtual int GetWidth() const = 0;
  virtual int GetHeight() const = 0;
  virtual unsigned int GetColorBitCount() const = 0;
  virtual ImageFormat_e GetFormat() const = 0;

  virtual void Init(int width, int height, int colorBitCount, const void* data = nullptr) = 0;
  virtual void HMirror() = 0;

  virtual int GetDataSize() const = 0;
  virtual const void* GetBits() const = 0;
  virtual void SetBits(const void*) = 0;

  virtual IImage* CreateRegionCopy(int x, int y, int width, int height) const = 0;

  virtual IImage* CreateCopy() const { return CreateRegionCopy(0, 0, GetWidth(), GetHeight()); }
};
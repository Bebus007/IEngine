#pragma once

#include "IImage.h"

class CGDIImage : public IImage
{
public:
  CGDIImage();
  CGDIImage(int width, int height, unsigned int colorBitCount, const void* pBits = nullptr);
  CGDIImage(const IImage&);
  CGDIImage(const BITMAP&);
  ~CGDIImage();

  void Init(int width, int height, unsigned int colorBitCount, const void* pBits = nullptr);

  bool IsValid() const;

  virtual int GetWidth() const override;
  virtual int GetHeight() const override;
  virtual unsigned int GetColorBitCount() const override;
  WORD GetPlanesCount() const;
  LONG GetWidthBytes() const;
  void* GetBits();

  virtual void Resize(int width, int height, int colorBitCount, const void* data = nullptr) override;
  virtual void HMirror() override;

  virtual int GetDataSize() const override;
  virtual const void* GetBits() const override;
  virtual void SetBits(const void*) override;

  virtual IImage* CreateRegionCopy(int x, int y, int width, int height) const override;

  virtual void Destroy() override;

private:

  BITMAP m_bitmap;
};
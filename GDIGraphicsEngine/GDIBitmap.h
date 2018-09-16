#pragma once

#include "IBitmap.h"

class CGDIBitmap : public IBitmap
{
public:
  CGDIBitmap();
  CGDIBitmap(int width, int height, unsigned int colorBitCount, const void* pBits = nullptr);
  CGDIBitmap(const IBitmap&);
  CGDIBitmap(const BITMAP&);
  ~CGDIBitmap();

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

  virtual IBitmap* CreateRegionCopy(int x, int y, int width, int height) const override;

  virtual void Destroy() override;

private:

  BITMAP m_bitmap;
};
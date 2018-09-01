#pragma once

#include "IBitmap.h"

class CGDIBitmap : public IBitmap
{
public:
  CGDIBitmap();
  CGDIBitmap(int width, int height, unsigned int colorBitCount, const void* pBits = nullptr);
  CGDIBitmap(const IBitmap&);
  ~CGDIBitmap();

  void Init(int width, int height, unsigned int colorBitCount, const void* pBits = nullptr);

  bool IsValid() const;

  virtual int GetWidth() const override;
  virtual int GetHeight() const override;
  virtual unsigned int GetColorBitCount() const override;

  virtual void Resize(int width, int height, void* data = nullptr) override;

  virtual int GetDataSize() const override;
  virtual void GetBits(void*) const override;
  virtual void SetBits(const void*) override;

  virtual IBitmap* CreateRegionCopy(int x, int y, int width, int height) const override;

  virtual void Destroy() override;

  HBITMAP GetBitmapHandle();

private:
  int m_width, m_height;
  unsigned int m_colorBitCount;
  HBITMAP m_hBitmap;
};
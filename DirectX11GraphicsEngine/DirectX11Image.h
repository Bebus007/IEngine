#pragma once

#include "IImage.h"

class CDirectX11RenderSystem;

class CDirectX11Image : public IImage
{
public:
  CDirectX11Image(CDirectX11RenderSystem* rs);
  CDirectX11Image(ID3D11Texture2D*, CDirectX11RenderSystem*);
  ~CDirectX11Image();

  virtual void Destroy() override;
  virtual int GetWidth() const override;
  virtual int GetHeight() const override;
  virtual unsigned int GetColorBitCount() const override;
  virtual ImageFormat_e GetFormat() const override;
  virtual void Init(int width, int height, int colorBitCount, const void * data = nullptr) override;
  virtual void HMirror() override;
  virtual int GetDataSize() const override;
  virtual const void * GetBits() const override;
  virtual void SetBits(const void *) override;
  virtual IImage * CreateRegionCopy(int x, int y, int width, int height) const override;

private:
  int GetWidthBytesCount() const;
  static ImageFormat_e GetFormat(DXGI_FORMAT);
  static DXGI_FORMAT GetFormat(int colorBitCount);

  CDirectX11RenderSystem* m_pRenderSystem;

  ID3D11Texture2D* m_pTexture;

  mutable void* m_tempDataBuffer;
};
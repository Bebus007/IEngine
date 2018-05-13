#pragma once

#include "IGraphicsEngine.h"

class CDirextX11RenderSystem;
class CGraphics2DDirectX11;

class IGraphics2D;

class IWindowEx;

class CDirectX11GraphicsEngine : public IGraphicsEngine
{
public:
  CDirectX11GraphicsEngine(IWindowEx*);
  ~CDirectX11GraphicsEngine();

  virtual bool Init(int width = 640, int height = 480, bool fullscreen = false) override;
  virtual void SetClearColor(float r, float g, float b, float a = 1.0f) override;
  virtual void ClearScreen() override;
  virtual void Swap() override;

  virtual IGraphics2D* Get2DInterface() override;

private:
  void Cleanup();

  float m_ClearColor[4];

  CDirextX11RenderSystem* m_pRenderSystem;

  CGraphics2DDirectX11   *m_p2DInterface;
};
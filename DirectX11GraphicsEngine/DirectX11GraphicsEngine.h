#pragma once

#include "IGraphicsEngine.h"

#include "IWindow.h"

class CDirectX11RenderSystem;
class CDirectX11Graphics2D;
class CDirectX11ShaderSet;

class IGraphics2D;

class IWindowEx;

class CDirectX11GraphicsEngine : public IGraphicsEngine, public IWindow::IWindowResizeEventListener
{
public:
  CDirectX11GraphicsEngine(IWindowEx*);
  ~CDirectX11GraphicsEngine();

  virtual bool Init(int width = 640, int height = 480, bool fullscreen = false) override;
  virtual void SetClearColor(float r, float g, float b, float a = 1.0f) override;
  virtual void ClearScreen() override;
  virtual void Swap() override;

  virtual void Destroy() override;

  virtual void DrawTriangle(Vertex_t a, Vertex_t b, Vertex_t c) override;

  virtual IGraphics2D* Get2DInterface() override;

  virtual void HandleWindowResize(int newW, int newH) override;

  IWindowEx* GetWindow() const;

private:
  void Cleanup();

  float m_ClearColor[4];

  CDirectX11RenderSystem* m_pRenderSystem;

  CDirectX11Graphics2D   *m_p2DInterface;

  CDirectX11ShaderSet* m_pTestShaderSet;

  IWindowEx* m_pWindow;
};
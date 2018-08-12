#pragma once

#include "IGraphics2D.h"

class CDirectX11RenderSystem;

class CDirectX11ShaderSet;

class CDirectX11Graphics2D : public IGraphics2D
{
public:
  CDirectX11Graphics2D(CDirectX11RenderSystem* pRenderSystem);
  ~CDirectX11Graphics2D();

  virtual void DrawTriangle(Vertex a, Vertex b, Vertex c) override;

  virtual int GetWidth() const override;
  virtual int GetHeight() const override;
private:
  CDirectX11RenderSystem * m_pRenderSystem;

  void InitTestShaderSet();
  CDirectX11ShaderSet* m_pTestShaderSet;
};
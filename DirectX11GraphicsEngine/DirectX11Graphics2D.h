#pragma once

#include "IGraphics2D.h"

class CDirextX11RenderSystem;

class CDirectX11Graphics2D : public IGraphics2D
{
public:
  CDirectX11Graphics2D(CDirextX11RenderSystem* pRenderSystem);
  ~CDirectX11Graphics2D();

  virtual void DrawTriangle(Vertex a, Vertex b, Vertex c) override;
private:
  CDirextX11RenderSystem* m_pRenderSystem;

  void InitTestShaders();
  ID3D11PixelShader* m_pTestPixelShader;
  ID3D11VertexShader* m_pTestVertexShader;
};
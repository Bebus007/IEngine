#pragma once

#include "IGraphics2D.h"

class CDirectX11RenderSystem;

class CDirectX11ShaderSet;

class CDirectX11Graphics2D : public IGraphics2D
{
public:
  CDirectX11Graphics2D(CDirectX11RenderSystem* pRenderSystem);
  ~CDirectX11Graphics2D();

  virtual void DrawTriangle(Vertex_t a, Vertex_t b, Vertex_t c) override;

  virtual int GetWidth() const override;
  virtual int GetHeight() const override;

  virtual IFont* CreateFontInstance(int height, const char* fontname) override;
  virtual void DrawText(const Vertex_t& pos, IFont* font, const char* text) override;
  virtual Size_t GetTextSize(IFont* pFont, const char* text) override;
private:
  CDirectX11RenderSystem * m_pRenderSystem;

  void InitTestShaderSet();
  CDirectX11ShaderSet* m_pTestShaderSet;
};
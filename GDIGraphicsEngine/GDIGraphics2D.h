#pragma once

#include "IGraphics2D.h"

class CGDIGraphics2D : public IGraphics2D
{
public:
  CGDIGraphics2D(HDC);

  virtual void DrawTriangle(Vertex_t a, Vertex_t b, Vertex_t c) override;

  virtual int GetWidth() const override;
  virtual int GetHeight() const override;

  virtual IFont* CreateFontInstance(int height, const char* fontname) override;
  virtual void DrawText(const Vertex_t& pos, IFont* font, const char* text) override;
  virtual Size_t GetTextSize(IFont* pFont, const char* text) override;

private:
  HDC m_hDC;
};
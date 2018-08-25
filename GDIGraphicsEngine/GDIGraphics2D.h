#pragma once

#include "IGraphics2D.h"

class CGDIGraphics2D : public IGraphics2D
{
public:
  CGDIGraphics2D(HDC);

  virtual void DrawTriangle(Vertex a, Vertex b, Vertex c) override;

  virtual int GetWidth() const override;
  virtual int GetHeight() const override;

  virtual IFont* CreateFontInstance(int height, const char* fontname) override;
  virtual void DrawText(const Vertex& pos, IFont* font, const char* text) override;
  virtual Vertex GetTextSize(IFont* pFont, const char* text) override;

private:
  HDC m_hDC;
};
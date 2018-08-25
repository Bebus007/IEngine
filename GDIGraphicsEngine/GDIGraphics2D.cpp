#include "stdafx.h"
#include "GDIGraphics2D.h"

#include "GDIFont.h"

CGDIGraphics2D::CGDIGraphics2D(HDC hDC) : m_hDC(hDC)
{
}

void CGDIGraphics2D::DrawTriangle(Vertex_t a, Vertex_t b, Vertex_t c)
{
}

int CGDIGraphics2D::GetWidth() const
{
  return GetDeviceCaps(m_hDC, HORZRES);
}

int CGDIGraphics2D::GetHeight() const
{
  return GetDeviceCaps(m_hDC, VERTRES);
}

IFont * CGDIGraphics2D::CreateFontInstance(int height, const char * fontname)
{
  HFONT hFont = CreateFontA(height, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, fontname);
  if (!hFont)
    return nullptr;

  return new CGDIFont(hFont);
}

void CGDIGraphics2D::DrawText(const Vertex_t & pos, IFont * pFont, const char * text)
{
  if (!pFont || !text)
    return;

  size_t textlen = strlen(text);
  if (textlen == 0)
    return;

  CGDIFont* pGDIFont = dynamic_cast<CGDIFont*>(pFont);
  if (!pGDIFont || !pGDIFont->GetFontHandle())
    return;

  HGDIOBJ hfOld = SelectObject(m_hDC, (HGDIOBJ)pGDIFont->GetFontHandle());

  COLORREF bkColor = 0xffffffff;
  COLORREF textColor = 0x0;
  SetBkColor(m_hDC, bkColor);
  SetTextColor(m_hDC, textColor);

  SetBkMode(m_hDC, TRANSPARENT);

  RECT rc;
  rc.top = (LONG)pos.Y;
  rc.left = (LONG)pos.X;
  rc.bottom = (LONG)(pos.Y + 100);
  rc.right = (LONG)(pos.X + 300);
  DrawTextA(m_hDC, text, -1, &rc, DT_NOCLIP);

  SelectObject(m_hDC, hfOld);
}

IGraphics2D::Size_t CGDIGraphics2D::GetTextSize(IFont* pFont, const char* text)
{
  if (!pFont || !text)
    return Size_t();

  size_t textlen = strlen(text);
  if (textlen == 0)
    return Size_t();

  CGDIFont* pGDIFont = dynamic_cast<CGDIFont*>(pFont);
  if (!pGDIFont || !pGDIFont->GetFontHandle())
    return Size_t();

  HGDIOBJ hfOld = SelectObject(m_hDC, (HGDIOBJ)pGDIFont->GetFontHandle());

  SIZE textSize;
  GetTextExtentPoint32A(m_hDC, text, textlen, &textSize);

  SelectObject(m_hDC, hfOld);

  return Size_t((float)textSize.cx, (float)textSize.cy);
}

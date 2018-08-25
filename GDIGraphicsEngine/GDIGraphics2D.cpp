#include "stdafx.h"
#include "GDIGraphics2D.h"

#include "GDIFont.h"

CGDIGraphics2D::CGDIGraphics2D(HDC hDC) : m_hDC(hDC)
{
}

void CGDIGraphics2D::DrawTriangle(Vertex a, Vertex b, Vertex c)
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

void CGDIGraphics2D::DrawText(const Vertex & pos, IFont * pFont, const char * text)
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
  rc.top = pos.Y;
  rc.left = pos.X;
  rc.bottom = pos.Y + 100;
  rc.right = pos.X + 300;
  DrawTextA(m_hDC, text, -1, &rc, DT_NOCLIP);

  SelectObject(m_hDC, hfOld);
}

IGraphics2D::Vertex CGDIGraphics2D::GetTextSize(IFont* pFont, const char* text)
{
  if (!pFont || !text)
    return Vertex();

  size_t textlen = strlen(text);
  if (textlen == 0)
    return Vertex();

  CGDIFont* pGDIFont = dynamic_cast<CGDIFont*>(pFont);
  if (!pGDIFont || !pGDIFont->GetFontHandle())
    return Vertex();

  HGDIOBJ hfOld = SelectObject(m_hDC, (HGDIOBJ)pGDIFont->GetFontHandle());

  SIZE textSize;
  GetTextExtentPoint32A(m_hDC, text, textlen, &textSize);

  SelectObject(m_hDC, hfOld);

  return Vertex(textSize.cx, textSize.cy);
}

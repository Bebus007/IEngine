#include "stdafx.h"
#include "GDIGraphics2D.h"

#include "GDIFont.h"
#include "GDIImage.h"

CGDIGraphics2D::CGDIGraphics2D(HDC hDC) : m_hDC(hDC)
{
}

void CGDIGraphics2D::DrawTriangle(Vertex_t a, Vertex_t b, Vertex_t c)
{
}

void CGDIGraphics2D::DrawImage(const Vertex_t& pos, const IImage* pBitmap)
{
  if (!pBitmap)
    return;

  HDC hDC = GetDrawingContext();
  if (!hDC)
    return;

  HDC hMemDC = CreateCompatibleDC(hDC);
  if (hMemDC)
  {
    HBITMAP bitmap = CreateBitmap(pBitmap->GetWidth(), pBitmap->GetHeight(), 1, pBitmap->GetColorBitCount(), pBitmap->GetBits());
    if (bitmap)
    {
      HGDIOBJ oldbmp = SelectObject(hMemDC, bitmap); //<<<<save it for later ...

      // Copy the bits from the memory DC into the current dc
      BOOL res = BitBlt(hDC, pos.X, pos.Y, pBitmap->GetWidth(), pBitmap->GetHeight(), hMemDC, 0, 0, SRCCOPY);

      // Restore the old bitmap
      SelectObject(hMemDC, oldbmp);
      DeleteObject(bitmap);
    }
    DeleteDC(hMemDC);
  }
}

int CGDIGraphics2D::GetWidth() const
{
  return GetDeviceCaps(GetDrawingContext(), HORZRES);
}

int CGDIGraphics2D::GetHeight() const
{
  return GetDeviceCaps(GetDrawingContext(), VERTRES);
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

  HGDIOBJ hfOld = SelectObject(GetDrawingContext(), (HGDIOBJ)pGDIFont->GetFontHandle());

  COLORREF bkColor = 0xffffffff;
  COLORREF textColor = 0x0;
  SetBkColor(GetDrawingContext(), bkColor);
  SetTextColor(GetDrawingContext(), textColor);

  SetBkMode(GetDrawingContext(), TRANSPARENT);

  RECT rc;
  rc.top = (LONG)pos.Y;
  rc.left = (LONG)pos.X;
  rc.bottom = (LONG)(pos.Y + 100);
  rc.right = (LONG)(pos.X + 300);
  DrawTextA(GetDrawingContext(), text, -1, &rc, DT_NOCLIP);

  SelectObject(GetDrawingContext(), hfOld);
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

  HGDIOBJ hfOld = SelectObject(GetDrawingContext(), (HGDIOBJ)pGDIFont->GetFontHandle());

  SIZE textSize;
  GetTextExtentPoint32A(GetDrawingContext(), text, textlen, &textSize);

  SelectObject(GetDrawingContext(), hfOld);

  return Size_t((float)textSize.cx, (float)textSize.cy);
}

IImage* CGDIGraphics2D::CaptureScreen()
{
  HDC hdcMemDC = nullptr;
  HBITMAP hbmScreen = nullptr;

  // Create a compatible DC which is used in a BitBlt from the window DC
  hdcMemDC = CreateCompatibleDC(GetDrawingContext());
  if (!hdcMemDC)
    return nullptr;

  // Create a compatible bitmap from the Window DC
  hbmScreen = CreateCompatibleBitmap(GetDrawingContext(), GetWidth(), GetHeight());
  if (!hbmScreen)
    return nullptr;

  // Select the compatible bitmap into the compatible memory DC.
  SelectObject(hdcMemDC, hbmScreen);

  // Bit block transfer into our compatible memory DC.
  if (!BitBlt(hdcMemDC,
    0, 0,
    GetWidth(), GetHeight(),
    GetDrawingContext(),
    0, 0,
    SRCCOPY))
    return nullptr;

  // Get the BITMAP from the HBITMAP
  BITMAP bmpScreen;
  GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

  if (!bmpScreen.bmBits)
  {
    LONG bytesCount = bmpScreen.bmWidthBytes * bmpScreen.bmHeight;
    bmpScreen.bmBits = new BYTE[bytesCount];
    GetBitmapBits(hbmScreen, bytesCount, bmpScreen.bmBits);
  }

  IImage* result = new CGDIImage(bmpScreen);

  if (bmpScreen.bmBits)
    delete bmpScreen.bmBits;

  DeleteObject(hdcMemDC);
  DeleteObject(hbmScreen);

  return result;
}

IImage * CGDIGraphics2D::CreateEmptyImage()
{
  return new CGDIImage();
}

HDC CGDIGraphics2D::GetDrawingContext() const { return m_hDC; }

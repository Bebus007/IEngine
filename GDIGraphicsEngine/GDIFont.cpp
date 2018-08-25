#include "stdafx.h"
#include "GDIFont.h"

CGDIFont::CGDIFont(HFONT hFont) : m_hFont(hFont)
{
}

CGDIFont::~CGDIFont()
{
  if (m_hFont)
  {
    DeleteObject(m_hFont);
    m_hFont = nullptr;
  }
}

void CGDIFont::Destroy()
{
  delete this;
}

HFONT CGDIFont::GetFontHandle()
{
  return m_hFont;
}

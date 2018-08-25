#pragma once

#include "IFont.h"

class CGDIFont : public IFont
{
public:
  CGDIFont(HFONT hFont);
  ~CGDIFont();

  virtual void Destroy() override;

  HFONT GetFontHandle();

private:
  HFONT m_hFont;
};
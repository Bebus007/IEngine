#include "stdafx.h"
#include "GDIBitmap.h"

CGDIBitmap::CGDIBitmap() : m_bitmap({ 0 }) { }

CGDIBitmap::CGDIBitmap(int width, int height, unsigned int colorBitCount, const void* pBits) : CGDIBitmap()
{ Init(width, height, colorBitCount, pBits); }

CGDIBitmap::CGDIBitmap(const IBitmap& bmp) : CGDIBitmap(bmp.GetWidth(), bmp.GetHeight(), bmp.GetColorBitCount(), bmp.GetBits()) { }

CGDIBitmap::~CGDIBitmap()
{
  if (m_bitmap.bmBits)
    delete[] m_bitmap.bmBits;

  m_bitmap = { 0 };
}

void CGDIBitmap::Init(int width, int height, unsigned int colorBitCount, const void* pBits)
{
  if (m_bitmap.bmBits)
    delete[] m_bitmap.bmBits;

  m_bitmap = { 0 };

  m_bitmap.bmWidth = width, m_bitmap.bmHeight = height, m_bitmap.bmBitsPixel = colorBitCount;
  m_bitmap.bmPlanes = 1;
  m_bitmap.bmType = 0;
  m_bitmap.bmWidthBytes = width * colorBitCount / 8;

  if (IsValid())
  {
    int dataSize = GetDataSize();
    void* pDataCopy = new BYTE[dataSize];
    memcpy(pDataCopy, pBits, dataSize);
    m_bitmap.bmBits = pDataCopy;
    pDataCopy = nullptr;
  }
}

bool CGDIBitmap::IsValid() const { return GetWidth() > 0 && GetHeight() > 0 && GetColorBitCount() > 0; }

int CGDIBitmap::GetWidth() const { return m_bitmap.bmWidth; }

int CGDIBitmap::GetHeight() const { return m_bitmap.bmHeight; }

unsigned int CGDIBitmap::GetColorBitCount() const { return m_bitmap.bmBitsPixel; }

void CGDIBitmap::Resize(int width, int height, void* pData)
{
  if (pData)
  {
    Init(width, height, GetColorBitCount(), pData);
    return;
  }

  if (width == GetWidth() && height == GetHeight())
    return;

  IBitmap* regionBitmap = CreateRegionCopy(0, 0, width, height);
  const void* pOldData = regionBitmap->GetBits();
  if (regionBitmap)
    pOldData = regionBitmap->GetBits();

  Init(width, height, GetColorBitCount(), pOldData);

  if (regionBitmap)
    regionBitmap->Destroy();
}

int CGDIBitmap::GetDataSize() const { return m_bitmap.bmWidthBytes * GetHeight(); }

const void* CGDIBitmap::GetBits() const { return m_bitmap.bmBits; }

void CGDIBitmap::SetBits(const void* pArray)
{
  if (!IsValid() || !pArray)
    return;

  LONG byteArraySize = GetDataSize();

  memcpy(m_bitmap.bmBits, pArray, byteArraySize);
}

IBitmap * CGDIBitmap::CreateRegionCopy(int x, int y, int width, int height) const
{
  int pixelSize = GetColorBitCount() / 8;

  int newDataSize = width * height * pixelSize;
  if (newDataSize == 0)
    return nullptr;

  BYTE* newData = new BYTE[newDataSize];

  int myDataSize = GetDataSize();
  if (myDataSize)
  {
    const BYTE* myData = (const BYTE*)GetBits();

    for (int i = y; i < y + height; i++)
    {
      for (int j = x; j < x + width; j++)
      {
        int myPixelIdx = i * GetWidth() + j;
        int newPixelIdx = (i - y) * width + (j - x);

        for (int k = 0; k < pixelSize; k++)
          newData[newPixelIdx * pixelSize + k] = myData[myPixelIdx * pixelSize + k];
      }
    }
  }

  IBitmap* result = new CGDIBitmap(width, height, GetColorBitCount(), newData);
  delete[] newData;
  return result;
}

void CGDIBitmap::Destroy() { delete this; }

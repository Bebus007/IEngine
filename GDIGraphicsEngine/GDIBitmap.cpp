#include "stdafx.h"
#include "GDIBitmap.h"

CGDIBitmap::CGDIBitmap() : m_hBitmap(nullptr), m_width(0), m_height(0), m_colorBitCount(0u) { }

CGDIBitmap::CGDIBitmap(int width, int height, unsigned int colorBitCount, const void* pBits) : CGDIBitmap()
{
  Init(width, height, colorBitCount, pBits);
}

CGDIBitmap::CGDIBitmap(const IBitmap& bmp) : CGDIBitmap()
{
  BYTE* pBitsArray = nullptr;
  LONG byteArraySize = bmp.GetWidth() * bmp.GetHeight() * (bmp.GetColorBitCount() / 8);

  if (byteArraySize > 0)
  {
    pBitsArray = new BYTE[byteArraySize];
    bmp.GetBits(pBitsArray);
  }

  Init(bmp.GetWidth(), bmp.GetHeight(), bmp.GetColorBitCount(), pBitsArray);

  if (pBitsArray)
    delete[] pBitsArray;
}

CGDIBitmap::~CGDIBitmap()
{
  if (m_hBitmap)
  {
    DeleteObject(m_hBitmap);
    m_hBitmap = nullptr;
  }
}

void CGDIBitmap::Init(int width, int height, unsigned int colorBitCount, const void * pBits)
{
  if (m_hBitmap)
  {
    DeleteObject(m_hBitmap);
    m_hBitmap = nullptr;
  }

  m_width = width, m_height = height, m_colorBitCount = colorBitCount;

  if (IsValid())
    m_hBitmap = CreateBitmap(m_width, m_height, 1u, m_colorBitCount, pBits);
}

bool CGDIBitmap::IsValid() const { return GetWidth() > 0 && GetHeight() > 0 && GetColorBitCount() > 0; }

int CGDIBitmap::GetWidth() const { return m_width; }

int CGDIBitmap::GetHeight() const { return m_height; }

unsigned int CGDIBitmap::GetColorBitCount() const { return m_colorBitCount; }

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
  BYTE* pOldData = nullptr;
  if (regionBitmap && regionBitmap->GetDataSize())
  {
    pOldData = new BYTE[regionBitmap->GetDataSize()];
    regionBitmap->GetBits(pOldData);
  }

  if (pOldData)
    Init(width, height, GetColorBitCount(), pOldData);

  if (pOldData)
    delete[] pOldData;
  if (regionBitmap)
    regionBitmap->Destroy();
}

int CGDIBitmap::GetDataSize() const
{
  return GetWidth() * GetHeight() * (GetColorBitCount() / 8);
}

void CGDIBitmap::GetBits(void* pArray) const
{
  if (!IsValid() || !pArray)
    return;

  LONG byteArraySize = GetDataSize();

#if _DEBUG
  LONG bytesTransfered = 
#endif

    GetBitmapBits(m_hBitmap, byteArraySize, pArray);

#if _DEBUG
  if (bytesTransfered != byteArraySize)
    DebugBreak();
#endif
}

void CGDIBitmap::SetBits(const void* pArray)
{
  if (!IsValid() || !pArray)
    return;

  LONG byteArraySize = GetDataSize();

#if _DEBUG
  LONG bytesTransfered =
#endif

  SetBitmapBits(m_hBitmap, byteArraySize, pArray);

#if _DEBUG
  if (bytesTransfered != byteArraySize)
    DebugBreak();
#endif
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
    BYTE* myData = new BYTE[myDataSize];
    GetBits(myData);

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

    delete[] myData;
  }

  IBitmap* result = new CGDIBitmap(width, height, GetColorBitCount(), newData);
  delete[] newData;
  return result;
}

void CGDIBitmap::Destroy() { delete this; }

HBITMAP CGDIBitmap::GetBitmapHandle() { return m_hBitmap; }

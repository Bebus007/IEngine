#include "stdafx.h"
#include "GDIImage.h"

CGDIImage::CGDIImage() : m_bitmap({ 0 }) { }

CGDIImage::CGDIImage(int width, int height, unsigned int colorBitCount, const void* pBits) : CGDIImage()
{ Init(width, height, colorBitCount, pBits); }

CGDIImage::CGDIImage(const IImage& bmp) : CGDIImage(bmp.GetWidth(), bmp.GetHeight(), bmp.GetColorBitCount(), bmp.GetBits()) { }

CGDIImage::CGDIImage(const BITMAP & bmp) : CGDIImage(bmp.bmWidth, bmp.bmHeight, bmp.bmBitsPixel, bmp.bmBits) { }

CGDIImage::~CGDIImage()
{
  if (m_bitmap.bmBits)
    delete[] m_bitmap.bmBits;

  m_bitmap = { 0 };
}

void CGDIImage::Init(int width, int height, unsigned int colorBitCount, const void* pBits)
{
  if (m_bitmap.bmBits)
    delete[] m_bitmap.bmBits;

  m_bitmap = { 0 };

  if (!pBits)
    return;

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

bool CGDIImage::IsValid() const { return GetWidth() > 0 && GetHeight() > 0 && GetColorBitCount() > 0; }

int CGDIImage::GetWidth() const { return m_bitmap.bmWidth; }

int CGDIImage::GetHeight() const { return m_bitmap.bmHeight; }

unsigned int CGDIImage::GetColorBitCount() const { return m_bitmap.bmBitsPixel; }

WORD CGDIImage::GetPlanesCount() const { return m_bitmap.bmPlanes; }

LONG CGDIImage::GetWidthBytes() const { return m_bitmap.bmWidthBytes; }

void * CGDIImage::GetBits() { return m_bitmap.bmBits; }

void CGDIImage::Resize(int width, int height, int colorBitCount, const void* pData)
{
  if (pData)
  {
    Init(width, height, colorBitCount, pData);
    return;
  }

  if (width == GetWidth() && height == GetHeight())
    return;

  IImage* regionBitmap = CreateRegionCopy(0, 0, width, height);
  const void* pOldData = regionBitmap->GetBits();
  if (regionBitmap)
    pOldData = regionBitmap->GetBits();

  Init(width, height, colorBitCount, pOldData);

  if (regionBitmap)
    regionBitmap->Destroy();
}

int CGDIImage::GetDataSize() const { return GetWidthBytes() * GetHeight(); }

const void* CGDIImage::GetBits() const { return m_bitmap.bmBits; }

void CGDIImage::SetBits(const void* pArray)
{
  if (!IsValid() || !pArray)
    return;

  LONG byteArraySize = GetDataSize();

  memcpy(m_bitmap.bmBits, pArray, byteArraySize);
}

IImage * CGDIImage::CreateRegionCopy(int x, int y, int width, int height) const
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

  IImage* result = new CGDIImage(width, height, GetColorBitCount(), newData);
  delete[] newData;
  return result;
}

void CGDIImage::Destroy() { delete this; }

void CGDIImage::HMirror()
{
  if (GetDataSize() == 0)
    return;

  BYTE* mirroredData = new BYTE[GetDataSize()];

  BYTE** destRows = new BYTE*[GetHeight()];
  destRows[0] = mirroredData;
  BYTE** srcRows = new BYTE*[GetHeight()];
  srcRows[0] = (BYTE*)GetBits();

  for (int i = 1; i < GetHeight(); i++)
  {
    destRows[i] = destRows[i - 1] + GetWidthBytes();
    srcRows[i] = srcRows[i - 1] + GetWidthBytes();
  }

  for (int i = 0; i < GetHeight(); i++)
    memcpy(destRows[i], srcRows[GetHeight() - i - 1], GetWidthBytes());

  Init(GetWidth(), GetHeight(), GetColorBitCount(), mirroredData);

  delete[] srcRows;
  delete[] destRows;
  delete[] mirroredData;
}

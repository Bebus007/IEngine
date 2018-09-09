#include "stdafx.h"
#include "GDIBitmap.h"

CGDIBitmap::CGDIBitmap() : m_bitmap({ 0 }) { }

CGDIBitmap::CGDIBitmap(int width, int height, unsigned int colorBitCount, const void* pBits) : CGDIBitmap()
{ Init(width, height, colorBitCount, pBits); }

CGDIBitmap::CGDIBitmap(const IBitmap& bmp) : CGDIBitmap(bmp.GetWidth(), bmp.GetHeight(), bmp.GetColorBitCount(), bmp.GetBits()) { }

CGDIBitmap::CGDIBitmap(const BITMAP & bmp) : CGDIBitmap(bmp.bmWidth, bmp.bmHeight, bmp.bmBitsPixel, bmp.bmBits) { }

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

bool CGDIBitmap::IsValid() const { return GetWidth() > 0 && GetHeight() > 0 && GetColorBitCount() > 0; }

int CGDIBitmap::GetWidth() const { return m_bitmap.bmWidth; }

int CGDIBitmap::GetHeight() const { return m_bitmap.bmHeight; }

unsigned int CGDIBitmap::GetColorBitCount() const { return m_bitmap.bmBitsPixel; }

WORD CGDIBitmap::GetPlanesCount() const { return m_bitmap.bmPlanes; }

LONG CGDIBitmap::GetWidthBytes() const { return m_bitmap.bmWidthBytes; }

void * CGDIBitmap::GetBits() { return m_bitmap.bmBits; }

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

int CGDIBitmap::GetDataSize() const { return GetWidthBytes() * GetHeight(); }

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

void CGDIBitmap::Save(const char* filename)
{
  HMirror();
  HANDLE hf;                 // file handle  
  LPBYTE lpBits = (LPBYTE)GetBits();  // memory pointer  
  DWORD dwTotal;              // total count of bytes  
  DWORD cb;                   // incremental count of bytes  
  BYTE *hp;                   // byte pointer  
  DWORD dwTmp;
  PBITMAPINFO pbi = CreateInfo();

  if (!lpBits)
    return;

  // Create the .BMP file.  
  hf = CreateFileA(filename,
    GENERIC_READ | GENERIC_WRITE,
    (DWORD)0,
    NULL,
    CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    (HANDLE)NULL);
  if (hf == INVALID_HANDLE_VALUE)
    return;

  BITMAPFILEHEADER hdr = CreateFileHeader();       // bitmap file-header  

  // Copy the BITMAPFILEHEADER into the .BMP file.  
  if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
    (LPDWORD)&dwTmp, NULL))
    return;

  PBITMAPINFOHEADER pbih;     // bitmap info-header  
  pbih = (PBITMAPINFOHEADER)pbi;

  // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
  if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
    + pbih->biClrUsed * sizeof(RGBQUAD),
    (LPDWORD)&dwTmp, (NULL)))
  {
    LocalFree(pbi);
    return;
  }

  // Copy the array of color indices into the .BMP file.  
  dwTotal = cb = pbih->biSizeImage;
  LocalFree(pbi);
  hp = lpBits;
  if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
    return;

  // Close the .BMP file.  
  CloseHandle(hf);
  HMirror();
}

void CGDIBitmap::Destroy() { delete this; }

void CGDIBitmap::HMirror()
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

PBITMAPINFO CGDIBitmap::CreateInfo() const
{
  PBITMAPINFO result = nullptr;

  WORD cClrBits = 0;
  cClrBits = (WORD)(GetPlanesCount() * GetColorBitCount());
  if (cClrBits == 1)
    cClrBits = 1;
  else if (cClrBits <= 4)
    cClrBits = 4;
  else if (cClrBits <= 8)
    cClrBits = 8;
  else if (cClrBits <= 16)
    cClrBits = 16;
  else if (cClrBits <= 24)
    cClrBits = 24;
  else cClrBits = 32;

  // Allocate memory for the BITMAPINFO structure. (This structure  
  // contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
  // data structures.)  
  if (cClrBits < 24)
    result = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));

  // There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 
  else
    result = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

  // Initialize the fields in the BITMAPINFO structure.  

  result->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  result->bmiHeader.biWidth = GetWidth();
  result->bmiHeader.biHeight = GetHeight();
  result->bmiHeader.biPlanes = GetPlanesCount();
  result->bmiHeader.biBitCount = GetColorBitCount();
  if (cClrBits < 24)
    result->bmiHeader.biClrUsed = (1 << cClrBits);

  // If the bitmap is not compressed, set the BI_RGB flag.  
  result->bmiHeader.biCompression = BI_RGB;

  // Compute the number of bytes in the array of color  
  // indices and store the result in biSizeImage.  
  // The width must be DWORD aligned unless the bitmap is RLE 
  // compressed. 
  result->bmiHeader.biSizeImage = ((result->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8 * result->bmiHeader.biHeight;
  // Set biClrImportant to 0, indicating that all of the  
  // device colors are important.  
  result->bmiHeader.biClrImportant = 0;

  return result;
}

BITMAPFILEHEADER CGDIBitmap::CreateFileHeader() const
{
  WORD cClrBits = 0;
  cClrBits = (WORD)(GetPlanesCount() * GetColorBitCount());
  if (cClrBits == 1)
    cClrBits = 1;
  else if (cClrBits <= 4)
    cClrBits = 4;
  else if (cClrBits <= 8)
    cClrBits = 8;
  else if (cClrBits <= 16)
    cClrBits = 16;
  else if (cClrBits <= 24)
    cClrBits = 24;
  else cClrBits = 32;

  BITMAPFILEHEADER result;       // bitmap file-header  
  result.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
  // Compute the size of the entire file.  
  result.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
    ((cClrBits < 24) ? (1 << cClrBits) : 0) * sizeof(RGBQUAD) +
    ((GetWidth() * cClrBits + 31) & ~31) / 8 * GetHeight());
  result.bfReserved1 = 0;
  result.bfReserved2 = 0;

  // Compute the offset to the array of color indices.  
  result.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
    sizeof(BITMAPINFOHEADER) + ((cClrBits < 24) ? (1 << cClrBits) : 0) * sizeof(RGBQUAD);

  return result;
}

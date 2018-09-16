#include "stdafx.h"
#include "BitmapFile.h"

CBitmapFile::CBitmapFile(const std::string& filename) : CFile(filename) { }

#pragma region Getters
int CBitmapFile::GetBitmapWidth() const
{
  const PBITMAPINFO pbi = GetBitmapInfo();
  if (!pbi)
    return 0;

  return pbi->bmiHeader.biWidth;
}

int CBitmapFile::GetBitmapHeight() const
{
  const PBITMAPINFO pbi = GetBitmapInfo();
  if (!pbi)
    return 0;

  return pbi->bmiHeader.biHeight;
}

unsigned int CBitmapFile::GetBitmapColorBitCount() const
{
  const PBITMAPINFO pbi = GetBitmapInfo();
  if (!pbi)
    return 0;

  return pbi->bmiHeader.biBitCount;
}

WORD CBitmapFile::GetBitmapPlanesCount() const
{
  const PBITMAPINFO pbi = GetBitmapInfo();
  if (!pbi)
    return 0;

  return pbi->bmiHeader.biPlanes;
}

int CBitmapFile::GetBitmapDataSize() const
{
  int result = CFile::GetFileSize() - GetBitmapDataPosition();
  return result < 0 ? 0 : result;
}

const void * CBitmapFile::GetBitmapData() const
{
  if (GetBitmapDataSize() == 0)
    return nullptr;

  return &CFile::GetFileData()[GetBitmapDataPosition()];
}

void * CBitmapFile::GetBitmapData()
{
  if (GetBitmapDataSize() == 0)
    return nullptr;

  return &CFile::GetFileData()[GetBitmapDataPosition()];
}
#pragma endregion

void CBitmapFile::SetBitmapWidth(int val)
{
  if (GetBitmapWidth() == val)
    return;

  OnParamChanged(val, GetBitmapHeight(), GetBitmapPlanesCount(), GetBitmapColorBitCount());
}

void CBitmapFile::SetBitmapHeight(int val)
{
  if (GetBitmapHeight() == val)
    return;

  OnParamChanged(GetBitmapWidth(), val, GetBitmapPlanesCount(), GetBitmapColorBitCount());
}

void CBitmapFile::SetBitmapColorBitCount(unsigned int val)
{
  if (GetBitmapColorBitCount() == val)
    return;

  OnParamChanged(GetBitmapWidth(), GetBitmapHeight(), GetBitmapPlanesCount(), val);
}

void CBitmapFile::SetBitmapPlanesCount(WORD val)
{
  if (GetBitmapPlanesCount() == val)
    return;

  OnParamChanged(GetBitmapWidth(), GetBitmapHeight(), val, GetBitmapColorBitCount());
}

void CBitmapFile::SetBitmapData(const void * pdata)
{
  size_t dataSize = GetBitmapDataSize();
  if (dataSize == 0 || !pdata)
    return;

  void* mData = GetBitmapData();
  if (!mData)
    return;

  memcpy(mData, pdata, dataSize);
}

void CBitmapFile::Destroy() { delete this; }

#pragma region Private

#pragma region Block Access
const BITMAPFILEHEADER* CBitmapFile::GetHeader() const
{
  if (CFile::GetFileSize() == 0)
    return nullptr;

  return (BITMAPFILEHEADER*)&CFile::GetFileData()[0];;
}

BITMAPFILEHEADER * CBitmapFile::GetHeader()
{
  if (CFile::GetFileSize() == 0)
    return nullptr;

  return (BITMAPFILEHEADER*)&CFile::GetFileData()[0];;
}

constexpr size_t CBitmapFile::GetBitmapInfoPos() { return sizeof(BITMAPFILEHEADER); }

const PBITMAPINFO CBitmapFile::GetBitmapInfo() const
{
  if (CFile::GetFileSize() <= GetBitmapInfoPos())
    return nullptr;

  return (PBITMAPINFO)&CFile::GetFileData()[GetBitmapInfoPos()];
}

PBITMAPINFO CBitmapFile::GetBitmapInfo()
{
  if (CFile::GetFileSize() <= GetBitmapInfoPos())
    return nullptr;

  return (PBITMAPINFO)&CFile::GetFileData()[GetBitmapInfoPos()];
}

size_t CBitmapFile::GetBitmapDataPosition() const
{
  return GetBitmapInfoPos() + GetBitmapInfoSize();
}
#pragma endregion

void CBitmapFile::OnParamChanged(int width, int height, int planesCount, int colorBitCount)
{
  UpdateHeader(width, height, planesCount, colorBitCount);
  UpdateBitmapInfo(width, height, planesCount, colorBitCount);
  ResizeBitmapData(CalculateDataSize(width, height, planesCount, colorBitCount));
}

size_t CBitmapFile::CalculateDataSize(int width, int height, int planesCount, int colorBitCount)
{
  WORD cClrBits = GetClrBits(planesCount, colorBitCount);

  return ((width * cClrBits + 31) & ~31) / 8 * height;
}

void CBitmapFile::CreateHeader()
{
  CFile::Clear();

  CFile::Resize(sizeof(BITMAPFILEHEADER));
}

void CBitmapFile::UpdateHeader(int width, int height, int planesCount, int colorBitCount)
{
  BITMAPFILEHEADER* result = GetHeader();  // bitmap file-header
  if (!result)
  {
    CreateHeader();
    result = GetHeader();
    if (!result)
      return;
  }

  WORD cClrBits = GetClrBits(planesCount, colorBitCount);

  result->bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
  // Compute the size of the entire file.
  
  result->bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
    GetBitmapInfoSize(planesCount, colorBitCount) +
    CalculateDataSize(width, height, planesCount, colorBitCount));

  result->bfReserved1 = 0;
  result->bfReserved2 = 0;

  // Compute the offset to the array of color indices.  
  result->bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
    sizeof(BITMAPINFOHEADER) + ((cClrBits < 24) ? (1 << cClrBits) : 0) * sizeof(RGBQUAD);
}

size_t CBitmapFile::GetBitmapInfoSize() const
{
  return GetBitmapInfoSize(GetBitmapPlanesCount(), GetBitmapColorBitCount());
}

size_t CBitmapFile::GetBitmapInfoSize(int planesCount, int colorBitCount)
{
  WORD cClrBits = GetClrBits(planesCount, colorBitCount);

  return (cClrBits < 24) ?
    sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits) :
    // There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 
    sizeof(BITMAPINFOHEADER);
}

WORD CBitmapFile::GetClrBits() const
{
  return GetClrBits(GetBitmapPlanesCount(), GetBitmapColorBitCount());
}

WORD CBitmapFile::GetClrBits(int planesCount, int colorBitCount)
{
  WORD cClrBits = 0;
  cClrBits = (WORD)(planesCount * colorBitCount);
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

  return cClrBits;
}

void CBitmapFile::ResizeBitmapData(size_t newSize)
{
  if (GetBitmapDataSize() == newSize)
    return;

  CFile::Resize(GetBitmapDataPosition() + newSize);
}

void CBitmapFile::CreateBitmapInfo()
{
  PBITMAPINFO result = GetBitmapInfo();
  if (result)
    return;

  CFile::Resize(GetBitmapInfoPos() + GetBitmapInfoSize(0, 0));
}

void CBitmapFile::UpdateBitmapInfo(int width, int height, int planesCount, int colorBitCount)
{
  PBITMAPINFO result = GetBitmapInfo();
  if (!result)
  {
    CreateBitmapInfo();
    result = GetBitmapInfo();
    if (!result)
      return;
  }

  WORD cClrBits = GetClrBits(planesCount, colorBitCount);

  size_t oldSize = GetBitmapInfoSize();

  size_t bitmapInfoSize = GetBitmapInfoSize(planesCount, colorBitCount);

  // Allocate memory for the BITMAPINFO structure. (This structure  
  // contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
  // data structures.)  
  if (oldSize != bitmapInfoSize)
  {
    CFile::ResizeChunk(GetBitmapInfoPos(), GetBitmapInfoPos() + oldSize, bitmapInfoSize);
    PBITMAPINFO result = GetBitmapInfo(); // Update pointer after data resize
  }

  // Initialize the fields in the BITMAPINFO structure.  

  result->bmiHeader.biSize = bitmapInfoSize;
  result->bmiHeader.biWidth = width;
  result->bmiHeader.biHeight = height;
  result->bmiHeader.biPlanes = planesCount;
  result->bmiHeader.biBitCount = colorBitCount;
  result->bmiHeader.biClrUsed = (cClrBits < 24) ? (1 << cClrBits) : 0;
  result->bmiHeader.biXPelsPerMeter = 0;
  result->bmiHeader.biYPelsPerMeter = 0;

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
}

#pragma endregion

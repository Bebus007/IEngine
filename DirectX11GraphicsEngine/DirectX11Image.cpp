#include "stdafx.h"
#include "DirectX11Image.h"

#include "DirectX11RenderSystem.h"

CDirectX11Image::CDirectX11Image(CDirectX11RenderSystem * rs) : m_pRenderSystem(rs), m_pTexture(nullptr), m_tempDataBuffer(nullptr)
{
}

CDirectX11Image::CDirectX11Image(ID3D11Texture2D* pSrc, CDirectX11RenderSystem* rs) : CDirectX11Image(rs)
{
  if (!m_pRenderSystem)
    return;

  m_pTexture = m_pRenderSystem->CreateTextureCopyForReading(pSrc);
}

CDirectX11Image::~CDirectX11Image()
{
  if (m_pTexture)
  {
    m_pTexture->Release();
    m_pTexture = nullptr;
  }

  if (m_tempDataBuffer)
  {
    delete[] m_tempDataBuffer;
    m_tempDataBuffer = nullptr;
  }
}

void CDirectX11Image::Destroy() { delete this; }

int CDirectX11Image::GetWidth() const
{
  if (!m_pTexture)
    return 0;

  D3D11_TEXTURE2D_DESC desc;
  m_pTexture->GetDesc(&desc);
  return desc.Width;
}

int CDirectX11Image::GetHeight() const
{
  if (!m_pTexture)
    return 0;

  D3D11_TEXTURE2D_DESC desc;
  m_pTexture->GetDesc(&desc);
  return desc.Height;
}

unsigned int CDirectX11Image::GetColorBitCount() const
{
  if (!m_pTexture)
    return 0;

  D3D11_TEXTURE2D_DESC desc;
  m_pTexture->GetDesc(&desc);
  return CDirectX11RenderSystem::GetColorBitCount(desc.Format);
}

IImage::ImageFormat_e CDirectX11Image::GetFormat() const
{
  D3D11_TEXTURE2D_DESC desc;
  m_pTexture->GetDesc(&desc);
  return GetFormat(desc.Format);
}

void CDirectX11Image::Init(int width, int height, int colorBitCount, const void * data)
{
  if (m_pTexture)
  {
    m_pTexture->Release();
    m_pTexture = nullptr;
  }

  if (m_tempDataBuffer)
  {
    delete[] m_tempDataBuffer;
    m_tempDataBuffer = nullptr;
  }

  if (!m_pRenderSystem)
    return;

  m_pTexture = m_pRenderSystem->CreateTexture(width, height, GetFormat(colorBitCount), data);
}

void CDirectX11Image::HMirror()
{
  BYTE* mirroredData = new BYTE[GetDataSize()];

  BYTE** destRows = new BYTE*[GetHeight()];
  destRows[0] = mirroredData;
  BYTE** srcRows = new BYTE*[GetHeight()];
  srcRows[0] = (BYTE*)GetBits();

  for (int i = 1; i < GetHeight(); i++)
  {
    destRows[i] = destRows[i - 1] + GetWidthBytesCount();
    srcRows[i] = srcRows[i - 1] + GetWidthBytesCount();
  }

  for (int i = 0; i < GetHeight(); i++)
    memcpy(destRows[i], srcRows[GetHeight() - i - 1], GetWidthBytesCount());

  Init(GetWidth(), GetHeight(), GetColorBitCount(), mirroredData);

  delete[] srcRows;
  delete[] destRows;
  delete[] mirroredData;
}

int CDirectX11Image::GetDataSize() const { return GetWidthBytesCount() * GetHeight(); }

const void * CDirectX11Image::GetBits() const
{
  if (m_tempDataBuffer)
  {
    delete[] m_tempDataBuffer;
    m_tempDataBuffer = nullptr;
  }

  if (!m_pRenderSystem)
    return nullptr;

  if (!m_pTexture)
    return nullptr;

  m_tempDataBuffer = m_pRenderSystem->CreateTextureDataCopy(m_pTexture);

  return m_tempDataBuffer;
}

void CDirectX11Image::SetBits(const void* data)
{
  if (m_tempDataBuffer)
  {
    delete[] m_tempDataBuffer;
    m_tempDataBuffer = nullptr;
  }

  if (!m_pRenderSystem)
    return;

  m_pRenderSystem->CopyToTextureData(m_pTexture, data);
}

IImage * CDirectX11Image::CreateRegionCopy(int x, int y, int width, int height) const
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
  IImage* result = new CDirectX11Image(m_pRenderSystem);
  result->Init(width, height, GetColorBitCount(), newData);
  delete[] newData;
  return result;
}

int CDirectX11Image::GetWidthBytesCount() const { return GetWidth() * GetColorBitCount() / 8; }

IImage::ImageFormat_e CDirectX11Image::GetFormat(DXGI_FORMAT fmt)
{
  switch (fmt)
  {
  case DXGI_FORMAT_R8G8B8A8_TYPELESS:
  case DXGI_FORMAT_R8G8B8A8_UNORM:
  case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
  case DXGI_FORMAT_R8G8B8A8_UINT:
  case DXGI_FORMAT_R8G8B8A8_SNORM:
  case DXGI_FORMAT_R8G8B8A8_SINT:
    return ImageFormat_e::IF_R8G8B8A8;
  case DXGI_FORMAT_B8G8R8A8_UNORM:
  case DXGI_FORMAT_B8G8R8X8_UNORM:
  case DXGI_FORMAT_B8G8R8A8_TYPELESS:
  case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
  case DXGI_FORMAT_B8G8R8X8_TYPELESS:
  case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
    return ImageFormat_e::IF_B8G8R8A8;
  case DXGI_FORMAT_R32G32B32A32_TYPELESS:
  case DXGI_FORMAT_R32G32B32A32_FLOAT:
  case DXGI_FORMAT_R32G32B32A32_UINT:
  case DXGI_FORMAT_R32G32B32A32_SINT:
  case DXGI_FORMAT_R32G32B32_TYPELESS:
  case DXGI_FORMAT_R32G32B32_FLOAT:
  case DXGI_FORMAT_R32G32B32_UINT:
  case DXGI_FORMAT_R32G32B32_SINT:
  case DXGI_FORMAT_R16G16B16A16_TYPELESS:
  case DXGI_FORMAT_R16G16B16A16_FLOAT:
  case DXGI_FORMAT_R16G16B16A16_UNORM:
  case DXGI_FORMAT_R16G16B16A16_UINT:
  case DXGI_FORMAT_R16G16B16A16_SNORM:
  case DXGI_FORMAT_R16G16B16A16_SINT:
  case DXGI_FORMAT_R32G32_TYPELESS:
  case DXGI_FORMAT_R32G32_FLOAT:
  case DXGI_FORMAT_R32G32_UINT:
  case DXGI_FORMAT_R32G32_SINT:
  case DXGI_FORMAT_R32G8X24_TYPELESS:
  case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
  case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
  case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
  case DXGI_FORMAT_R10G10B10A2_TYPELESS:
  case DXGI_FORMAT_R10G10B10A2_UNORM:
  case DXGI_FORMAT_R10G10B10A2_UINT:
  case DXGI_FORMAT_R11G11B10_FLOAT:
  case DXGI_FORMAT_R16G16_TYPELESS:
  case DXGI_FORMAT_R16G16_FLOAT:
  case DXGI_FORMAT_R16G16_UNORM:
  case DXGI_FORMAT_R16G16_UINT:
  case DXGI_FORMAT_R16G16_SNORM:
  case DXGI_FORMAT_R16G16_SINT:
  case DXGI_FORMAT_R32_TYPELESS:
  case DXGI_FORMAT_D32_FLOAT:
  case DXGI_FORMAT_R32_FLOAT:
  case DXGI_FORMAT_R32_UINT:
  case DXGI_FORMAT_R32_SINT:
  case DXGI_FORMAT_R24G8_TYPELESS:
  case DXGI_FORMAT_D24_UNORM_S8_UINT:
  case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
  case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
  case DXGI_FORMAT_R8G8_TYPELESS:
  case DXGI_FORMAT_R8G8_UNORM:
  case DXGI_FORMAT_R8G8_UINT:
  case DXGI_FORMAT_R8G8_SNORM:
  case DXGI_FORMAT_R8G8_SINT:
  case DXGI_FORMAT_R16_TYPELESS:
  case DXGI_FORMAT_R16_FLOAT:
  case DXGI_FORMAT_D16_UNORM:
  case DXGI_FORMAT_R16_UNORM:
  case DXGI_FORMAT_R16_UINT:
  case DXGI_FORMAT_R16_SNORM:
  case DXGI_FORMAT_R16_SINT:
  case DXGI_FORMAT_R8_TYPELESS:
  case DXGI_FORMAT_R8_UNORM:
  case DXGI_FORMAT_R8_UINT:
  case DXGI_FORMAT_R8_SNORM:
  case DXGI_FORMAT_R8_SINT:
  case DXGI_FORMAT_A8_UNORM:
  case DXGI_FORMAT_R1_UNORM:
  case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
  case DXGI_FORMAT_R8G8_B8G8_UNORM:
  case DXGI_FORMAT_G8R8_G8B8_UNORM:
  case DXGI_FORMAT_BC1_TYPELESS:
  case DXGI_FORMAT_BC1_UNORM:
  case DXGI_FORMAT_BC1_UNORM_SRGB:
  case DXGI_FORMAT_BC2_TYPELESS:
  case DXGI_FORMAT_BC2_UNORM:
  case DXGI_FORMAT_BC2_UNORM_SRGB:
  case DXGI_FORMAT_BC3_TYPELESS:
  case DXGI_FORMAT_BC3_UNORM:
  case DXGI_FORMAT_BC3_UNORM_SRGB:
  case DXGI_FORMAT_BC4_TYPELESS:
  case DXGI_FORMAT_BC4_UNORM:
  case DXGI_FORMAT_BC4_SNORM:
  case DXGI_FORMAT_BC5_TYPELESS:
  case DXGI_FORMAT_BC5_UNORM:
  case DXGI_FORMAT_BC5_SNORM:
  case DXGI_FORMAT_B5G6R5_UNORM:
  case DXGI_FORMAT_B5G5R5A1_UNORM:
  case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
  case DXGI_FORMAT_BC6H_TYPELESS:
  case DXGI_FORMAT_BC6H_UF16:
  case DXGI_FORMAT_BC6H_SF16:
  case DXGI_FORMAT_BC7_TYPELESS:
  case DXGI_FORMAT_BC7_UNORM:
  case DXGI_FORMAT_BC7_UNORM_SRGB:
  case DXGI_FORMAT_AYUV:
  case DXGI_FORMAT_Y410:
  case DXGI_FORMAT_Y416:
  case DXGI_FORMAT_NV12:
  case DXGI_FORMAT_P010:
  case DXGI_FORMAT_P016:
  case DXGI_FORMAT_420_OPAQUE:
  case DXGI_FORMAT_YUY2:
  case DXGI_FORMAT_Y210:
  case DXGI_FORMAT_Y216:
  case DXGI_FORMAT_NV11:
  case DXGI_FORMAT_AI44:
  case DXGI_FORMAT_IA44:
  case DXGI_FORMAT_P8:
  case DXGI_FORMAT_A8P8:
  case DXGI_FORMAT_B4G4R4A4_UNORM:
  case DXGI_FORMAT_P208:
  case DXGI_FORMAT_V208:
  case DXGI_FORMAT_V408:
  case DXGI_FORMAT_FORCE_UINT:
  case DXGI_FORMAT_UNKNOWN:
  default:
    return ImageFormat_e::IF_UNKNOWN;
  }
}

DXGI_FORMAT CDirectX11Image::GetFormat(int colorBitCount)
{
  switch (colorBitCount)
  {
  case 128:
    return DXGI_FORMAT_R32G32B32A32_UINT;
  case 96:
    return DXGI_FORMAT_R32G32B32_UINT;
  case 64:
    return DXGI_FORMAT_R16G16B16A16_UINT;
  case 32:
    return DXGI_FORMAT_R8G8B8A8_UINT;
  default:
    return DXGI_FORMAT_UNKNOWN;
  }
}

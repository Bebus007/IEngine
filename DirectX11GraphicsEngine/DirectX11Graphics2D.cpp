#include "stdafx.h"
#include "DirectX11Graphics2D.h"

#include "DirectX11ShaderSet.h"
#include "DirectX11RenderSystem.h"

#include <string>

CDirectX11Graphics2D::CDirectX11Graphics2D(CDirectX11RenderSystem * pRenderSystem) : m_pRenderSystem(pRenderSystem), m_pTestShaderSet(nullptr)
{
  InitTestShaderSet();
}

CDirectX11Graphics2D::~CDirectX11Graphics2D()
{
  if (m_pTestShaderSet)
    delete m_pTestShaderSet;
}

void CDirectX11Graphics2D::DrawTriangle(Vertex_t a, Vertex_t b, Vertex_t c)
{
  if (!m_pRenderSystem)
    return;

  const unsigned int vertexCount = 3;
  Vertex_t vertexArray[vertexCount] = { a, b, c };
  ID3D11Buffer* pVertexBuffer = m_pRenderSystem->CreateVertexBuffer(sizeof(Vertex_t) * vertexCount, vertexArray);
  if (!pVertexBuffer)
    return;

  float constantBufferData[] = { (float)GetWidth(), (float)GetHeight(), 0.0f, 0.0f };
  m_pTestShaderSet->FillConstantBuffer(constantBufferData);

  m_pRenderSystem->ApplyShaderSet(m_pTestShaderSet);

  m_pRenderSystem->DrawVertexBuffer(pVertexBuffer, sizeof(Vertex_t), vertexCount);

  pVertexBuffer->Release();
}

void CDirectX11Graphics2D::DrawBitmap(const Vertex_t& pos, const IBitmap* bitmap)
{
  DebugBreak();
}

int CDirectX11Graphics2D::GetWidth() const
{
  if (!m_pRenderSystem)
    return 0;

  return m_pRenderSystem->GetWidth();
}

int CDirectX11Graphics2D::GetHeight() const
{
  if (!m_pRenderSystem)
    return 0;

  return m_pRenderSystem->GetHeight();
}

IFont * CDirectX11Graphics2D::CreateFontInstance(int height, const char * fontname)
{
  return nullptr;
}

void CDirectX11Graphics2D::DrawText(const Vertex_t& pos, IFont* font, const char* text)
{
}

IGraphics2D::Size_t CDirectX11Graphics2D::GetTextSize(IFont * pFont, const char * text)
{
  return Size_t();
}

IBitmap * CDirectX11Graphics2D::CaptureScreen()
{
  return nullptr;
}

IBitmap * CDirectX11Graphics2D::CreateEmptyBitmap()
{
  return nullptr;
}

void CDirectX11Graphics2D::InitTestShaderSet()
{
  static const std::string psEntry = "PS";
  static const std::string vsEntry = "VS";
  static const int version = 4;
  static const std::string shaderText = { "\
cbuffer ConstantBuffer : register(b0)                                                      \r\n\
{                                                                                          \r\n\
  float4 vScreenHalfSize;                                                                  \r\n\
}                                                                                          \r\n\
                                                                                           \r\n\
struct VS_INPUT                                                                            \r\n\
{                                                                                          \r\n\
  float2 Pos : POSITION0;                                                                  \r\n\
};                                                                                         \r\n\
                                                                                           \r\n\
struct PS_INPUT                                                                            \r\n\
{                                                                                          \r\n\
  float4 Pos : SV_POSITION;                                                                \r\n\
};                                                                                         \r\n\
                                                                                           \r\n\
PS_INPUT VS(VS_INPUT input)                                                                \r\n\
{                                                                                          \r\n\
  PS_INPUT output = (PS_INPUT)0;                                                           \r\n\
  output.Pos.x = input.Pos.x / vScreenHalfSize.x;                                          \r\n\
  output.Pos.y = input.Pos.y / vScreenHalfSize.y;                                          \r\n\
  output.Pos.z = 0.5f;                                                                     \r\n\
  output.Pos.w = 1.0f;                                                                     \r\n\
                                                                                           \r\n\
  return output;                                                                           \r\n\
}                                                                                          \r\n\
                                                                                           \r\n\
float4 PS(PS_INPUT input) : SV_Target                                                      \r\n\
{                                                                                          \r\n\
  float4 finalColor = 0;                                                                   \r\n\
  finalColor.r = 1.0f;                                                                     \r\n\
  finalColor.a = 1.0f;                                                                     \r\n\
  return finalColor;                                                                       \r\n\
}                                                                                          \r\n " };

  static const D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };
  static const unsigned int numElementDescs = ARRAYSIZE(inputElementDescs);

  static const float constantBufferData[] = { 320, 240, 0, 0 };
  static const size_t constantBufferSize = 16;

  if (!m_pTestShaderSet)
    m_pTestShaderSet = new CDirectX11ShaderSet;

  m_pTestShaderSet->SetRenderSystem(m_pRenderSystem);

  m_pTestShaderSet->SetShaderText(shaderText, psEntry, vsEntry, version);
  m_pTestShaderSet->SetInputElementDescriptors(inputElementDescs, numElementDescs);
  m_pTestShaderSet->SetConstantBufferSize(constantBufferSize);

  m_pTestShaderSet->Compile();

  m_pTestShaderSet->FillConstantBuffer(constantBufferData);
}

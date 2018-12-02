#include "stdafx.h"
#include "DirectX11ShaderLibrary.h"

#include "DirectX11ShaderSet.h"

CDirectX11ShaderSet * CDirectX11ShaderLibrary::CreateTestShader(CDirectX11RenderSystem* pRenderSystem)
{
  CDirectX11ShaderSet* pTestShaderSet = nullptr;
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
    float4 Pos : POSITION0;                                                                  \r\n\
  };                                                                                         \r\n\
                                                                                             \r\n\
  struct PS_INPUT                                                                            \r\n\
  {                                                                                          \r\n\
    float4 Pos : SV_POSITION;                                                                \r\n\
  };                                                                                         \r\n\
                                                                                             \r\n\
  PS_INPUT VS(VS_INPUT input)                                                                \r\n\
  {                                                                                          \r\n\
    PS_INPUT output = (PS_INPUT)input;                                                       \r\n\
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
    { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };
  static const unsigned int numElementDescs = ARRAYSIZE(inputElementDescs);
  static const size_t constantBufferSize = 16;

  if (!pTestShaderSet)
    pTestShaderSet = new CDirectX11ShaderSet;

  pTestShaderSet->SetRenderSystem(pRenderSystem);

  pTestShaderSet->SetShaderText(shaderText, psEntry, vsEntry, version);
  pTestShaderSet->SetInputElementDescriptors(inputElementDescs, numElementDescs);
  pTestShaderSet->SetConstantBufferSize(constantBufferSize);

  pTestShaderSet->Compile();

  return pTestShaderSet;
}

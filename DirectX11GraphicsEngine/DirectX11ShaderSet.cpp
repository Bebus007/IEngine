#include "stdafx.h"
#include "DirectX11ShaderSet.h"

#include "DirectX11RenderSystem.h"

CDirectX11ShaderSet::CDirectX11ShaderSet() :
  m_pRenderSystem(nullptr),
  m_numElementDescs(0),
  m_inputElementDescs(nullptr),
  m_pPixelShader(nullptr),
  m_pVertexShader(nullptr),
  m_pVertexInputLayout(nullptr),
  m_pConstantBuffer(nullptr),
  m_constantBufferSize(0)
{
}

CDirectX11ShaderSet::CDirectX11ShaderSet(const CDirectX11ShaderSet & shaderSet) : CDirectX11ShaderSet()
{
  SetRenderSystem(shaderSet.GetRenderSystem());
  SetShaderText(shaderSet.GetShaderText(), shaderSet.GetPSEntry(), shaderSet.GetVSEntry(), shaderSet.GetVersion());
  SetInputElementDescriptors(shaderSet.GetInputElementDescriptors(), shaderSet.GetInputElementDescriptorsCount());
}

CDirectX11ShaderSet::~CDirectX11ShaderSet()
{
  DestroyDXObjects();
  SetInputElementDescriptors(nullptr, 0);
}

void CDirectX11ShaderSet::DestroyDXObjects()
{
  SetPixelShader(nullptr);

  SetVertexShader(nullptr);

  SetInputLayout(nullptr);

  SetConstantBuffer(nullptr);
}

CDirectX11RenderSystem * CDirectX11ShaderSet::GetRenderSystem() const { return m_pRenderSystem; }

void CDirectX11ShaderSet::SetRenderSystem(CDirectX11RenderSystem * pRenderSystem)
{
  DestroyDXObjects();
  m_pRenderSystem = pRenderSystem;
}

void CDirectX11ShaderSet::Compile()
{
  DestroyDXObjects();

  if (!m_pRenderSystem)
    return;

  ID3D11PixelShader* pPixelShader = m_pRenderSystem->CompilePixelShader(GetShaderText(), GetPSEntry(), GetVersion());
  SetPixelShader(pPixelShader);

  ID3DBlob* pVertexShaderBlob = m_pRenderSystem->CompileVertexShader(GetShaderText(), GetVSEntry(), GetVersion());

  ID3D11VertexShader* pVertexShader = m_pRenderSystem->CreateVertexShader(pVertexShaderBlob);
  SetVertexShader(pVertexShader);

  ID3D11InputLayout* pInputLayout = m_pRenderSystem->CreateVertexLayout(GetInputElementDescriptors(), GetInputElementDescriptorsCount(), pVertexShaderBlob);
  SetInputLayout(pInputLayout);

  ID3D11Buffer* pConstantBuffer = m_pRenderSystem->CreateConstantBuffer(m_constantBufferSize);
  SetConstantBuffer(pConstantBuffer);

  if (!IsCompiled())
    DestroyDXObjects();
}

bool CDirectX11ShaderSet::IsCompiled() const { return GetPixelShader() && GetVertexShader() && GetInputLayout() && GetConstantBuffer(); }

void CDirectX11ShaderSet::SetShaderText(const std::string& text, const std::string& PSEntry, const std::string& VSEntry, int version)
{
  DestroyDXObjects();

  m_shaderText = text;
  m_PSEntry = PSEntry;
  m_VSEntry = VSEntry;
  m_version = version;
}

const std::string & CDirectX11ShaderSet::GetShaderText() const { return m_shaderText; }

const std::string & CDirectX11ShaderSet::GetPSEntry() const { return m_PSEntry; }

const std::string & CDirectX11ShaderSet::GetVSEntry() const { return m_VSEntry; }

int CDirectX11ShaderSet::GetVersion() const { return m_version; }

void CDirectX11ShaderSet::SetInputElementDescriptors(const D3D11_INPUT_ELEMENT_DESC * inputElementDescs, const int numElementDescs)
{
  if (m_inputElementDescs)
  {
    delete[] m_inputElementDescs;
    m_inputElementDescs = nullptr;
    m_numElementDescs = 0;
  }

  if (numElementDescs == 0 || inputElementDescs == nullptr)
    return;

  m_inputElementDescs = new D3D11_INPUT_ELEMENT_DESC[numElementDescs];
  for (int i = 0; i < numElementDescs; i++)
    m_inputElementDescs[i] = inputElementDescs[i];

  m_numElementDescs = numElementDescs;
}

int CDirectX11ShaderSet::GetInputElementDescriptorsCount() const { return m_numElementDescs; }

const D3D11_INPUT_ELEMENT_DESC * CDirectX11ShaderSet::GetInputElementDescriptors() const { return m_inputElementDescs; }

void CDirectX11ShaderSet::SetConstantBufferSize(size_t size)
{
  if (m_constantBufferSize == size)
    return;

  SetConstantBuffer(nullptr);
  m_constantBufferSize = size;
}

ID3D11PixelShader * CDirectX11ShaderSet::GetPixelShader() const { return m_pPixelShader; }

void CDirectX11ShaderSet::SetPixelShader(ID3D11PixelShader * pPixelShader)
{
  if (m_pPixelShader)
    m_pPixelShader->Release();

  m_pPixelShader = pPixelShader;
}

ID3D11VertexShader * CDirectX11ShaderSet::GetVertexShader() const { return m_pVertexShader; }

void CDirectX11ShaderSet::SetVertexShader(ID3D11VertexShader * pVertexShader)
{
  if (m_pVertexShader)
    m_pVertexShader->Release();

  m_pVertexShader = pVertexShader;
}

ID3D11InputLayout * CDirectX11ShaderSet::GetInputLayout() const { return m_pVertexInputLayout; }

void CDirectX11ShaderSet::SetInputLayout(ID3D11InputLayout * pInputLayout)
{
  if (m_pVertexInputLayout)
    m_pVertexInputLayout->Release();

  m_pVertexInputLayout = pInputLayout;
}

ID3D11Buffer * CDirectX11ShaderSet::GetConstantBuffer() const { return m_pConstantBuffer; }

void CDirectX11ShaderSet::SetConstantBuffer(ID3D11Buffer * pConstantBuffer)
{
  if (m_pConstantBuffer)
    m_pConstantBuffer->Release();

  m_pConstantBuffer = pConstantBuffer;
}

void CDirectX11ShaderSet::FillConstantBuffer(const void * pData)
{
  m_pRenderSystem->FillBuffer(GetConstantBuffer(), pData);
}

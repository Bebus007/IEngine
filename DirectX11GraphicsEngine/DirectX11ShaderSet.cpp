#include "stdafx.h"
#include "DirectX11ShaderSet.h"

CDirectX11ShaderSet::CDirectX11ShaderSet() :
  m_pRenderSystem(nullptr),
  m_numElementDescs(0),
  m_inputElementDescs(nullptr),
  m_pPixelShader(nullptr),
  m_pVertexShader(nullptr),
  m_pVertexInputLayout(nullptr),
  m_pConstantBuffer(nullptr)
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

CDirextX11RenderSystem * CDirectX11ShaderSet::GetRenderSystem() const { return m_pRenderSystem; }

void CDirectX11ShaderSet::SetRenderSystem(CDirextX11RenderSystem * pRenderSystem)
{
  DestroyDXObjects();
  m_pRenderSystem = pRenderSystem;
}

void CDirectX11ShaderSet::Compile()
{
  DestroyDXObjects();

  // TODO: compilation code here

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
}

int CDirectX11ShaderSet::GetInputElementDescriptorsCount() const { return m_numElementDescs; }

const D3D11_INPUT_ELEMENT_DESC * CDirectX11ShaderSet::GetInputElementDescriptors() const { return m_inputElementDescs; }

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

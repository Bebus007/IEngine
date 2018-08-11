#pragma once

#include <string>

class CDirextX11RenderSystem;

class CDirectX11ShaderSet
{
public:
  CDirectX11ShaderSet();
  CDirectX11ShaderSet(const CDirectX11ShaderSet&);
  ~CDirectX11ShaderSet();

  void DestroyDXObjects();

  CDirextX11RenderSystem* GetRenderSystem() const;
  void SetRenderSystem(CDirextX11RenderSystem*);

  void Compile();
  bool IsCompiled() const;

  void SetShaderText(const std::string& text, const std::string& PSEntry, const std::string& VSEntry, int version);
  const std::string& GetShaderText() const;
  const std::string& GetPSEntry() const;
  const std::string& GetVSEntry() const;
  int GetVersion() const;

  void SetInputElementDescriptors(const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, const int numElementDescs);
  int GetInputElementDescriptorsCount() const;
  const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescriptors() const;

  ID3D11PixelShader* GetPixelShader() const;
  void SetPixelShader(ID3D11PixelShader*);

  ID3D11VertexShader* GetVertexShader() const;
  void SetVertexShader(ID3D11VertexShader*);

  ID3D11InputLayout* GetInputLayout() const;
  void SetInputLayout(ID3D11InputLayout*);

  ID3D11Buffer* GetConstantBuffer() const;
  void SetConstantBuffer(ID3D11Buffer*);

private:
  CDirextX11RenderSystem* m_pRenderSystem;

  std::string m_shaderText;
  std::string m_PSEntry;
  std::string m_VSEntry;
  int m_version;
  unsigned int m_numElementDescs;
  D3D11_INPUT_ELEMENT_DESC* m_inputElementDescs;

  ID3D11PixelShader* m_pPixelShader;
  ID3D11VertexShader* m_pVertexShader;
  ID3D11InputLayout* m_pVertexInputLayout;
  ID3D11Buffer* m_pConstantBuffer;
};
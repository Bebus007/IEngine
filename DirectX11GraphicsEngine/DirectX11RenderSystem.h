#pragma once

#include <string>

class IWindowEx;

class CDirectX11ShaderSet;

class CDirectX11RenderSystem
{
public:
  CDirectX11RenderSystem(IWindowEx* pWnd);

  bool Init(int width, int height, bool fullscreen);
  void Cleanup();

  bool ResizeSwapChain(int width, int height);
  int GetWidth() const;
  int GetHeight() const;

  void ClearScreen(float r, float g, float b, float a = 1.0f);
  void ClearScreen(const float* color);

  void SwapBuffers();

  ID3D11PixelShader* CompilePixelShader(const std::string& text, const std::string& PSEntry, int version);
  ID3DBlob* CompileVertexShader(const std::string& text, const std::string& VSEntry, int version);
  ID3D11VertexShader* CreateVertexShader(ID3DBlob*);
  ID3D11InputLayout* CreateVertexLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, unsigned int NumElements, ID3DBlob*);
  ID3D11Buffer* CreateConstantBuffer(unsigned int bufferSize);
  void FillBuffer(ID3D11Buffer*, const void* data);

  ID3D11Buffer* CreateVertexBuffer(unsigned int bufferSize, void* bufferData);
  void DrawVertexBuffer(ID3D11Buffer* pBuff, UINT vertexSize, UINT vertexCount, D3D11_PRIMITIVE_TOPOLOGY topologyType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  ID3D11Texture2D* CreateTexture(int width, int height, const DXGI_FORMAT& fmt, const void * data = nullptr);
  static unsigned int GetColorBitCount(const DXGI_FORMAT&);
  void* CreateTextureDataCopy(ID3D11Texture2D*);
  void CopyToTextureData(ID3D11Texture2D*, const void*);

  ID3D11Texture2D* GetBackBufferCopyForReading();
  ID3D11Texture2D* CreateTextureCopyForReading(ID3D11Texture2D*);

  void ApplyShaderSet(CDirectX11ShaderSet*);
private:

  ID3DBlob * CompileShader(const std::string& pShaderText, const std::string& pEntrypoint, const std::string& versionString);

  IWindowEx * m_pWindow;

  ID3D11Device           *m_pd3dDevice;          // ���������� (��� �������� ��������)
  ID3D11DeviceContext    *m_pImmediateContext;   // �������� ���������� (���������)
  IDXGISwapChain         *m_pSwapChain;          // ���� ����� (������ � �������)
  ID3D11RenderTargetView *m_pRenderTargetView;   // ������ ������� ������

  ID3D11Texture2D        *m_pDepthStencil;       // �������� ������ ������
  ID3D11DepthStencilView *m_pDepthStencilView;   // ������ ����, ����� ������

  bool CreateDeviceAndSwapChain(int width, int height, bool fullscreen);
  void SetupViewport(float width, float height);
  bool CreateBackBuffer();
  bool CreateDepthStencilBuffer(unsigned int width, unsigned int height);
};
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

  void ClearScreen(float r, float g, float b, float a = 1.0f);
  void ClearScreen(const float* color);

  void SwapBuffers();

  ID3D11PixelShader* CompilePixelShader(const std::string& text, const std::string& PSEntry, int version);
  ID3DBlob* CompileVertexShader(const std::string& text, const std::string& VSEntry, int version);
  ID3D11VertexShader* CreateVertexShader(ID3DBlob*);
  ID3D11InputLayout* CreateVertexLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, unsigned int NumElements, ID3DBlob*);
  ID3D11Buffer* CreateConstantBuffer(unsigned int bufferSize);
  void FillBuffer(ID3D11Buffer*, const void* data);

  void ApplyShaderSet(CDirectX11ShaderSet*);
private:

  ID3DBlob * CompileShader(const std::string& pShaderText, const std::string& pEntrypoint, const std::string& versionString);

  IWindowEx * m_pWindow;

  ID3D11Device           *m_pd3dDevice;          // Устройство (для создания объектов)
  ID3D11DeviceContext    *m_pImmediateContext;   // Контекст устройства (рисование)
  IDXGISwapChain         *m_pSwapChain;          // Цепь связи (буфера с экраном)
  ID3D11RenderTargetView *m_pRenderTargetView;   // Объект заднего буфера

  ID3D11Texture2D        *m_pDepthStencil;       // Текстура буфера глубин
  ID3D11DepthStencilView *m_pDepthStencilView;   // Объект вида, буфер глубин

  bool CreateDeviceAndSwapChain(int width, int height, bool fullscreen);
  void SetupViewport(float width, float height);
  bool CreateBackBuffer();
  bool CreateDepthStencilBuffer(unsigned int width, unsigned int height);
};
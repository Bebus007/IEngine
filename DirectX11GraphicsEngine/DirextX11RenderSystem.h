#pragma once

//#include <d3d11.h>
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

struct ID3D11PixelShader;
struct ID3D11VertexShader;
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
typedef struct ID3D10Blob ID3DBlob;
struct ID3D11Buffer;

class IWindowEx;

class CDirextX11RenderSystem
{
public:
  CDirextX11RenderSystem(IWindowEx* pWnd);

  bool Init(int width, int height, bool fullscreen);
  void Cleanup();

  void ClearScreen(float r, float g, float b, float a = 1.0f);
  void ClearScreen(const float* color);

  void SwapBuffers();

  ID3D11PixelShader* CompilePixelShader(const char* pShaderText, unsigned long shaderTextLength);
  ID3DBlob* CompileVertexShader(const char* pShaderText, unsigned long shaderTextLength);
  ID3D11VertexShader* CreateVertexShader(ID3DBlob*);
  ID3D11InputLayout* CreateVertexLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, unsigned int NumElements, ID3DBlob*);
  ID3D11Buffer* CreateConstantBuffer(unsigned int bufferSize);
private:

  ID3DBlob * CompileShader(const char* pShaderText, unsigned long shaderTextLength, const char* pEntrypoint, const char* pTargetVersion);

  IWindowEx * m_pWindow;

  ID3D11Device * m_pd3dDevice;          // Устройство (для создания объектов)
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
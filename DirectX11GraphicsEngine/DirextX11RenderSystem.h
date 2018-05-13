#pragma once

//#include <d3d11.h>
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

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
private:

  IWindowEx * m_pWindow;

  ID3D11Device * m_pd3dDevice;          // ���������� (��� �������� ��������)
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
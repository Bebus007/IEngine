#pragma once

#include "IGraphicsEngine.h"

class IWindowEx;

// d3d11.h
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

class CDirectX11GraphicsEngine : public IGraphicsEngine
{
public:
  CDirectX11GraphicsEngine(IWindowEx*);
  ~CDirectX11GraphicsEngine();

  virtual bool Init(int width = 640, int height = 480, bool fullscreen = false) override;
  virtual void SetClearColor(float r, float g, float b, float a = 1.0f) override;
  virtual void ClearScreen() override;
  virtual void Swap() override;

private:
  void Cleanup();

  IWindowEx * m_pWindow;

  float m_ClearColor[4];

  ID3D11Device           *m_pd3dDevice;          // ���������� (��� �������� ��������)
  ID3D11DeviceContext    *m_pImmediateContext;   // �������� ���������� (���������)
  IDXGISwapChain         *m_pSwapChain;          // ���� ����� (������ � �������)
  ID3D11RenderTargetView *m_pRenderTargetView;   // ������ ������� ������

  ID3D11Texture2D        *m_pDepthStencil;       // �������� ������ ������
  ID3D11DepthStencilView *m_pDepthStencilView;   // ������ ����, ����� ������
};
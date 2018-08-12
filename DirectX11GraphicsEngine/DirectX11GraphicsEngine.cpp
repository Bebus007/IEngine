#include "stdafx.h"
#include "DirectX11GraphicsEngine.h"

#include "DirectX11Graphics2D.h"

#include "DirectX11RenderSystem.h"

#include "IWindowEx.h"

CDirectX11GraphicsEngine::CDirectX11GraphicsEngine(IWindowEx * pWnd) :
  m_p2DInterface(nullptr),
  m_pRenderSystem(new CDirectX11RenderSystem(pWnd)),
  m_pWindow(pWnd)
{
  if (GetWindow())
    GetWindow()->AddWindowResizeEventListener(this);
}

CDirectX11GraphicsEngine::~CDirectX11GraphicsEngine()
{
  if (GetWindow())
    GetWindow()->RemoveWindowResizeEventListener(this);

  Cleanup();

  if (m_pRenderSystem)
    delete m_pRenderSystem;
}

bool CDirectX11GraphicsEngine::Init(int width, int height, bool fullscreen)
{
  Cleanup();

  if (!m_pRenderSystem->Init(width, height, fullscreen))
    return false;

  m_p2DInterface = new CDirectX11Graphics2D(m_pRenderSystem);

  SetClearColor(0.0f, 0.0f, 0.25f);

  return true;
}

void CDirectX11GraphicsEngine::SetClearColor(float r, float g, float b, float a)
{
  m_ClearColor[0] = r;
  m_ClearColor[1] = g;
  m_ClearColor[2] = b;
  m_ClearColor[3] = a;
}

void CDirectX11GraphicsEngine::ClearScreen()
{
  if (m_pRenderSystem)
    m_pRenderSystem->ClearScreen(m_ClearColor);
}

void CDirectX11GraphicsEngine::Swap()
{
  if (m_pRenderSystem)
    m_pRenderSystem->SwapBuffers();
}

IGraphics2D * CDirectX11GraphicsEngine::Get2DInterface() { return m_p2DInterface; }

void CDirectX11GraphicsEngine::HandleWindowResize(int newW, int newH)
{
  m_pRenderSystem->ResizeSwapChain(newW, newH);
}

IWindowEx * CDirectX11GraphicsEngine::GetWindow() const { return m_pWindow; }

void CDirectX11GraphicsEngine::Cleanup()
{
  if (m_p2DInterface)
    delete m_p2DInterface;

  if (m_pRenderSystem)
    m_pRenderSystem->Cleanup();
}

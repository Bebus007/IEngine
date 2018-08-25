#include "stdafx.h"

#include "GDIGraphicsEngine.h"
#include "GDIGraphics2D.h"

#include "IWindow.h"
#include "IWindowEx.h"

CGDIGraphicsEngine::CGDIGraphicsEngine(IWindow* pWindow) : m_pWindow(pWindow), m_pGraphics2D(nullptr), m_hDC(nullptr)
{
  if (m_pWindow)
  {
    IWindowEx* pWindow = dynamic_cast<IWindowEx*>(m_pWindow);
    if (pWindow)
    {
      HWND hWnd = (HWND)pWindow->GetHandle();
      if (hWnd)
        m_hDC = GetDC(hWnd);
    }
  }

  if (m_hDC)
    m_pGraphics2D = new CGDIGraphics2D(m_hDC);
}

CGDIGraphicsEngine::~CGDIGraphicsEngine()
{
  if (m_hDC)
  {
    IWindowEx* pWindow = dynamic_cast<IWindowEx*>(m_pWindow);
    HWND hWnd = (HWND)pWindow->GetHandle();
    ReleaseDC(hWnd, m_hDC);
    hWnd = nullptr;
  }

  if (m_pGraphics2D)
  {
    delete m_pGraphics2D;
    m_pGraphics2D = nullptr;
  }
}

bool CGDIGraphicsEngine::Init(int width, int height, bool fullscreen)
{
  if (!m_pWindow)
    return false;

  m_pWindow->SetClientSize(width, height);

  return true;
}

void CGDIGraphicsEngine::SetClearColor(float r, float g, float b, float a)
{
  m_ClearColor[0] = r;
  m_ClearColor[1] = g;
  m_ClearColor[2] = b;
  m_ClearColor[3] = a;
}

void CGDIGraphicsEngine::ClearScreen()
{
}

void CGDIGraphicsEngine::Swap()
{
}

void CGDIGraphicsEngine::Destroy()
{
  delete this;
}

IGraphics2D * CGDIGraphicsEngine::Get2DInterface()
{
  return m_pGraphics2D;
}

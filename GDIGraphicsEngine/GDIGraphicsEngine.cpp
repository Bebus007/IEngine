#include "stdafx.h"

#include "GDIGraphicsEngine.h"

#include "IWindow.h"

CGDIGraphicsEngine::CGDIGraphicsEngine(IWindow* pWindow) : m_pWindow(pWindow)
{
}

CGDIGraphicsEngine::~CGDIGraphicsEngine()
{
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
  return nullptr;
}

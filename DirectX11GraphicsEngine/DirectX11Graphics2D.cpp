#include "stdafx.h"
#include "DirectX11Graphics2D.h"

CDirectX11Graphics2D::CDirectX11Graphics2D(CDirextX11RenderSystem * pRenderSystem) : m_pRenderSystem(pRenderSystem), m_pTestPixelShader(nullptr)
{
  InitTestShaders();
}

void CDirectX11Graphics2D::DrawTriangle(Vertex a, Vertex b, Vertex c)
{
}

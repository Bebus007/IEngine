#pragma once

#include "IGraphics2D.h"

class IShader;

class CGraphics2DDirectX11 : public IGraphics2D
{
public:

  virtual void DrawTriangle(Vertex a, Vertex b, Vertex c) override;
private:
};
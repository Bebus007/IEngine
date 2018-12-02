#pragma once

#include "..\IEngineObject.h"

class IGraphics2D;

class IGraphicsEngine : public IEngineObject
{
public:
  struct Vertex_t
  {
    Vertex_t() : X(0), Y(0), Z(0), W(0) {}
    Vertex_t(float x) : X(x), Y(0), Z(0), W(0) {}
    Vertex_t(float x, float y) : X(x), Y(y), Z(0), W(0) {}
    Vertex_t(float x, float y, float z) : X(x), Y(y), Z(z), W(0) {}
    Vertex_t(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
    Vertex_t(const Vertex_t& v) : X(v.X), Y(v.Y), Z(v.Z), W(v.W) {}

    float
      X,
      Y,
      Z,
      W;
  };

  virtual bool Init(int width = 640, int height = 480, bool fullscreen = false) = 0;
  virtual void SetClearColor(float r, float g, float b, float a = 1.0f) = 0;
  virtual void ClearScreen() = 0;
  virtual void Swap() = 0;
  virtual void DrawTriangle(Vertex_t a, Vertex_t b, Vertex_t c) = 0;

  virtual IGraphics2D* Get2DInterface() = 0;
};
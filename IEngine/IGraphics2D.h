#pragma once

class IGraphics2D
{
public:
  struct Vertex
  {
    Vertex() : X(0), Y(0) {}
    Vertex(float x, float y) : X(x), Y(y) {}
    Vertex(const Vertex& v) : X(v.X), Y(v.Y) {}
    float X;
    float Y;
  };

  virtual void DrawTriangle(Vertex a, Vertex b, Vertex c) = 0;
};
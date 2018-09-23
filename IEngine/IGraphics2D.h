#pragma once

class IFont;
class IImage;

class IGraphics2D
{
public:
  struct Vertex_t
  {
    Vertex_t() : X(0), Y(0) {}
    Vertex_t(float x, float y) : X(x), Y(y) {}
    Vertex_t(const Vertex_t& v) : X(v.X), Y(v.Y) {}
    float X;
    float Y;
  };

  typedef Vertex_t Size_t;

  virtual void DrawTriangle(Vertex_t a, Vertex_t b, Vertex_t c) = 0;
  virtual void DrawBitmap(const Vertex_t& pos, const IImage* bitmap) = 0;

  virtual int GetWidth() const = 0;
  virtual int GetHeight() const = 0;

  virtual IFont* CreateFontInstance(int height, const char* fontname) = 0;
  virtual void DrawText(const Vertex_t& pos, IFont* font, const char* text) = 0;
  virtual Size_t GetTextSize(IFont* pFont, const char* text) = 0;

  virtual IImage* CaptureScreen() = 0;
  virtual IImage* CreateEmptyBitmap() = 0;
};
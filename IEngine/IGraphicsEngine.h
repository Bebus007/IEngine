#pragma once

class IGraphics2D;

class IGraphicsEngine
{
public:
  virtual bool Init(int width = 640, int height = 480, bool fullscreen = false) = 0;
  virtual void SetClearColor(float r, float g, float b, float a = 1.0f) = 0;
  virtual void ClearScreen() = 0;
  virtual void Swap() = 0;

  virtual IGraphics2D* Get2DInterface() = 0;
};
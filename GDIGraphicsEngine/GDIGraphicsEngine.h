#pragma once

#include "IGraphicsEngine.h"

class CGDIGraphicsEngine : public IGraphicsEngine
{
public:
  virtual bool Init(int width = 640, int height = 480, bool fullscreen = false) override;
  virtual void SetClearColor(float r, float g, float b, float a = 1.0f) override;
  virtual void ClearScreen() override;
  virtual void Swap() override;

  virtual IGraphics2D* Get2DInterface() override;
};
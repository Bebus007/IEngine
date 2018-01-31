#pragma once

class IWindow
{
public:
  virtual bool IsVisible() = 0;
  virtual void Close() = 0;
  virtual void Hide() = 0;

  virtual void SetPosition(int x, int y) = 0;
  virtual void SetClientSize(int w, int h) = 0;

  virtual bool Idle() = 0;
};
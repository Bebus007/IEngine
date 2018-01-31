#pragma once

class IWindow;

class IOperatingSystemEngine
{
public:
  virtual IWindow* GetConsoleWindow() = 0;
  virtual IWindow* GetWindowInstance() = 0;
};
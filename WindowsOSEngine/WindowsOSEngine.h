#pragma once

#include "IOperatingSystemEngine.h"

#include <Windows.h>

class CWindowsOSEngine : public IOperatingSystemEngine
{
public:
  CWindowsOSEngine();
  ~CWindowsOSEngine();

  virtual IWindow* GetConsoleWindow() override;
  virtual IWindow* GetWindowInstance() override;

private:
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  static IWindow * m_pWindowInstance;
};
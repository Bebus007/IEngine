#pragma once

#include "IGamepad.h"
#include "DirectInputDevice.h"

class IWindowEx;

class CDirectInputGamepad : public IGamepad, public CDirectInputDevice
{
public:
  CDirectInputGamepad(HWND hWnd, LPDIRECTINPUTDEVICE8 device);
  ~CDirectInputGamepad();

  // Унаследовано через IGamepad
  virtual bool IsButtonDown(int buttonIdx) override;

private:
  void Update();

  DIJOYSTATE2 *m_State;
};

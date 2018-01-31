#pragma once

#include "IKeyboard.h"
#include "DirectInputDevice.h"

class IWindowEx;

class CDirectInputKeyboard : public CDirectInputDevice, public IKeyboard
{
public:
  CDirectInputKeyboard(HWND hWnd, LPDIRECTINPUTDEVICE8 device);
  ~CDirectInputKeyboard();

  virtual bool IsKeyDown(Key_e) override;

private:
  int GetDirectInputKeyCode(Key_e key);

  void Update();

  char* m_State;
};
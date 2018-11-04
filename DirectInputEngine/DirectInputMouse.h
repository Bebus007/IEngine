#pragma once

#include "IMouse.h"

#include "DirectInputDevice.h"

class CDirectInputMouse : public IMouse, public CDirectInputDevice
{
public:
  CDirectInputMouse(HWND hWnd, LPDIRECTINPUTDEVICE8 device);
  ~CDirectInputMouse();

  virtual bool IsButtonDown(Button_e) override;
  virtual int GetRelativeAxisPosition(Axis_e) override;

private:
  void Update();

  static int GetDirectInputButtonCode(Button_e btn);

  DIMOUSESTATE2* m_State;
  long* m_AxesSummaryState;
};
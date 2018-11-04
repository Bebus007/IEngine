#include "stdafx.h"
#include "DirectInputGamepad.h"

CDirectInputGamepad::CDirectInputGamepad(HWND hWnd, LPDIRECTINPUTDEVICE8 device) : CDirectInputDevice(hWnd, device)
{
  HRESULT hr = S_OK;
  hr = GetInputDevice()->SetDataFormat(&c_dfDIJoystick2);

  m_State = new DIJOYSTATE2;

  SetStateContainer(m_State, sizeof(DIJOYSTATE2));
}

CDirectInputGamepad::~CDirectInputGamepad()
{
  delete m_State;

  SetStateContainer(nullptr, 0);
}

bool CDirectInputGamepad::IsButtonDown(int buttonIdx)
{
  if (buttonIdx < 0 || buttonIdx > 127)
    return false;

  Update();

  return m_State->rgbButtons[buttonIdx] != 0;
}

void CDirectInputGamepad::Update()
{
  UpdateStateContainer();
}

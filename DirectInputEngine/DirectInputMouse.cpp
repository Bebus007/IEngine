#include "stdafx.h"
#include "DirectInputMouse.h"

CDirectInputMouse::CDirectInputMouse(HWND hWnd, LPDIRECTINPUTDEVICE8 device) : CDirectInputDevice(hWnd, device)
{
  HRESULT hr = S_OK;
  hr = GetInputDevice()->SetDataFormat(&c_dfDIMouse2);

  m_State = new DIMOUSESTATE2;
  m_AxesSummaryState = new long[3];
  m_AxesSummaryState[0] = 0;
  m_AxesSummaryState[1] = 0;
  m_AxesSummaryState[2] = 0;

  SetStateContainer(m_State, sizeof(DIMOUSESTATE2));

  ClearState();
}

CDirectInputMouse::~CDirectInputMouse()
{
  delete m_State;
  delete[] m_AxesSummaryState;

  SetStateContainer(nullptr, 0);
}

bool CDirectInputMouse::IsButtonDown(Button_e btn)
{
  Update();

  int btnCode = GetDirectInputButtonCode(btn);
  if (btnCode < 0 || btnCode > 7)
    return false;

  return m_State->rgbButtons[btnCode] != 0;
}

int CDirectInputMouse::GetRelativeAxisPosition(Axis_e ax)
{
  Update();

  switch (ax)
  {
  case IMouse::AX_X:
    return m_AxesSummaryState[0];
  case IMouse::AX_Y:
    return m_AxesSummaryState[1];
  case IMouse::AX_Z:
    return m_AxesSummaryState[2];
  default:
    return 0;
  }
}

void CDirectInputMouse::Update()
{
  UpdateStateContainer();

  m_AxesSummaryState[0] += m_State->lX;
  m_AxesSummaryState[1] += m_State->lY;
  m_AxesSummaryState[2] += m_State->lZ;
}

int CDirectInputMouse::GetDirectInputButtonCode(Button_e btn)
{
  switch (btn)
  {
  case IMouse::BTN_0:
    return 0;
  case IMouse::BTN_1:
    return 1;
  case IMouse::BTN_2:
    return 2;
  case IMouse::BTN_3:
    return 3;
  case IMouse::BTN_4:
    return 4;
  case IMouse::BTN_5:
    return 5;
  case IMouse::BTN_6:
    return 6;
  case IMouse::BTN_7:
    return 7;
  default:
    return -1;
  }
}

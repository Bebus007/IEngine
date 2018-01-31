#include "stdafx.h"
#include "DirectInputDevice.h"

CDirectInputDevice::CDirectInputDevice(HWND hWnd, LPDIRECTINPUTDEVICE8 device) :
  m_hWnd(hWnd),
  m_pStateContainer(nullptr),
  m_StateContainerSize(0)
{
  SetInputDevice(device);
  GetInputDevice()->SetCooperativeLevel(
    m_hWnd,
    DISCL_FOREGROUND | DISCL_EXCLUSIVE
  );
  GetInputDevice()->Acquire();
}

CDirectInputDevice::~CDirectInputDevice()
{
  GetInputDevice()->Unacquire();
  GetInputDevice()->Release();
}

void CDirectInputDevice::UpdateStateContainer()
{
  HRESULT hr;

  hr = GetInputDevice()->GetDeviceState(
    m_StateContainerSize,
    m_pStateContainer
  );
  if (FAILED(hr))
  {
    // It's possible that we lost access to the device
    // Here we acquire access to the keyboard again
    GetInputDevice()->Acquire();
    return;
  }
}

void CDirectInputDevice::SetStateContainer(void * stateContainer, int stateContainerSize)
{
  m_StateContainerSize = stateContainerSize;
  m_pStateContainer = stateContainer;
}

void CDirectInputDevice::ClearState()
{
  memset(m_pStateContainer, 0, m_StateContainerSize);
}

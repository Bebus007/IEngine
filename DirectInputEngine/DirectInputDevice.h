#pragma once

#include <Windows.h>
#include <dinput.h>

class CDirectInputDevice
{
public:
  CDirectInputDevice(HWND hWnd, LPDIRECTINPUTDEVICE8 device);
  ~CDirectInputDevice();

protected:
  void UpdateStateContainer();
  void SetStateContainer(void* stateContainer, int stateContainerSize);

  void SetInputDevice(const LPDIRECTINPUTDEVICE8 device) { m_pDevice = device; }
  const LPDIRECTINPUTDEVICE8 GetInputDevice() { return m_pDevice; }

  int m_StateContainerSize;
  void *m_pStateContainer;

  void ClearState();

private:
  LPDIRECTINPUTDEVICE8 m_pDevice;
  HWND m_hWnd;
};
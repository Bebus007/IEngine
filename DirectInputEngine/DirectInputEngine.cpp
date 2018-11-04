#include "stdafx.h"
#include "DirectInputEngine.h"

#include "DirectInputKeyboard.h"
#include "DirectInputMouse.h"
#include "DirectInputGamepad.h"

#include "IWindowEx.h"

CDirectInputEngine::CDirectInputEngine(const IWindowEx * pWnd) : m_pWindow(pWnd), m_pKeyboard(nullptr), m_pMouse(nullptr), m_pGamepad(nullptr), m_pInterface(nullptr)
{
  // Create the abstract DirectInput connection
  DirectInput8Create(
    GetModuleHandle(NULL),
    DIRECTINPUT_VERSION,
    IID_IDirectInput8,
    (void**)&m_pInterface,
    NULL
  );
}

CDirectInputEngine::~CDirectInputEngine()
{
  if (m_pKeyboard)
  {
    delete m_pKeyboard;
    m_pKeyboard = nullptr;
  }

  if (m_pMouse)
  {
    delete m_pMouse;
    m_pMouse = nullptr;
  }

  if (m_pGamepad)
  {
    delete m_pGamepad;
    m_pGamepad = nullptr;
  }
}

IKeyboard * CDirectInputEngine::GetKeyboard()
{
  if (!m_pKeyboard)
  {
    if (!m_pWindow || !m_pInterface)
      return nullptr;

    HWND hWnd = static_cast<HWND>(m_pWindow->GetHandle());
    if (!hWnd)
      return nullptr;

    LPDIRECTINPUTDEVICE8 pDevice = nullptr;
    m_pInterface->CreateDevice(GUID_SysKeyboard, &pDevice, nullptr);
    if (!pDevice)
      return nullptr;

    m_pKeyboard = new CDirectInputKeyboard(hWnd, pDevice);
  }
  
  return dynamic_cast<IKeyboard*>(m_pKeyboard);
}

IMouse * CDirectInputEngine::GetMouse()
{
  if (!m_pMouse)
  {
    if (!m_pWindow || !m_pInterface)
      return nullptr;

    HWND hWnd = static_cast<HWND>(m_pWindow->GetHandle());
    if (!hWnd)
      return nullptr;

    LPDIRECTINPUTDEVICE8 mouseDevice = nullptr;
    HRESULT hr = m_pInterface->CreateDevice(GUID_SysMouse, &mouseDevice, NULL);
    if (FAILED(hr) || !mouseDevice)
      return nullptr;

    m_pMouse = new CDirectInputMouse(hWnd, mouseDevice);
  }

  return dynamic_cast<IMouse*>(m_pMouse);
}

IGamepad * CDirectInputEngine::GetGamepad()
{
  if (!m_pGamepad)
  {
    if (!m_pWindow || !m_pInterface)
      return nullptr;

    HWND hWnd = static_cast<HWND>(m_pWindow->GetHandle());
    if (!hWnd)
      return nullptr;

    LPDIRECTINPUTDEVICE8 gamepadDevice = nullptr;
    HRESULT hr = m_pInterface->CreateDevice(GUID_Joystick, &gamepadDevice, nullptr);
    if (!gamepadDevice)
      return nullptr;

    m_pGamepad = new CDirectInputGamepad(hWnd, gamepadDevice);
  }

  return dynamic_cast<IGamepad*>(m_pGamepad);
}

void CDirectInputEngine::Destroy()
{
  delete this;
}

#pragma once

#include "IInputEngine.h"

#include <dinput.h>

class IWindowEx;

class CDirectInputKeyboard;
class CDirectInputMouse;
class CDirectInputGamepad;

class CDirectInputEngine : public IInputEngine
{
public:
  CDirectInputEngine(const IWindowEx*);
  ~CDirectInputEngine();

  virtual IKeyboard* GetKeyboard() override;
  virtual IMouse* GetMouse() override;
  virtual IGamepad* GetGamepad() override;

  virtual void Destroy() override;

private:
  const IWindowEx* m_pWindow;

  CDirectInputKeyboard* m_pKeyboard;
  CDirectInputMouse* m_pMouse;
  CDirectInputGamepad* m_pGamepad;

private:
  LPDIRECTINPUT8 m_pInterface;
};
#pragma once

class IKeyboard;
class IMouse;
class IGamepad;

class IInputEngine
{
public:
  virtual IKeyboard* GetKeyboard() = 0;
  virtual IMouse* GetMouse() = 0;
  virtual IGamepad* GetGamepad() = 0;

  virtual void Destroy() = 0;
};
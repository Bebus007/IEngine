#pragma once

#include "..\IEngineObject.h"

class IKeyboard;
class IMouse;
class IGamepad;

class IInputEngine : public IEngineObject
{
public:
  virtual IKeyboard* GetKeyboard() = 0;
  virtual IMouse* GetMouse() = 0;
  virtual IGamepad* GetGamepad() = 0;
};
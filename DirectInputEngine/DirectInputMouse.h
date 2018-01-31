#pragma once

#include "IMouse.h"

class IWindowEx;

class CDirectInputMouse : public IMouse
{
public:
  CDirectInputMouse(const IWindowEx*);

  virtual bool IsButtonDown(Button_e) override;
  virtual int GetRelativeAxisPosition(Axis_e) override;
};
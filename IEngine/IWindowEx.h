#pragma once

#include "IWindow.h"

class IWindowEx : public IWindow
{
public:
  virtual void* GetHandle() const = 0;
};
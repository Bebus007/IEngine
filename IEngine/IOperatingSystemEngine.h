#pragma once

#include "IEngineObject.h"

class IWindow;
class IFile;
class IFont;

class IOperatingSystemEngine : public IEngineObject
{
public:
  virtual IWindow* GetConsoleWindow() = 0;
  virtual IWindow* GetWindowInstance() = 0;

  virtual IFile* OpenFile(const char* filename) = 0;
};
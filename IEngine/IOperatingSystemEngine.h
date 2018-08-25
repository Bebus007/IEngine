#pragma once

class IWindow;
class IFile;
class IFont;

class IOperatingSystemEngine
{
public:
  virtual IWindow* GetConsoleWindow() = 0;
  virtual IWindow* GetWindowInstance() = 0;

  //virtual IFont* CreateFont(const char* fontname) = 0;

  virtual IFile* OpenFile(const char* filename) = 0;
};
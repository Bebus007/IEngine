#pragma once

#include "CommandLineParser.h"

class IOperatingSystemEngine;
class IGraphicsEngine;
class IWindow;

class CApp
{
public:
  static void Init(int argc, const char* argv[]);
  static void Run();
  static void Destroy();

private:
  static IOperatingSystemEngine* m_pOSEngine;
  static IGraphicsEngine* m_pGraphicsEngine;
  static CCommandLineParser::data_t m_InputData;
  static IWindow* m_pWindow;
};
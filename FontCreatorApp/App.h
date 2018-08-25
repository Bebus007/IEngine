#pragma once

#include "CommandLineParser.h"

class IOperatingSystemEngine;

static class CApp
{
public:
  static void Init(int argc, const char* argv[]);
  static void Run();
  static void Destroy();
private:
  static IOperatingSystemEngine* m_pOSEngine;
  static CCommandLineParser::data_t m_InputData;
};
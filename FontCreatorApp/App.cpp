#include "App.h"

#include "IEngine.h"
#include "IOperatingSystemEngine.h"

#include "CommandLineParser.h"

IOperatingSystemEngine* CApp::m_pOSEngine = IEngine::CreateOperatingSystemEngine();
CCommandLineParser::data_t CApp::m_InputData;

void CApp::Init(int argc, const char * argv[])
{
  m_InputData = CCommandLineParser::Parse(argc, argv);
}

void CApp::Run()
{
  if (!m_pOSEngine)
    return;
}

void CApp::Destroy()
{
}

#include "App.h"

#include "IEngine.h"
#include "IOperatingSystemEngine.h"
#include "IWindow.h"
#include "IGraphicsEngine.h"
#include "IGraphics2D.h"

#include "CommandLineParser.h"

IOperatingSystemEngine* CApp::m_pOSEngine = IEngine::CreateOperatingSystemEngine();
IGraphicsEngine* CApp::m_pGraphicsEngine = nullptr;
CCommandLineParser::data_t CApp::m_InputData;
IWindow* CApp::m_pWindow = nullptr;
IGraphics2D* CApp::m_pGraphics2D = nullptr;
IFont* CApp::m_pFont = nullptr;

const int textSize = 32;

void CApp::Init(int argc, const char * argv[])
{
  m_InputData = CCommandLineParser::Parse(argc, argv);

  if (m_pOSEngine->GetConsoleWindow())
    m_pOSEngine->GetConsoleWindow()->Hide();

  m_pWindow = m_pOSEngine->GetWindowInstance();
  if (!m_pWindow)
    return;
  
  m_pGraphicsEngine = IEngine::CreateGraphicsEngine(IEngine::GET_GDI, m_pWindow);
  if (!m_pGraphicsEngine)
    return;

  m_pGraphics2D = m_pGraphicsEngine->Get2DInterface();
  if (!m_pGraphics2D)
    return;

  m_pFont = m_pGraphics2D->CreateFontInstance(textSize, "Consolas");
  if (!m_pFont)
    return;
}

void CApp::Run()
{
  if (!m_pFont || !m_pGraphics2D || !m_pWindow)
    return;

  for (int i = 0; i < 1000; i++)
  {
    m_pWindow->Idle();
  }

  while (m_pWindow->Idle())
  {
    char val[2] = { 0, 0 };
    float offsetX, offsetY;
    offsetX = offsetY = 0.0f;
    for (int i = 0; i < 8; i++)
    {
      offsetX = 0;

      for (int j = 0; j < 32; j++)
      {
        val[0] = (char)(i * 32 + j);

        IGraphics2D::Size_t size = m_pGraphics2D->GetTextSize(m_pFont, val);
        offsetX += size.X;

        m_pGraphics2D->DrawText(IGraphics2D::Vertex_t(offsetX, offsetY), m_pFont, val);
      }

      offsetY += textSize;
    }
  }
}

void CApp::Destroy()
{
}

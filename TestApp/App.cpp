#include "App.h"

#include "IEngine.h"

#include "OS/IOperatingSystemEngine.h"
#include "OS/IWindow.h"

#include "Input/IInputEngine.h"
#include "Input/IKeyboard.h"

#include "Graphics/IGraphicsEngine.h"
#include "Graphics/IGraphics2D.h"

ISoundEngine* CApp::m_pSoundEngine = nullptr;
IOperatingSystemEngine* CApp::m_pOSEngine = nullptr;
IInputEngine* CApp::m_pInputEngine = nullptr;
const IEngine::GraphicsEngineType_e CApp::m_graphicsType = IEngine::GraphicsEngineType_e::GET_DIRECT_X_11;
IGraphicsEngine* CApp::m_pGraphicsEngine = nullptr;

bool CApp::Init(int width, int height, bool fullscreen, bool hideConsole)
{
  IOperatingSystemEngine* pOSEngine = GetOSEngine();
  if (!pOSEngine)
  {
    Destroy();
    return false;
  }

  if (hideConsole)
  {
    IWindow* pConsoleWindow = pOSEngine->GetConsoleWindow();
    pConsoleWindow->Hide();
  }

  IGraphicsEngine* pGraphicsEngine = GetGraphicsEngine();
  if (!pGraphicsEngine)
  {
    Destroy();
    return false;
  }

  bool result = pGraphicsEngine->Init(width, height, fullscreen);
  if (!result)
    Destroy();

  return result;
}

void CApp::Run()
{
  IWindow* pGameWindow = GetWindow();
  if (!pGameWindow)
    return;

  IKeyboard* pKeyboard = GetKeyboard();
  if (!pKeyboard)
    return;

  while (pGameWindow->Idle())
  {
    if (pKeyboard->IsKeyDown(IKeyboard::KEY_ESCAPE))
      break;

    RenderScene();
  }
}

void CApp::Destroy()
{
  if (m_pInputEngine)
  {
    m_pInputEngine->Destroy();
    m_pInputEngine = nullptr;
  }

  if (m_pGraphicsEngine)
  {
    m_pGraphicsEngine->Destroy();
    m_pGraphicsEngine = nullptr;
  }

  if (m_pOSEngine)
  {
    m_pOSEngine->Destroy();
    m_pOSEngine = nullptr;
  }
}

ISoundEngine * CApp::GetSoundEngine()
{
  if (!m_pSoundEngine)
  {
    IWindow* pGameWindow = GetWindow();
    if (!pGameWindow)
      return nullptr;

    m_pSoundEngine = IEngine::CreateSoundEngine(pGameWindow);
  }

  return m_pSoundEngine;
}

IOperatingSystemEngine * CApp::GetOSEngine()
{
  if (!m_pOSEngine)
    m_pOSEngine = IEngine::CreateOperatingSystemEngine();

  return m_pOSEngine;
}

IInputEngine * CApp::GetInputEngine()
{
  if (!m_pInputEngine)
  {
    IWindow* pGameWindow = GetWindow();
    if (!pGameWindow)
      return nullptr;

    m_pInputEngine = IEngine::CreateInputEngine(pGameWindow);
  }

  return m_pInputEngine;
}

IGraphicsEngine * CApp::GetGraphicsEngine()
{
  if (!m_pGraphicsEngine)
  {
    IWindow* pGameWindow = GetWindow();
    if (!pGameWindow)
      return nullptr;

    m_pGraphicsEngine = IEngine::CreateGraphicsEngine(m_graphicsType, pGameWindow);
  }

  return m_pGraphicsEngine;
}

IWindow* CApp::GetWindow()
{
  if (!GetOSEngine())
    return nullptr;

  return GetOSEngine()->GetWindowInstance();
}

IKeyboard* CApp::GetKeyboard()
{
  if (!GetInputEngine())
    return nullptr;

  return GetInputEngine()->GetKeyboard();
}

IGraphics2D* CApp::Get2DInterface()
{
  if (!GetGraphicsEngine())
    return nullptr;

  return GetGraphicsEngine()->Get2DInterface();
}

void CApp::RenderScene()
{
  auto pGraphicsEngine = GetGraphicsEngine();
  if (!pGraphicsEngine)
    return;

  auto p2DInterface = Get2DInterface();
  if (!p2DInterface)
    return;

  pGraphicsEngine->ClearScreen();

  static const IGraphicsEngine::Vertex_t vertices[3] = {
    {-200.0f / 320.0f, -240.0f / 240.0f, 0.5f, 1.0f},
    {0.0f / 320.0f, 240.0f / 240.0f, 0.5f, 1.0f},
    {200.0f / 320.0f, -240.0f / 240.0f, 0.5f, 1.0f}
  };

  pGraphicsEngine->DrawTriangle(vertices[0], vertices[1], vertices[2]);

  pGraphicsEngine->Swap();
}

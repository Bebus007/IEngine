#include <iostream>
#include <Windows.h>
using namespace std;

#include "IEngine.h"
#include "IOperatingSystemEngine.h"
#include "IWindow.h"
#include "IInputEngine.h"
#include "IKeyboard.h"
#include "IGraphicsEngine.h"
#include "IGraphics2D.h"
#include "ISoundEngine.h"
#include "ISound.h"

int main()
{
  IOperatingSystemEngine* pOSEngine = IEngine::CreateOperatingSystemEngine();

  IWindow* pConsoleWindow = pOSEngine->GetConsoleWindow();
  pConsoleWindow->Hide();

  IWindow* pGameWindow = pOSEngine->GetWindowInstance();

  auto pInputEngine = IEngine::CreateInputEngine(pGameWindow);

  auto pKeyboard = pInputEngine->GetKeyboard();

  auto pGraphicsEngine = IEngine::CreateGraphicsEngine(IEngine::GraphicsEngineType_e::GET_DIRECT_X_11, pGameWindow);

  pGraphicsEngine->Init();

  auto p2DInterface = pGraphicsEngine->Get2DInterface();

  IGraphics2D::Vertex vertices[3] = {
    {-200.0f, -240.0f},
    {0.0f, 240.0f},
    {200.0f, -240.0f}
  };

  while (!pKeyboard->IsKeyDown(IKeyboard::KEY_ESCAPE) && pGameWindow->Idle())
  {
    if (pKeyboard->IsKeyDown(IKeyboard::KEY_ESCAPE))
      break;

    pGraphicsEngine->ClearScreen();

    p2DInterface->DrawTriangle(vertices[0], vertices[1], vertices[2]);

    pGraphicsEngine->Swap();
  }

  pGraphicsEngine->Destroy();
  pInputEngine->Destroy();
  pOSEngine->Destroy();

  return 0;
}
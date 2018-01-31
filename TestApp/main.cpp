#include <iostream>
#include <Windows.h>
using namespace std;

#include "IEngine.h"
#include "IOperatingSystemEngine.h"
#include "IWindow.h"
#include "IInputEngine.h"
#include "IKeyboard.h"
#include "IGraphicsEngine.h"

void main()
{
  IOperatingSystemEngine* pOSEngine = IEngine::CreateOperatingSystemEngine();

  IWindow* pConsoleWindow = pOSEngine->GetConsoleWindow();
  pConsoleWindow->Hide();

  IWindow* pGameWindow = pOSEngine->GetWindowInstance();

  auto pInputEngine = IEngine::CreateInputEngine(pGameWindow);

  auto pKeyboard = pInputEngine->GetKeyboard();

  auto pGraphicsEngine = IEngine::CreateGraphicsEngine(pGameWindow);

  pGraphicsEngine->Init();

  while (!pKeyboard->IsKeyDown(IKeyboard::KEY_ESCAPE) && pGameWindow->Idle())
  {
    if (pKeyboard->IsKeyDown(IKeyboard::KEY_ESCAPE))
      break;

    pGraphicsEngine->ClearScreen();
    pGraphicsEngine->Swap();
  }

  delete pInputEngine;
  delete pOSEngine;
}
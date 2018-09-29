#include <iostream>
#include <Windows.h>
using namespace std;

#include "IEngine.h"

#include "OS/IOperatingSystemEngine.h"
#include "OS/IWindow.h"

#include "Input/IInputEngine.h"
#include "Input/IKeyboard.h"

#include "Graphics/IGraphicsEngine.h"
#include "Graphics/IGraphics2D.h"
#include "Graphics/IImage.h"

#include "Sound/ISoundEngine.h"
#include "Sound/ISound.h"

#include "OS/IBitmapFile.h"

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

  IGraphics2D::Vertex_t vertices[3] = {
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

    IImage* pScreen = p2DInterface->CaptureScreen();

    IFile* pFile = pOSEngine->OpenFile("D:\\image.bmp");
    if (!pFile)
      return 0;

    IBitmapFile* pBitmapFile = dynamic_cast<IBitmapFile*>(pFile);
    pBitmapFile->Clear();
    pBitmapFile->SetBitmapWidth(pScreen->GetWidth());
    pBitmapFile->SetBitmapHeight(pScreen->GetHeight());
    pBitmapFile->SetBitmapColorBitCount(pScreen->GetColorBitCount());
    pBitmapFile->SetBitmapPlanesCount(1);
    pBitmapFile->SetBitmapData(pScreen->GetBits());

    pFile->Destroy();

    pGraphicsEngine->Swap();
  }

  pGraphicsEngine->Destroy();
  pInputEngine->Destroy();
  pOSEngine->Destroy();

  return 0;
}
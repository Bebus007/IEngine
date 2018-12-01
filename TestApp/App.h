#pragma once

#include "IEngine.h"

class IKeyboard;
class IGraphics2D;

static class CApp
{
public:
  static bool Init(int width = 640, int height = 480, bool fullscreen = false, bool hideConsole = true);

  static void Run();

  static void Destroy();

  static ISoundEngine*            GetSoundEngine();
  static IOperatingSystemEngine*  GetOSEngine();
  static IInputEngine*            GetInputEngine();
  static IGraphicsEngine*         GetGraphicsEngine();

  static void SetSoundEngine(ISoundEngine*);
  static void SetOSEngine(IOperatingSystemEngine*);
  static void SetInputEngine(IInputEngine*);
  static void SetGraphicsEngine(IGraphicsEngine*);

private:
  static ISoundEngine* m_pSoundEngine;
  static IOperatingSystemEngine* m_pOSEngine;
  static IInputEngine* m_pInputEngine;
  static const IEngine::GraphicsEngineType_e m_graphicsType;
  static IGraphicsEngine* m_pGraphicsEngine;

  static IWindow* GetWindow();
  static IKeyboard* GetKeyboard();
  static IGraphics2D* Get2DInterface();

  static void RenderScene();
};
// IEngine.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "inc/IEngine.h"

#include "..\WindowsOSEngine\WindowsOSEngine.h"
#include "..\DirectInputEngine\DirectInputEngine.h"
#include "..\DirectX11GraphicsEngine\DirectX11GraphicsEngine.h"
#include "..\DirectSoundEngine\DirectSoundEngine.h"

#include "..\GDIGraphicsEngine\GDIGraphicsEngine.h"

#include "inc\OS\IWindowEx.h"

IOperatingSystemEngine * IEngine::CreateOperatingSystemEngine()
{
  return new CWindowsOSEngine();
}

IENGINE_API IInputEngine * IEngine::CreateInputEngine(const IWindow* pWnd)
{
  if (!pWnd)
    return nullptr;

  return new CDirectInputEngine(dynamic_cast<const IWindowEx*>(pWnd));
}

IENGINE_API IGraphicsEngine * IEngine::CreateGraphicsEngine(GraphicsEngineType_e type, IWindow * pWnd)
{
  if (!pWnd)
    return nullptr;

  switch (type)
  {
  case IEngine::GET_DIRECT_X_11:
    return new CDirectX11GraphicsEngine(dynamic_cast<IWindowEx*>(pWnd));
  /*case IEngine::GET_GDI:
    return new CGDIGraphicsEngine(dynamic_cast<IWindowEx*>(pWnd));*/
  case IEngine::GET_UNDEFINED:
  default:
    break;
  }

  return nullptr;
}

IENGINE_API ISoundEngine * IEngine::CreateSoundEngine(IWindow * pWnd)
{
  if (!pWnd)
    return nullptr;

  return new CDirectSoundEngine(dynamic_cast<IWindowEx*>(pWnd));
}

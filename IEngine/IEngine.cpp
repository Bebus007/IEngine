// IEngine.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "IEngine.h"

#include "..\WindowsOSEngine\WindowsOSEngine.h"
#include "..\DirectInputEngine\DirectInputEngine.h"
#include "..\DirectX11GraphicsEngine\DirectX11GraphicsEngine.h"

#include "IWindowEx.h"

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

IENGINE_API IGraphicsEngine * IEngine::CreateGraphicsEngine(IWindow * pWnd)
{
  if (!pWnd)
    return nullptr;

  return new CDirectX11GraphicsEngine(dynamic_cast<IWindowEx*>(pWnd));
}

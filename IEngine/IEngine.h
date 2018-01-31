#pragma once

#if IENGINE_EXPORTS
#define IENGINE_API __declspec(dllexport)
#else
#define IENGINE_API __declspec(dllimport)
#endif

class IOperatingSystemEngine;
class IInputEngine;
class IGraphicsEngine;
class IWindow;

static class IEngine
{
public:
  static IENGINE_API IOperatingSystemEngine * CreateOperatingSystemEngine();
  static IENGINE_API IInputEngine * CreateInputEngine(const IWindow*);
  static IENGINE_API IGraphicsEngine * CreateGraphicsEngine(IWindow*);
};
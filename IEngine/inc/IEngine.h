#pragma once

#if IENGINE_EXPORTS
#define IENGINE_API __declspec(dllexport)
#else
#define IENGINE_API __declspec(dllimport)
#endif

class IOperatingSystemEngine;
class IInputEngine;
class IGraphicsEngine;
class ISoundEngine;
class IWindow;

class IEngine
{
public:
  static IENGINE_API IOperatingSystemEngine * CreateOperatingSystemEngine();

  static IENGINE_API IInputEngine * CreateInputEngine(const IWindow*);

  enum GraphicsEngineType_e
  {
    GET_DIRECT_X_11,
    GET_GDI,
    GET_UNDEFINED
  };
  static IENGINE_API IGraphicsEngine * CreateGraphicsEngine(GraphicsEngineType_e, IWindow*);

  static IENGINE_API ISoundEngine * CreateSoundEngine(IWindow*);
};
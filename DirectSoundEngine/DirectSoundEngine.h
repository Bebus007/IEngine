#pragma once

#include "ISoundEngine.h"

class IWindowEx;
struct IDirectSound8;

class CDirectSoundEngine : public ISoundEngine
{
public:
  CDirectSoundEngine(const IWindowEx * pWnd);
  ~CDirectSoundEngine();

  virtual void Play(ISound*) override;

  virtual ISound* Load(const char* filename) override;

private:
  IDirectSound8 * m_pInterface;
};
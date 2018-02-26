#pragma once

#include "ISound.h"

struct IDirectSoundBuffer8;

class CDirectSoundSound : public ISound
{
public:
  CDirectSoundSound(IDirectSoundBuffer8* pBuffer);
  ~CDirectSoundSound();

  virtual void Play() override;

private:
  IDirectSoundBuffer8 * m_pBuffer;
};
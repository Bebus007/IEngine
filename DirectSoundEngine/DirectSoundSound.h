#pragma once

#include "ISound.h"

struct IDirectSoundBuffer8;

class DirectSoundSound : public ISound
{
public:
  DirectSoundSound(IDirectSoundBuffer8* pBuffer);
  ~DirectSoundSound();

private:
  IDirectSoundBuffer8 * m_pBuffer;
};
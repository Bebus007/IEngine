#include "stdafx.h"
#include "DirectSoundSound.h"

#include <dsound.h>

DirectSoundSound::DirectSoundSound(IDirectSoundBuffer8 * pBuffer) : m_pBuffer(pBuffer)
{
}

DirectSoundSound::~DirectSoundSound()
{
  if (m_pBuffer)
  {
    m_pBuffer->Release();
    m_pBuffer = nullptr;
  }
}

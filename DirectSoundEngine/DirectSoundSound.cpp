#include "stdafx.h"
#include "DirectSoundSound.h"

#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>

CDirectSoundSound::CDirectSoundSound(IDirectSoundBuffer8 * pBuffer) : m_pBuffer(pBuffer)
{
}

CDirectSoundSound::~CDirectSoundSound()
{
  if (m_pBuffer)
  {
    m_pBuffer->Release();
    m_pBuffer = nullptr;
  }
}

void CDirectSoundSound::Play()
{
  if (!m_pBuffer)
    return;

  HRESULT hr = S_OK;

  // Set position at the beginning of the sound buffer.
  hr = m_pBuffer->SetCurrentPosition(0);
  if (FAILED(hr))
    return;

  // Set volume of the buffer to 100%.
  hr = m_pBuffer->SetVolume(DSBVOLUME_MAX);
  if (FAILED(hr))
    return;

  // Play the contents of the secondary sound buffer.
  hr = m_pBuffer->Play(0, 0, 0);
  if (FAILED(hr))
    return;
}

#include "stdafx.h"
#include "DirectSoundEngine.h"

#include "IWindowEx.h"

#include <dsound.h>

CDirectSoundEngine::CDirectSoundEngine(const IWindowEx * pWnd) : m_pInterface(nullptr)
{
  HRESULT hr = S_OK;
  hr = DirectSoundCreate8(&DSDEVID_DefaultPlayback, &m_pInterface, nullptr);
  m_pInterface->SetCooperativeLevel((HWND)pWnd->GetHandle(), DSSCL_PRIORITY);
}

CDirectSoundEngine::~CDirectSoundEngine()
{
  if (m_pInterface)
  {
    m_pInterface->Release();
    m_pInterface = nullptr;
  }
}

void CDirectSoundEngine::Play(ISound *)
{
}

ISound * CDirectSoundEngine::Load(const char * filename)
{
  return nullptr;
}

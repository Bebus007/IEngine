#include "stdafx.h"
#include "DirectSoundEngine.h"

#include "IWindowEx.h"
#include "IWAVFile.h"
#include "DirectSoundSound.h"

#include <Windows.h>
#include <mmsystem.h>
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

ISound * CDirectSoundEngine::CreateSound(IFile * pFile)
{
  IWAVFile* pWAVFile = dynamic_cast<IWAVFile*>(pFile);
  if (!pWAVFile)
    return nullptr;

  if (!m_pInterface)
    return nullptr;

  WAVEFORMATEX waveFormat;
  DSBUFFERDESC bufferDesc;
  IDirectSoundBuffer* tempBuffer = nullptr;

  // Set the wave format of secondary buffer that this wave file will be loaded onto.
  waveFormat.wFormatTag = pWAVFile->GetAudioFormat();
  waveFormat.nSamplesPerSec = pWAVFile->GetSampleRate();
  waveFormat.wBitsPerSample = pWAVFile->GetBitsPerSample();
  waveFormat.nChannels = pWAVFile->GetNumChannels();
  waveFormat.nBlockAlign = pWAVFile->GetBlockAlign();
  waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
  waveFormat.cbSize = 0;

  // Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
  bufferDesc.dwSize = sizeof(DSBUFFERDESC);
  bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
  bufferDesc.dwBufferBytes = pWAVFile->GetSubChunk2Size();
  bufferDesc.dwReserved = 0;
  bufferDesc.lpwfxFormat = &waveFormat;
  bufferDesc.guid3DAlgorithm = GUID_NULL;

  // Create a temporary sound buffer with the specific buffer settings.
  HRESULT hr = m_pInterface->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
  if (FAILED(hr) || !tempBuffer)
    return nullptr;

  IDirectSoundBuffer8* secondaryBuffer = nullptr;
  // Test the buffer format against the direct sound 8 interface and create the secondary buffer.
  hr = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&secondaryBuffer);

  // Release the temporary buffer.
  tempBuffer->Release();
  tempBuffer = nullptr;

  if (FAILED(hr) || !secondaryBuffer)
    return nullptr;

  unsigned char *bufferPtr = nullptr;
  unsigned long bufferSize = 0;

  // Lock the secondary buffer to write wave data into it.
  hr = secondaryBuffer->Lock(0, pWAVFile->GetSubChunk2Size(), (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, DSBLOCK_ENTIREBUFFER);
  if (FAILED(hr))
  {
    secondaryBuffer->Release();
    secondaryBuffer = nullptr;
    return nullptr;
  }

  // Copy the wave data into the buffer.
  memcpy(bufferPtr, pWAVFile->GetWAVDataPointer(), bufferSize < pWAVFile->GetSubChunk2Size() ? bufferSize : pWAVFile->GetSubChunk2Size());

  // Unlock the secondary buffer after the data has been written to it.
  hr = secondaryBuffer->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
  if (FAILED(hr))
  {
    secondaryBuffer->Release();
    secondaryBuffer = nullptr;
    return nullptr;
  }

  return new CDirectSoundSound(secondaryBuffer);
}

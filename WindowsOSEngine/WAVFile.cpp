#include "stdafx.h"
#include "WAVFile.h"

#include <memory>

CWAVFile::CWAVFile(const char * filename) : CFile(filename)
{
  size_t headerSize = sizeof(WaveHeader_t);

  memset(&m_header, 0, headerSize);
  m_WAVData = nullptr;

  if (CFile::GetFileSize() < headerSize)
    return;

  memcpy(&m_header, CFile::GetFileData(), headerSize);
  m_WAVData = CFile::GetFileData() + headerSize;
}

const char * CWAVFile::GetChunkID() const
{
  return m_header.chunkId;
}

unsigned long CWAVFile::GetChunkSize() const
{
  return m_header.chunkSize;
}

const char * CWAVFile::GetFormat() const
{
  return m_header.format;
}

const char * CWAVFile::GetSubChunk1ID() const
{
  return m_header.subChunkId;
}

unsigned long CWAVFile::GetSubChunk1Size() const
{
  return m_header.subChunkSize;
}

unsigned short CWAVFile::GetAudioFormat() const
{
  return m_header.audioFormat;
}

unsigned short CWAVFile::GetNumChannels() const
{
  return m_header.numChannels;
}

unsigned long CWAVFile::GetSampleRate() const
{
  return m_header.sampleRate;
}

unsigned long CWAVFile::GetByteRate() const
{
  return m_header.bytesPerSecond;
}

unsigned short CWAVFile::GetBlockAlign() const
{
  return m_header.blockAlign;
}

unsigned short CWAVFile::GetBitsPerSample() const
{
  return m_header.bitsPerSample;
}

const char * CWAVFile::GetSubChunk2ID() const
{
  return m_header.dataChunkId;
}

unsigned long CWAVFile::GetSubChunk2Size() const
{
  return m_header.dataSize;
}

const void * CWAVFile::GetWAVDataPointer() const
{
  return m_WAVData;
}

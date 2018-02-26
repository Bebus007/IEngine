#pragma once

#include "IWAVFile.h"
#include "File.h"

class CWAVFile : public CFile, virtual public IWAVFile
{
public:
  CWAVFile(const char* filename);

  virtual const char* GetChunkID() const override;

  virtual unsigned long GetChunkSize() const override;

  virtual const char* GetFormat() const override;

  virtual const char* GetSubChunk1ID() const override;

  virtual unsigned long GetSubChunk1Size() const override;

  virtual unsigned short GetAudioFormat() const override;

  virtual unsigned short GetNumChannels() const override;

  virtual unsigned long GetSampleRate() const override;

  virtual unsigned long GetByteRate() const override;

  virtual unsigned short GetBlockAlign() const override;

  virtual unsigned short GetBitsPerSample() const override;

  virtual const char* GetSubChunk2ID() const override;

  virtual unsigned long GetSubChunk2Size() const override;

  virtual const void* GetWAVDataPointer() const override;

private:
  struct WaveHeader_t
  {
    char chunkId[4];
    unsigned long chunkSize;
    char format[4];
    char subChunkId[4];
    unsigned long subChunkSize;
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned long sampleRate;
    unsigned long bytesPerSecond;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    char dataChunkId[4];
    unsigned long dataSize;
  };

  WaveHeader_t m_header;
  void* m_WAVData;
};
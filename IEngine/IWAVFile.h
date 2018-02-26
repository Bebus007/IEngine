#pragma once

#include "IFile.h"

class IWAVFile
{
public:

  // Содержит символы "RIFF" в ASCII кодировке
  virtual const char* GetChunkID() const = 0;
  // это размер файла - 8, исключены поля chunkId и chunkSize
  virtual unsigned long GetChunkSize() const = 0;
  // Содержит символы "WAVE"
  virtual const char* GetFormat() const = 0;
  // Содержит символы "fmt "
  virtual const char* GetSubChunk1ID() const = 0;
  // 16 для формата PCM. Это оставшийся размер подцепочки, начиная с этой позиции.
  virtual unsigned long GetSubChunk1Size() const = 0;
  // Для PCM = 1. Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
  virtual unsigned short GetAudioFormat() const = 0;
  // Количество каналов. Моно = 1, Стерео = 2 и т.д.
  virtual unsigned short GetNumChannels() const = 0;
  // Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
  virtual unsigned long GetSampleRate() const = 0;
  // sampleRate * numChannels * bitsPerSample/8
  virtual unsigned long GetByteRate() const = 0;
  // Количество байт для одного сэмпла, включая все каналы. (numChannels * bitsPerSample/8)
  virtual unsigned short GetBlockAlign() const = 0;
  // Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
  virtual unsigned short GetBitsPerSample() const = 0;
  // Содержит символы "data".
  virtual const char* GetSubChunk2ID() const = 0;
  // Количество байт в области данных.(numSamples * numChannels * bitsPerSample/8)
  virtual unsigned long GetSubChunk2Size() const = 0;
  // Hепосредственно Wav данные.
  virtual const void* GetWAVDataPointer() const = 0;
};
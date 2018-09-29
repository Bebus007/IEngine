#pragma once

#include "IFile.h"

class IWAVFile
{
public:

  // �������� ������� "RIFF" � ASCII ���������
  virtual const char* GetChunkID() const = 0;
  // ��� ������ ����� - 8, ��������� ���� chunkId � chunkSize
  virtual unsigned long GetChunkSize() const = 0;
  // �������� ������� "WAVE"
  virtual const char* GetFormat() const = 0;
  // �������� ������� "fmt "
  virtual const char* GetSubChunk1ID() const = 0;
  // 16 ��� ������� PCM. ��� ���������� ������ ����������, ������� � ���� �������.
  virtual unsigned long GetSubChunk1Size() const = 0;
  // ��� PCM = 1. ��������, ������������ �� 1, ���������� ��������� ������ ������.
  virtual unsigned short GetAudioFormat() const = 0;
  // ���������� �������. ���� = 1, ������ = 2 � �.�.
  virtual unsigned short GetNumChannels() const = 0;
  // ������� �������������. 8000 ��, 44100 �� � �.�.
  virtual unsigned long GetSampleRate() const = 0;
  // sampleRate * numChannels * bitsPerSample/8
  virtual unsigned long GetByteRate() const = 0;
  // ���������� ���� ��� ������ ������, ������� ��� ������. (numChannels * bitsPerSample/8)
  virtual unsigned short GetBlockAlign() const = 0;
  // ��� ���������� "��������" ��� �������� ��������. 8 ���, 16 ��� � �.�.
  virtual unsigned short GetBitsPerSample() const = 0;
  // �������� ������� "data".
  virtual const char* GetSubChunk2ID() const = 0;
  // ���������� ���� � ������� ������.(numSamples * numChannels * bitsPerSample/8)
  virtual unsigned long GetSubChunk2Size() const = 0;
  // H�������������� Wav ������.
  virtual const void* GetWAVDataPointer() const = 0;
};
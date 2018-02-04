#pragma once

class ISound;

class ISoundEngine
{
public:
  virtual void Play(ISound*) = 0;

  virtual ISound* Load(const char* filename) = 0;
};
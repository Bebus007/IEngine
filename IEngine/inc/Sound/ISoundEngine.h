#pragma once

class ISound;
class IFile;

class ISoundEngine
{
public:
  virtual ISound* CreateSound(IFile*) = 0;
};
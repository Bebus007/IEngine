#pragma once

#include "IFile.h"

#include <vector>

class CFile : virtual public IFile
{
public:
  CFile(const std::string& filename);
  ~CFile();

  virtual const std::string& GetFileName() const override;
  virtual size_t GetFileSize() const override;

  virtual std::vector<unsigned char>& GetFileData() override;

  virtual const std::vector<unsigned char>& GetFileData() const override;
  virtual void Clear() override;
  virtual void Write(unsigned char * data, int size) override;

  virtual void Destroy() override;

protected:
  void Load();
  void Unload();

  void SetModified(bool);
  bool IsModified() const;

  void ResizeChunk(size_t startPos, size_t endPos, size_t newSize);
  void Resize(size_t newSize);

private:
  bool m_isModified;
  std::string m_filename;
  std::vector<unsigned char> m_data;
};
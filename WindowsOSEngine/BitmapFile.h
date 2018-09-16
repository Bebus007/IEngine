#pragma once

#include "IBitmapFile.h"
#include "File.h"

#include <wingdi.h>

class CBitmapFile : public CFile, virtual public IBitmapFile
{
public:
  CBitmapFile(const std::string& filename);

#pragma region Getters
  virtual int GetBitmapWidth() const override;
  virtual int GetBitmapHeight() const override;
  virtual unsigned int GetBitmapColorBitCount() const override;
  virtual WORD GetBitmapPlanesCount() const override;

  virtual int GetBitmapDataSize() const override;
  virtual const void* GetBitmapData() const override;
  void* GetBitmapData();
#pragma endregion

#pragma region Setters
  virtual void SetBitmapWidth(int) override;
  virtual void SetBitmapHeight(int) override;
  virtual void SetBitmapColorBitCount(unsigned int) override;
  virtual void SetBitmapPlanesCount(WORD) override;

  virtual void SetBitmapData(const void*) override;
#pragma endregion

  virtual void Destroy() override;

private:
#pragma region Block Access
  const BITMAPFILEHEADER* GetHeader() const;
  BITMAPFILEHEADER* GetHeader();

  static constexpr size_t GetBitmapInfoPos();
  const PBITMAPINFO GetBitmapInfo() const;
  PBITMAPINFO GetBitmapInfo();

  size_t GetBitmapDataPosition() const;
#pragma endregion

  void OnParamChanged(int width, int height, int planesCount, int colorBitCount);

  static size_t CalculateDataSize(int width, int height, int planesCount, int colorBitCount);

  void CreateHeader();
  void UpdateHeader(int width, int height, int planesCount, int colorBitCount);
  void CreateBitmapInfo();
  void UpdateBitmapInfo(int width, int height, int planesCount, int colorBitCount);
  void ResizeBitmapData(size_t newSize);

  static size_t GetBitmapInfoSize(int planesCount, int colorBitCount);
  size_t GetBitmapInfoSize() const;
  WORD GetClrBits() const;
  static WORD GetClrBits(int planesCount, int colorBitCount);
};
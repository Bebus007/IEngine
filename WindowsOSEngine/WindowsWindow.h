#pragma once

#include "IWindowEx.h"

#include <Windows.h>

#include <list>

class CWindowsWindow : public IWindowEx
{
public:
  CWindowsWindow(HWND);

  virtual bool IsVisible() override;
  virtual void Close() override;
  virtual void Hide() override;

  virtual void SetPosition(int x, int y) override;

  virtual void SetClientSize(int w, int h) override;
  virtual void GetClientSize(int* w, int* h) const override;

  virtual void* GetHandle() const override;

  virtual bool Idle() override;

  virtual void AddWindowResizeEventListener(IWindowResizeEventListener*) override;
  virtual void RemoveWindowResizeEventListener(IWindowResizeEventListener*) override;

  LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:
  HWND m_hWnd;

  std::list<IWindowResizeEventListener*> m_resizeListenersList;
};
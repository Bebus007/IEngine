#include "stdafx.h"
#include "WindowsWindow.h"

CWindowsWindow::CWindowsWindow(HWND hWnd) : m_hWnd(hWnd)
{
  ShowWindow(m_hWnd, SW_SHOW);
}

bool CWindowsWindow::IsVisible()
{
  if (!m_hWnd)
    return false;

  return IsWindowVisible(m_hWnd) ? true : false;
}

void CWindowsWindow::Close()
{
  DWORD err = GetLastError();
  BOOL res = CloseWindow(m_hWnd);
  err = GetLastError();
  if (!res)
    return;
}

void CWindowsWindow::Hide()
{
  ShowWindow(m_hWnd, 0);
}

void CWindowsWindow::SetPosition(int x, int y)
{
  RECT oldRC;
  GetWindowRect(m_hWnd, &oldRC);

  MoveWindow(m_hWnd, x, y, oldRC.right - oldRC.left, oldRC.bottom - oldRC.top, FALSE);
}

void CWindowsWindow::SetClientSize(int w, int h)
{
  if (!m_hWnd)
    return;

  RECT oldRc;
  GetWindowRect(m_hWnd, &oldRc);

  RECT rc = { 0, 0, w, h };
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

  MoveWindow(m_hWnd, oldRc.left, oldRc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
}

void * CWindowsWindow::GetHandle() const
{
  return (void*)m_hWnd;
}

bool CWindowsWindow::Idle()
{
  if (!m_hWnd)
    return false;

  bool result = true;

  MSG msg = { 0 };
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (msg.message == WM_QUIT)
      result = false;
  }

  return result;
}

LRESULT CWindowsWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message)
  {
  case WM_PAINT:
    hdc = BeginPaint(m_hWnd, &ps);
    EndPaint(m_hWnd, &ps);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  default:
    return DefWindowProc(m_hWnd, message, wParam, lParam);
  }

  return 0;
}

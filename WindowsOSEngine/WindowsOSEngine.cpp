#include "stdafx.h"
#include "WindowsOSEngine.h"

#include "WindowsWindow.h"

#include "File.h"
#include "WAVFile.h"

#include <tchar.h>
#include <string>
#include <algorithm>
#include <cctype>

IWindow* CWindowsOSEngine::m_pWindowInstance = nullptr;

CWindowsOSEngine::CWindowsOSEngine()
{
}

CWindowsOSEngine::~CWindowsOSEngine()
{
  if (m_pWindowInstance)
  {
    delete m_pWindowInstance;
    m_pWindowInstance = nullptr;
  }
}

IWindow * CWindowsOSEngine::GetConsoleWindow()
{
  HWND hConsoleWnd = ::GetConsoleWindow();
  if (!hConsoleWnd)
    return nullptr;

  return new CWindowsWindow(hConsoleWnd);
}

IWindow * CWindowsOSEngine::GetWindowInstance()
{
    if (m_pWindowInstance)
      return m_pWindowInstance;

    HINSTANCE hInstance = GetModuleHandle(nullptr);

    TCHAR className[256];
    lstrcpy(className, __T("WindowsOSEngine"));
    _itot((int)this, &className[lstrlen(className)], 16);

    // Регистрация класса
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = className;
    wcex.hIconSm = NULL;
    if (!RegisterClassEx(&wcex))
      return nullptr;

    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    HWND hWnd = CreateWindow(className, _T("X11 Engine Window"), WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
    if (!hWnd)
      return nullptr;

    m_pWindowInstance = new CWindowsWindow(hWnd);
    return m_pWindowInstance;
}

void CWindowsOSEngine::Destroy()
{
  delete this;
}

LRESULT CWindowsOSEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if (!m_pWindowInstance)
    return DefWindowProc(hWnd, message, wParam, lParam);

  return (dynamic_cast<CWindowsWindow*>(m_pWindowInstance))->HandleMessage(message, wParam, lParam);
}

IFile * CWindowsOSEngine::OpenFile(const char * filename)
{
  std::string sFilename = filename;
  std::string sExtensionString = sFilename.substr(sFilename.rfind('.') + 1);

  std::transform(sExtensionString.begin(), sExtensionString.end(), sExtensionString.begin(), ::toupper);

  if (sExtensionString == "WAV")
  {
    return new CWAVFile(filename);
  }
  else
    return new CFile(filename);
  
  return nullptr;
}

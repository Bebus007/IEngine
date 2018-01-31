#include "stdafx.h"
#include "DirectX11GraphicsEngine.h"

#include "IWindowEx.h"

#include <d3d11.h>

CDirectX11GraphicsEngine::CDirectX11GraphicsEngine(IWindowEx * pWnd) :
  m_pWindow(pWnd),
  m_pd3dDevice(nullptr),
  m_pImmediateContext(nullptr),
  m_pRenderTargetView(nullptr),
  m_pSwapChain(nullptr),
  m_pDepthStencil(nullptr),
  m_pDepthStencilView(nullptr)
{
}

CDirectX11GraphicsEngine::~CDirectX11GraphicsEngine()
{
  Cleanup();
}

bool CDirectX11GraphicsEngine::Init(int width, int height, bool fullscreen)
{
  Cleanup();

  if (!m_pWindow)
    return false;

  if (m_pd3dDevice || m_pImmediateContext || m_pSwapChain || m_pRenderTargetView)
    return false;

  HWND hWnd = (HWND)m_pWindow->GetHandle();
  if (!hWnd)
    return false;

  m_pWindow->SetClientSize(width, height);

  HRESULT hr = S_OK;

  static const UINT createDeviceFlags = 0;

  static const D3D_DRIVER_TYPE driverTypes[] = {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
  };
  static const UINT numDriverTypes = ARRAYSIZE(driverTypes);

  // Тут мы создаем список поддерживаемых версий DirectX
  static const D3D_FEATURE_LEVEL featureLevels[] = {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
  };
  static const UINT numFeatureLevels = ARRAYSIZE(featureLevels);

  // Сейчас мы создадим устройства DirectX. Для начала заполним структуру,
  // которая описывает свойства переднего буфера и привязывает его к нашему окну.
  DXGI_SWAP_CHAIN_DESC sd;                            // Структура, описывающая цепь связи (Swap Chain)
  ZeroMemory(&sd, sizeof(sd));                        // очищаем ее
  sd.BufferCount = 1;                                 // у нас один задний буфер
  sd.BufferDesc.Width = width;                        // ширина буфера
  sd.BufferDesc.Height = height;                      // высота буфера
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // формат пикселя в буфере
  sd.BufferDesc.RefreshRate.Numerator = 60;           // частота обновления экрана
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // назначение буфера - задний буфер
  sd.OutputWindow = hWnd;                             // привязываем к нашему окну
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;  // allow full-screen switching
  sd.Windowed = fullscreen ? FALSE : TRUE;            // не полноэкранный режим

  for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
  {
    D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
    g_driverType = driverTypes[driverTypeIndex];
    D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
    hr = D3D11CreateDeviceAndSwapChain(
      NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
      D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &g_featureLevel, &m_pImmediateContext);
    if (SUCCEEDED(hr)) // Если устройства созданы успешно, то выходим из цикла
      break;
  }
  if (FAILED(hr)) return false;

  // Настройка вьюпорта
  {
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    // Подключаем вьюпорт к контексту устройства
    m_pImmediateContext->RSSetViewports(1, &vp);
  }

  // Теперь создаем задний буфер. Обратите внимание, в SDK
  // RenderTargetOutput - это передний буфер, а RenderTargetView - задний.
  {
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) return false;

    // Я уже упоминал, что интерфейс g_pd3dDevice будет
    // использоваться для создания остальных объектов
    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr)) return false;
  }

  // Создаем текстуру-описание буфера глубин
  static const D3D11_TEXTURE2D_DESC descDepth = {
    (UINT)width,                           // ширина и
    (UINT)height,                          // высота текстуры
    1u,                               // уровень интерполяции
    1u,
    DXGI_FORMAT_D24_UNORM_S8_UINT,   // формат (размер пикселя)
    { 1u, 0u },
    D3D11_USAGE_DEFAULT,
    D3D11_BIND_DEPTH_STENCIL,
    0u,                               // вид - буфер глубин
    0u
  };

  static const D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {
    descDepth.Format,
    D3D11_DSV_DIMENSION_TEXTURE2D,
    0u
  };

  // При помощи заполненной структуры-описания создаем объект текстуры
  hr = m_pd3dDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);
  if (FAILED(hr)) return false;

  // При помощи заполненной структуры-описания и текстуры создаем объект буфера глубин
  hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
  if (FAILED(hr)) return false;

  SetClearColor(0.0f, 0.0f, 0.25f);

  // Подключаем объект заднего буфера и объект буфера глубин к контексту устройства
  m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

  return true;
}

void CDirectX11GraphicsEngine::SetClearColor(float r, float g, float b, float a)
{
  m_ClearColor[0] = r;
  m_ClearColor[1] = g;
  m_ClearColor[2] = b;
  m_ClearColor[3] = a;
}

void CDirectX11GraphicsEngine::ClearScreen()
{
  if (!m_pImmediateContext || !m_pRenderTargetView)
    return;
  // Просто очищаем задний буфер
  m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, m_ClearColor);
  // Очистить буфер глубин до 1.0 (максимальное значение)
  m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CDirectX11GraphicsEngine::Swap()
{
  if (!m_pSwapChain)
    return;
  // Выбросить задний буфер на экран
  m_pSwapChain->Present(0, 0);
}

void CDirectX11GraphicsEngine::Cleanup()
{
  if (m_pSwapChain) m_pSwapChain->SetFullscreenState(FALSE, nullptr);    // switch to windowed mode

  // Сначала отключим контекст устройства, потом отпустим объекты.
  if (m_pImmediateContext) m_pImmediateContext->ClearState();

  // Удалим буффер глубины
  if (m_pDepthStencil) m_pDepthStencil->Release(); m_pDepthStencil = nullptr;
  if (m_pDepthStencilView) m_pDepthStencilView->Release(); m_pDepthStencilView = nullptr;
  // Удалим задний буффер
  if (m_pRenderTargetView) m_pRenderTargetView->Release(); m_pRenderTargetView = nullptr;

  // Удалим свои поля
  if (m_pSwapChain) m_pSwapChain->Release(); m_pSwapChain = nullptr;
  if (m_pImmediateContext) m_pImmediateContext->Release(); m_pImmediateContext = nullptr;
  if (m_pd3dDevice) m_pd3dDevice->Release(); m_pd3dDevice = nullptr;
}

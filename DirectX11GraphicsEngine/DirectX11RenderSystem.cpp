#include "stdafx.h"
#include "DirectX11RenderSystem.h"

#include "IWindowEx.h"

#include <d3dcompiler.h>

#include "DirectX11ShaderSet.h"

CDirectX11RenderSystem::CDirectX11RenderSystem(IWindowEx* pWnd) :
  m_pWindow(pWnd),
  m_pd3dDevice(nullptr),
  m_pImmediateContext(nullptr),
  m_pSwapChain(nullptr),
  m_pRenderTargetView(nullptr),
  m_pDepthStencil(nullptr),
  m_pDepthStencilView(nullptr)
{
}

bool CDirectX11RenderSystem::Init(int width, int height, bool fullscreen)
{
  Cleanup();

  HRESULT hr = S_OK;

  if (!m_pWindow)
    return false;

  if (m_pd3dDevice || m_pImmediateContext || m_pSwapChain || m_pRenderTargetView)
    return false;

  m_pWindow->SetClientSize(width, height);

  if (!CreateDeviceAndSwapChain(width, height, fullscreen))
    return false;

  // Настройка вьюпорта
  SetupViewport((FLOAT)width, (FLOAT)height);

  // Теперь создаем задний буфер. Обратите внимание, в SDK
  if (!CreateBackBuffer())
  {
    Cleanup();
    return false;
  }

  if (!CreateDepthStencilBuffer((UINT)width, (UINT)height))
  {
    Cleanup();
    return false;
  }

  // Подключаем объект заднего буфера и объект буфера глубин к контексту устройства
  m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

  return true;
}

void CDirectX11RenderSystem::Cleanup()
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

bool CDirectX11RenderSystem::ResizeSwapChain(int width, int height)
{
  if (!m_pImmediateContext || !m_pRenderTargetView || !m_pSwapChain)
    return false;

  m_pImmediateContext->OMSetRenderTargets(0, 0, 0);

  // Release all outstanding references to the swap chain's buffers.
  m_pRenderTargetView->Release();

  HRESULT hr;
  // Preserve the existing buffer count and format.
  // Automatically choose the width and height to match the client rect for HWNDs.
  hr = m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

  if (FAILED(hr))
    return false;

  // Get buffer and create a render-target-view.
  ID3D11Texture2D* pBuffer;
  hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
    (void**)&pBuffer);

  if (FAILED(hr))
    return false;

  hr = m_pd3dDevice->CreateRenderTargetView(pBuffer, NULL,
    &m_pRenderTargetView);

  if (FAILED(hr))
    return false;

  pBuffer->Release();

  m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

  // Set up the viewport.
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)width;
  vp.Height = (FLOAT)height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  m_pImmediateContext->RSSetViewports(1, &vp);

  return true;
}

int CDirectX11RenderSystem::GetWidth() const
{
  if (!m_pWindow)
    return 0;

  int w, h;
  m_pWindow->GetClientSize(&w, &h);

  return w;
}

int CDirectX11RenderSystem::GetHeight() const
{
  if (!m_pWindow)
    return 0;

  int w, h;
  m_pWindow->GetClientSize(&w, &h);

  return h;
}

void CDirectX11RenderSystem::ClearScreen(float r, float g, float b, float a)
{
  float clearColor[4] = { r, g, b, a };
  ClearScreen(clearColor);
}

void CDirectX11RenderSystem::ClearScreen(const float * color)
{
  if (!m_pImmediateContext)
    return;

  // Просто очищаем задний буфер
  if (m_pRenderTargetView)
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);
  // Очистить буфер глубин до 1.0 (максимальное значение)
  if (m_pDepthStencilView)
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CDirectX11RenderSystem::SwapBuffers()
{
  if (!m_pSwapChain)
    return;
  // Выбросить задний буфер на экран
  m_pSwapChain->Present(0, 0);
}

ID3D11PixelShader * CDirectX11RenderSystem::CompilePixelShader(const std::string& text, const std::string& PSEntry, int version)
{
  if (!m_pd3dDevice)
    return nullptr;

  char pBuff[32];
  _itoa_s(version, pBuff, 10);

  std::string versionString = "ps_";
  versionString += pBuff;
  versionString += "_0";

  ID3DBlob* pPSBlob = CompileShader(text, PSEntry, versionString);
  if (!pPSBlob)
    return nullptr;

  ID3D11PixelShader* g_pPixelShader = nullptr;          // Пиксельный шейдер
  HRESULT hr = S_OK;

  hr = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);

  if (pPSBlob)
    pPSBlob->Release();

  if (FAILED(hr) || !g_pPixelShader)
    return nullptr;

  return g_pPixelShader;
}

ID3DBlob * CDirectX11RenderSystem::CompileVertexShader(const std::string& text, const std::string& VSEntry, int version)
{
  char pBuff[32];
  _itoa_s(version, pBuff, 10);

  std::string versionString = "vs_";
  versionString += pBuff;
  versionString += "_0";

  return CompileShader(text, VSEntry, versionString);
}

ID3D11VertexShader * CDirectX11RenderSystem::CreateVertexShader(ID3DBlob * pBlob)
{
  if (!m_pd3dDevice || !pBlob)
    return nullptr;

  ID3D11VertexShader* g_pVertexShader = nullptr;        // Вершинный шейдер

  HRESULT hr = m_pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &g_pVertexShader);

  return g_pVertexShader;
}

ID3D11InputLayout * CDirectX11RenderSystem::CreateVertexLayout(const D3D11_INPUT_ELEMENT_DESC * pInputElementDescs, unsigned int NumElements, ID3DBlob * pBlob)
{
  if (!m_pd3dDevice || !pBlob)
    return nullptr;

  ID3D11InputLayout* g_pVertexLayout = nullptr;         // Описание формата вершин

  HRESULT hr = m_pd3dDevice->CreateInputLayout(pInputElementDescs, NumElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &g_pVertexLayout);

  return g_pVertexLayout;
}

ID3D11Buffer * CDirectX11RenderSystem::CreateConstantBuffer(unsigned int bufferSize)
{
  HRESULT hr = S_OK;

  if (!m_pd3dDevice)
    return nullptr;

  ID3D11Buffer* g_pConstantBuffer = nullptr;            // Константный буфер

  D3D11_BUFFER_DESC bd;                       // Структура, описывающая создаваемый буфер
  ZeroMemory(&bd, sizeof(bd));                // очищаем ее
                                              // Создание константного буфера
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = bufferSize;                  // размер буфера = размеру структуры, должен быть кратным 16-ти
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  // тип - константный буфер
  bd.CPUAccessFlags = 0;
  hr = m_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
  if (FAILED(hr) || !g_pConstantBuffer)
    return nullptr;

  return g_pConstantBuffer;
}

void CDirectX11RenderSystem::FillBuffer(ID3D11Buffer* pBuffer, const void * pData)
{
  if (!pBuffer || !pData)
    return;

  // загружаем временную структуру в константный буфер g_pConstantBuffer
  m_pImmediateContext->UpdateSubresource(pBuffer, 0, NULL, pData, 0, 0);
}

ID3D11Buffer * CDirectX11RenderSystem::CreateVertexBuffer(unsigned int bufferSize, void* bufferData)
{
  ID3D11Buffer* result = nullptr;

  if (bufferSize == 0 || bufferData == nullptr)
    return nullptr;

  HRESULT hr = S_OK;
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  D3D11_SUBRESOURCE_DATA InitData;
  ZeroMemory(&InitData, sizeof(InitData));

  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.CPUAccessFlags = 0;
  bd.ByteWidth = bufferSize;
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  InitData.pSysMem = bufferData;

  hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &result);

  if (FAILED(hr)) return nullptr;

  return result;
}

void CDirectX11RenderSystem::DrawVertexBuffer(ID3D11Buffer * pBuff, UINT vertexSize, UINT vertexCount, D3D11_PRIMITIVE_TOPOLOGY topologyType)
{
  ID3D11Buffer* pLocBuff = pBuff;
  UINT offset = 0;

  m_pImmediateContext->IASetVertexBuffers(0, 1, &pLocBuff, &vertexSize, &offset);
  m_pImmediateContext->IASetPrimitiveTopology(topologyType);

  m_pImmediateContext->Draw(vertexCount, 0);
}

void CDirectX11RenderSystem::ApplyShaderSet(CDirectX11ShaderSet * pShaderSet)
{
  ID3D11Buffer* pBuffer = pShaderSet->GetConstantBuffer();

  m_pImmediateContext->IASetInputLayout(pShaderSet->GetInputLayout());
  m_pImmediateContext->VSSetShader(pShaderSet->GetVertexShader(), NULL, 0);
  m_pImmediateContext->VSSetConstantBuffers(0, 1, &pBuffer);
  m_pImmediateContext->PSSetShader(pShaderSet->GetPixelShader(), NULL, 0);
  m_pImmediateContext->PSSetConstantBuffers(0, 1, &pBuffer);
}

ID3DBlob * CDirectX11RenderSystem::CompileShader(const std::string& shaderText, const std::string& entrypoint, const std::string & versionString)
{
  ID3DBlob* pBlob = nullptr;
  ID3DBlob *pErrorBlob = nullptr;
  HRESULT hr = S_OK;

  hr = D3DCompile(shaderText.c_str(), shaderText.length(), nullptr, nullptr, nullptr, entrypoint.c_str(), versionString.c_str(), D3D10_SHADER_ENABLE_STRICTNESS, 0, &pBlob, &pErrorBlob);

  if (pErrorBlob)
  {
    OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
    pErrorBlob->Release();
    pErrorBlob = nullptr;
  }

  if (FAILED(hr) || !pBlob)
    return nullptr;

  return pBlob;
}

bool CDirectX11RenderSystem::CreateDeviceAndSwapChain(int width, int height, bool fullscreen)
{
  HRESULT hr = S_OK;

  HWND hWnd = (HWND)m_pWindow->GetHandle();
  if (!hWnd)
    return false;

  // Сейчас мы создадим устройства DirectX. Для начала заполним структуру,
  // которая описывает свойства переднего буфера и привязывает его к нашему окну.
  DXGI_SWAP_CHAIN_DESC sd;                            // Структура, описывающая цепь связи (Swap Chain)
  ZeroMemory(&sd, sizeof(sd));                        // очищаем ее
  sd.BufferCount = 1;                                 // у нас один задний буфер
  sd.BufferDesc.Width = (UINT)width;                        // ширина буфера
  sd.BufferDesc.Height = (UINT)height;                      // высота буфера
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // формат пикселя в буфере
  sd.BufferDesc.RefreshRate.Numerator = 60;           // частота обновления экрана
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // назначение буфера - задний буфер
  sd.OutputWindow = hWnd;                             // привязываем к нашему окну
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;  // allow full-screen switching
  sd.Windowed = fullscreen ? FALSE : TRUE;            // не полноэкранный режим

  static const UINT createDeviceFlags = 0;

  static const D3D_DRIVER_TYPE driverTypes[] = {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
  };
  static const UINT numDriverTypes = ARRAYSIZE(driverTypes);

  // Тут мы создаем список поддерживаемых версий DirectX
  static const D3D_FEATURE_LEVEL featureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0
  };

  for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
  {
    D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
    g_driverType = driverTypes[driverTypeIndex];
    D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
    hr = D3D11CreateDeviceAndSwapChain(
      NULL, g_driverType, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels),
      D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &g_featureLevel, &m_pImmediateContext);
    if (SUCCEEDED(hr)) // Если устройства созданы успешно, то выходим из цикла
      break;
  }
  if (FAILED(hr)) return false;

  if (!m_pSwapChain || !m_pd3dDevice || !m_pImmediateContext)
  {
    Cleanup();
    return false;
  }

  return true;
}

void CDirectX11RenderSystem::SetupViewport(float width, float height)
{
  if (!m_pImmediateContext)
    return;

  D3D11_VIEWPORT vp;
  vp.Width = width;
  vp.Height = height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  // Подключаем вьюпорт к контексту устройства
  m_pImmediateContext->RSSetViewports(1, &vp);
}

bool CDirectX11RenderSystem::CreateBackBuffer()
{
  // RenderTargetOutput - это передний буфер, а RenderTargetView - задний.
  HRESULT hr = S_OK;

  if (!m_pSwapChain)
    return false;

  ID3D11Texture2D* pBackBuffer = nullptr;
  hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  if (FAILED(hr) || !pBackBuffer) return false;

  hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
  pBackBuffer->Release();
  if (FAILED(hr) || !m_pRenderTargetView) return false;

  return true;
}

bool CDirectX11RenderSystem::CreateDepthStencilBuffer(unsigned int width, unsigned int height)
{
  HRESULT hr = S_OK;

  if (!m_pd3dDevice)
    return false;

  // Создаем текстуру-описание буфера глубин
  static const D3D11_TEXTURE2D_DESC descDepth = {
    width,                           // ширина и
    height,                          // высота текстуры
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
  if (FAILED(hr) || !m_pDepthStencil) return false;

  // При помощи заполненной структуры-описания и текстуры создаем объект буфера глубин
  hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
  if (FAILED(hr))
  {
    m_pDepthStencil->Release();
    m_pDepthStencil = nullptr;
    return false;
  }

  return true;
}

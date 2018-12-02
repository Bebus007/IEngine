#pragma once

class CDirectX11ShaderSet;
class CDirectX11RenderSystem;

static class CDirectX11ShaderLibrary
{
public:
  static CDirectX11ShaderSet* CreateTestShader(CDirectX11RenderSystem*);
};
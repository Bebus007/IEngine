#include <iostream>
#include <Windows.h>
using namespace std;

#include "App.h"

int main()
{
  CApp::Init();

  CApp::Run();

  CApp::Destroy();

  return 0;
}
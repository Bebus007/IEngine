
#include "App.h"

int main(int argc, const char* argv[])
{
  CApp::Init(argc, argv);

  CApp::Run();

  CApp::Destroy();
}
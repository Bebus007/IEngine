#include "stdafx.h"
#include "DirectInputKeyboard.h"

CDirectInputKeyboard::CDirectInputKeyboard(HWND hWnd, LPDIRECTINPUTDEVICE8 device) : CDirectInputDevice(hWnd, device)
{
  GetInputDevice()->SetDataFormat(&c_dfDIKeyboard);

  m_State = new char[256];

  SetStateContainer(m_State, sizeof(char) * 256);
  for (int i = 0; i < 256; i++)
    m_State[i] = 0;
}

CDirectInputKeyboard::~CDirectInputKeyboard()
{
  delete[] m_State;

  SetStateContainer(nullptr, 0);
}

bool CDirectInputKeyboard::IsKeyDown(Key_e key)
{
  Update();

  int keyIdx = GetDirectInputKeyCode(key);

  if (keyIdx < 0 || keyIdx > 255)
    return false;

  return m_State[keyIdx] != 0;
}

int CDirectInputKeyboard::GetDirectInputKeyCode(Key_e key)
{
  switch (key)
  {
  case KEY_ESCAPE:
    return DIK_ESCAPE;
    break;
  case KEY_1:
    return DIK_1;
    break;
  case KEY_2:
    return DIK_2;
    break;
  case KEY_3:
    return DIK_3;
    break;
  case KEY_4:
    return DIK_4;
    break;
  case KEY_5:
    return DIK_5;
    break;
  case KEY_6:
    return DIK_6;
    break;
  case KEY_7:
    return DIK_7;
    break;
  case KEY_8:
    return DIK_8;
    break;
  case KEY_9:
    return DIK_9;
    break;
  case KEY_0:
    return DIK_0;
    break;
  case KEY_MINUS:
    return DIK_MINUS;
    break;
  case KEY_EQUALS:
    return DIK_EQUALS;
    break;
  case KEY_BACK:
    return DIK_BACK;
    break;
  case KEY_TAB:
    return DIK_TAB;
    break;
  case KEY_Q:
    return DIK_Q;
    break;
  case KEY_W:
    return DIK_W;
    break;
  case KEY_E:
    return DIK_E;
    break;
  case KEY_R:
    return DIK_R;
    break;
  case KEY_T:
    return DIK_T;
    break;
  case KEY_Y:
    return DIK_Y;
    break;
  case KEY_U:
    return DIK_U;
    break;
  case KEY_I:
    return DIK_I;
    break;
  case KEY_O:
    return DIK_O;
    break;
  case KEY_P:
    return DIK_P;
    break;
  case KEY_LBRACKET:
    return DIK_LBRACKET;
    break;
  case KEY_RBRACKET:
    return DIK_RBRACKET;
    break;
  case KEY_RETURN:
    return DIK_RETURN;
    break;
  case KEY_LCONTROL:
    return DIK_LCONTROL;
    break;
  case KEY_A:
    return DIK_A;
    break;
  case KEY_S:
    return DIK_S;
    break;
  case KEY_D:
    return DIK_D;
    break;
  case KEY_F:
    return DIK_F;
    break;
  case KEY_G:
    return DIK_G;
    break;
  case KEY_H:
    return DIK_H;
    break;
  case KEY_J:
    return DIK_J;
    break;
  case KEY_K:
    return DIK_K;
    break;
  case KEY_L:
    return DIK_L;
    break;
  case KEY_SEMICOLON:
    return DIK_SEMICOLON;
    break;
  case KEY_APOSTROPHE:
    return DIK_APOSTROPHE;
    break;
  case KEY_GRAVE:
    return DIK_GRAVE;
    break;
  case KEY_LSHIFT:
    return DIK_LSHIFT;
    break;
  case KEY_BACKSLASH:
    return DIK_BACKSLASH;
    break;
  case KEY_Z:
    return DIK_Z;
    break;
  case KEY_X:
    return DIK_X;
    break;
  case KEY_C:
    return DIK_C;
    break;
  case KEY_V:
    return DIK_V;
    break;
  case KEY_B:
    return DIK_B;
    break;
  case KEY_N:
    return DIK_N;
    break;
  case KEY_M:
    return DIK_M;
    break;
  case KEY_COMMA:
    return DIK_COMMA;
    break;
  case KEY_PERIOD:
    return DIK_PERIOD;
    break;
  case KEY_SLASH:
    return DIK_SLASH;
    break;
  case KEY_RSHIFT:
    return DIK_RSHIFT;
    break;
  case KEY_MULTIPLY:
    return DIK_MULTIPLY;
    break;
  case KEY_LMENU:
    return DIK_LMENU;
    break;
  case KEY_SPACE:
    return DIK_SPACE;
    break;
  case KEY_CAPITAL:
    return DIK_CAPITAL;
    break;
  case KEY_F1:
    return DIK_F1;
    break;
  case KEY_F2:
    return DIK_F2;
    break;
  case KEY_F3:
    return DIK_F3;
    break;
  case KEY_F4:
    return DIK_F4;
    break;
  case KEY_F5:
    return DIK_F5;
    break;
  case KEY_F6:
    return DIK_F6;
    break;
  case KEY_F7:
    return DIK_F7;
    break;
  case KEY_F8:
    return DIK_F8;
    break;
  case KEY_F9:
    return DIK_F9;
    break;
  case KEY_F10:
    return DIK_F10;
    break;
  case KEY_NUMLOCK:
    return DIK_NUMLOCK;
    break;
  case KEY_SCROLL:
    return DIK_SCROLL;
    break;
  case KEY_NUMPAD7:
    return DIK_NUMPAD7;
    break;
  case KEY_NUMPAD8:
    return DIK_NUMPAD8;
    break;
  case KEY_NUMPAD9:
    return DIK_NUMPAD9;
    break;
  case KEY_SUBTRACT:
    return DIK_SUBTRACT;
    break;
  case KEY_NUMPAD4:
    return DIK_NUMPAD4;
    break;
  case KEY_NUMPAD5:
    return DIK_NUMPAD5;
    break;
  case KEY_NUMPAD6:
    return DIK_NUMPAD6;
    break;
  case KEY_ADD:
    return DIK_ADD;
    break;
  case KEY_NUMPAD1:
    return DIK_NUMPAD1;
    break;
  case KEY_NUMPAD2:
    return DIK_NUMPAD2;
    break;
  case KEY_NUMPAD3:
    return DIK_NUMPAD3;
    break;
  case KEY_NUMPAD0:
    return DIK_NUMPAD0;
    break;
  case KEY_DECIMAL:
    return DIK_DECIMAL;
    break;
  case KEY_OEM_102:
    return DIK_OEM_102;
    break;
  case KEY_F11:
    return DIK_F11;
    break;
  case KEY_F12:
    return DIK_F12;
    break;
  case KEY_F13:
    return DIK_F13;
    break;
  case KEY_F14:
    return DIK_F14;
    break;
  case KEY_F15:
    return DIK_F15;
    break;
  case KEY_KANA:
    return DIK_KANA;
    break;
  case KEY_ABNT_C1:
    return DIK_ABNT_C1;
    break;
  case KEY_CONVERT:
    return DIK_CONVERT;
    break;
  case KEY_NOCONVERT:
    return DIK_NOCONVERT;
    break;
  case KEY_YEN:
    return DIK_YEN;
    break;
  case KEY_ABNT_C2:
    return DIK_ABNT_C2;
    break;
  case KEY_NUMPADEQUALS:
    return DIK_NUMPADEQUALS;
    break;
  case KEY_PREVTRACK:
    return DIK_PREVTRACK;
    break;
  case KEY_AT:
    return DIK_AT;
    break;
  case KEY_COLON:
    return DIK_COLON;
    break;
  case KEY_UNDERLINE:
    return DIK_UNDERLINE;
    break;
  case KEY_KANJI:
    return DIK_KANJI;
    break;
  case KEY_STOP:
    return DIK_STOP;
    break;
  case KEY_AX:
    return DIK_AX;
    break;
  case KEY_UNLABELED:
    return DIK_UNLABELED;
    break;
  case KEY_NEXTTRACK:
    return DIK_NEXTTRACK;
    break;
  case KEY_NUMPADENTER:
    return DIK_NUMPADENTER;
    break;
  case KEY_RCONTROL:
    return DIK_RCONTROL;
    break;
  case KEY_MUTE:
    return DIK_MUTE;
    break;
  case KEY_CALCULATOR:
    return DIK_CALCULATOR;
    break;
  case KEY_PLAYPAUSE:
    return DIK_PLAYPAUSE;
    break;
  case KEY_MEDIASTOP:
    return DIK_MEDIASTOP;
    break;
  case KEY_VOLUMEDOWN:
    return DIK_VOLUMEDOWN;
    break;
  case KEY_VOLUMEUP:
    return DIK_VOLUMEUP;
    break;
  case KEY_WEBHOME:
    return DIK_WEBHOME;
    break;
  case KEY_NUMPADCOMMA:
    return DIK_NUMPADCOMMA;
    break;
  case KEY_DIVIDE:
    return DIK_DIVIDE;
    break;
  case KEY_SYSRQ:
    return DIK_SYSRQ;
    break;
  case KEY_RMENU:
    return DIK_RMENU;
    break;
  case KEY_PAUSE:
    return DIK_PAUSE;
    break;
  case KEY_HOME:
    return DIK_HOME;
    break;
  case KEY_UP:
    return DIK_UP;
    break;
  case KEY_PRIOR:
    return DIK_PRIOR;
    break;
  case KEY_LEFT:
    return DIK_LEFT;
    break;
  case KEY_RIGHT:
    return DIK_RIGHT;
    break;
  case KEY_END:
    return DIK_END;
    break;
  case KEY_DOWN:
    return DIK_DOWN;
    break;
  case KEY_NEXT:
    return DIK_NEXT;
    break;
  case KEY_INSERT:
    return DIK_INSERT;
    break;
  case KEY_DELETE:
    return DIK_DELETE;
    break;
  case KEY_LWIN:
    return DIK_LWIN;
    break;
  case KEY_RWIN:
    return DIK_RWIN;
    break;
  case KEY_APPS:
    return DIK_APPS;
    break;
  case KEY_POWER:
    return DIK_POWER;
    break;
  case KEY_SLEEP:
    return DIK_SLEEP;
    break;
  case KEY_WAKE:
    return DIK_WAKE;
    break;
  case KEY_WEBSEARCH:
    return DIK_WEBSEARCH;
    break;
  case KEY_WEBFAVORITES:
    return DIK_WEBFAVORITES;
    break;
  case KEY_WEBREFRESH:
    return DIK_WEBREFRESH;
    break;
  case KEY_WEBSTOP:
    return DIK_WEBSTOP;
    break;
  case KEY_WEBFORWARD:
    return DIK_WEBFORWARD;
    break;
  case KEY_WEBBACK:
    return DIK_WEBBACK;
    break;
  case KEY_MYCOMPUTER:
    return DIK_MYCOMPUTER;
    break;
  case KEY_MAIL:
    return DIK_MAIL;
    break;
  case KEY_MEDIASELECT:
    return DIK_MEDIASELECT;
    break;
  default:
    return -1;
  }
}

void CDirectInputKeyboard::Update()
{
  char* tmp = nullptr;

  // update data
  SetStateContainer(m_State, sizeof(char) * 256);
  UpdateStateContainer();
}

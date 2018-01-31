#pragma once

class IMouse
{
public:
  enum Button_e
  {
    BTN_0,
    BTN_1,
    BTN_2,
    BTN_3,
    BTN_4,
    BTN_5,
    BTN_6,
    BTN_7,

    BTN_LEFT = BTN_0,
    BTN_RIGHT = BTN_1,
    BTN_MIDDLE = BTN_2
  };

  enum Axis_e
  {
    AX_X,
    AX_Y,
    AX_Z
  };

  virtual bool IsButtonDown(Button_e) = 0;
  virtual int GetRelativeAxisPosition(Axis_e) = 0;
};
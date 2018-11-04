#pragma once

class IGamepad
{
public:
  virtual bool IsButtonDown(int buttonIdx) = 0;

private:
  enum Axes_e
  {
    GAX_X_AXIS,  // X-axis, usually the left-right movement of a stick.
    GAX_Y_AXIS,  // Y-axis, usually the forward-backward movement of a stick.
    GAX_Z_AXIS,  // Z-axis, often the throttle control.
    GAX_U_AXIS,  // U-axis.
    GAX_V_AXIS,  // V-axis.
    GAX_X_ROTATION_AXIS, // X-axis rotation.
    GAX_Y_ROTATION_AXIS, // Y-axis rotation.
    GAX_Z_ROTATION_AXIS, // Z-axis rotation (often called the rudder).
  };
};

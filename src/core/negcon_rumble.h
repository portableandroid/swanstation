#pragma once
#include "controller.h"
#include <array>
#include <memory>
#include <optional>
#include <string_view>

class NeGconRumble final : public Controller
{
public:
  enum class Axis : u8
  {
    Steering = 0,
    I = 1,
    II = 2,
    L = 3,
    Count
  };

  enum class Button : u8
  {
    Start = 0,
    Up = 1,
    Right = 2,
    Down = 3,
    Left = 4,
    R = 5,
    B = 6,
    A = 7,
    Analog = 8,
    Count
  };

  static constexpr u8 NUM_MOTORS = 2;

  NeGconRumble(u32 index);
  ~NeGconRumble() override;

  static std::unique_ptr<NeGconRumble> Create(u32 index);
  static u32 StaticGetVibrationMotorCount();

  ControllerType GetType() const override;

  void Reset() override;
  bool DoState(StateWrapper& sw, bool ignore_input_state) override;

  void SetAxisState(s32 axis_code, float value) override;
  void SetButtonState(s32 button_code, bool pressed) override;
  u32 GetButtonStateBits() const override;
  std::optional<u32> GetAnalogInputBytes() const override;

  void ResetTransferState() override;
  bool Transfer(const u8 data_in, u8* data_out) override;

  void SetAxisState(Axis axis, u8 value);
  void SetButtonState(Button button, bool pressed);

  u32 GetVibrationMotorCount() const override;
  float GetVibrationMotorStrength(u32 motor) override;

  void LoadSettings(const char* section) override;

private:
  using MotorState = std::array<u8, NUM_MOTORS>;

  enum class Command : u8
  {
    Idle,
    Ready,
    ReadPad,           // 0x42
    ConfigModeSetMode, // 0x43
    SetAnalogMode,     // 0x44
    GetAnalogMode,     // 0x45
    Command46,         // 0x46
    Command47,         // 0x47
    Command4C,         // 0x4C
    GetSetRumble       // 0x4D
  };

  Command m_command = Command::Idle;
  int m_command_step = 0;

  // Transmit and receive buffers, not including the first Hi-Z/ack response byte
  static constexpr u32 MAX_RESPONSE_LENGTH = 8;
  std::array<u8, MAX_RESPONSE_LENGTH> m_rx_buffer;
  std::array<u8, MAX_RESPONSE_LENGTH> m_tx_buffer;
  u32 m_response_length = 0;

  // Get number of response halfwords (excluding the initial controller info halfword)
  u8 GetResponseNumHalfwords() const;

  u8 GetModeID() const;
  u8 GetIDByte() const;

  void SetAnalogMode(bool enabled);
  void ProcessAnalogModeToggle();
  void SetMotorState(u8 motor, u8 value);
  u8 GetExtraButtonMaskLSB() const;
  void ResetRumbleConfig();
  void SetMotorStateForConfigIndex(int index, u8 value);

  u32 m_index;

  u8 m_rumble_bias = 8;

  bool m_analog_mode = false;
  bool m_analog_locked = false;
  bool m_dualshock_enabled = false;
  bool m_configuration_mode = false;

  std::array<u8, static_cast<u8>(Axis::Count)> m_axis_state{};

  static constexpr u8 LargeMotor = 0, SmallMotor = 1;

  std::array<u8, 6> m_rumble_config{};
  int m_rumble_config_large_motor_index = -1;
  int m_rumble_config_small_motor_index = -1;

  bool m_analog_toggle_queued = false;
  u8 m_status_byte = 0x5A;

  // buttons are active low
  u16 m_button_state = UINT16_C(0xFFFF);

  MotorState m_motor_state{};

  // Member variables that are no longer used, but kept and serialized for compatibility with older save states
  float m_steering_deadzone = 0.00f;
  std::string m_twist_response;
};

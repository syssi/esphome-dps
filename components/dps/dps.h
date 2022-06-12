#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/modbus/modbus.h"

namespace esphome {
namespace dps {

class Dps : public PollingComponent, public modbus::ModbusDevice {
 public:
  void set_output_binary_sensor(binary_sensor::BinarySensor *output_binary_sensor) {
    output_binary_sensor_ = output_binary_sensor;
  }
  void set_key_lock_binary_sensor(binary_sensor::BinarySensor *key_lock_binary_sensor) {
    key_lock_binary_sensor_ = key_lock_binary_sensor;
  }
  void set_constant_current_mode_binary_sensor(binary_sensor::BinarySensor *constant_current_mode_binary_sensor) {
    constant_current_mode_binary_sensor_ = constant_current_mode_binary_sensor;
  }

  void set_voltage_setting_number(number::Number *voltage_setting_number) {
    voltage_setting_number_ = voltage_setting_number;
  }
  void set_current_setting_number(number::Number *current_setting_number) {
    current_setting_number_ = current_setting_number;
  }

  void set_output_voltage_sensor(sensor::Sensor *output_voltage_sensor) {
    output_voltage_sensor_ = output_voltage_sensor;
  }
  void set_output_current_sensor(sensor::Sensor *output_current_sensor) {
    output_current_sensor_ = output_current_sensor;
  }
  void set_output_power_sensor(sensor::Sensor *output_power_sensor) { output_power_sensor_ = output_power_sensor; }
  void set_input_voltage_sensor(sensor::Sensor *input_voltage_sensor) { input_voltage_sensor_ = input_voltage_sensor; }
  void set_voltage_setting_sensor(sensor::Sensor *voltage_setting_sensor) {
    voltage_setting_sensor_ = voltage_setting_sensor;
  }
  void set_current_setting_sensor(sensor::Sensor *current_setting_sensor) {
    current_setting_sensor_ = current_setting_sensor;
  }
  void set_backlight_brightness_sensor(sensor::Sensor *backlight_brightness_sensor) {
    backlight_brightness_sensor_ = backlight_brightness_sensor;
  }
  void set_firmware_version_sensor(sensor::Sensor *firmware_version_sensor) {
    firmware_version_sensor_ = firmware_version_sensor;
  }

  void set_output_switch(switch_::Switch *output_switch) { output_switch_ = output_switch; }
  void set_key_lock_switch(switch_::Switch *key_lock_switch) { key_lock_switch_ = key_lock_switch; }

  void set_protection_status_text_sensor(text_sensor::TextSensor *protection_status_text_sensor) {
    protection_status_text_sensor_ = protection_status_text_sensor;
  }
  void set_device_model_text_sensor(text_sensor::TextSensor *device_model_text_sensor) {
    device_model_text_sensor_ = device_model_text_sensor;
  }

  void set_enable_fake_traffic(bool enable_fake_traffic) { enable_fake_traffic_ = enable_fake_traffic; }

  void dump_config() override;

  void on_modbus_data(const std::vector<uint8_t> &data) override;

  void update() override;

  void write_register(uint16_t address, uint16_t value);

 protected:
  binary_sensor::BinarySensor *output_binary_sensor_;
  binary_sensor::BinarySensor *key_lock_binary_sensor_;
  binary_sensor::BinarySensor *constant_current_mode_binary_sensor_;

  number::Number *voltage_setting_number_;
  number::Number *current_setting_number_;

  sensor::Sensor *output_voltage_sensor_;
  sensor::Sensor *output_current_sensor_;
  sensor::Sensor *output_power_sensor_;
  sensor::Sensor *input_voltage_sensor_;
  sensor::Sensor *voltage_setting_sensor_;
  sensor::Sensor *current_setting_sensor_;
  sensor::Sensor *backlight_brightness_sensor_;
  sensor::Sensor *firmware_version_sensor_;

  switch_::Switch *output_switch_;
  switch_::Switch *key_lock_switch_;

  text_sensor::TextSensor *protection_status_text_sensor_;
  text_sensor::TextSensor *device_model_text_sensor_;

  bool enable_fake_traffic_;

  void on_status_data_(const std::vector<uint8_t> &data);
  void Dps::on_acknowledge_data_(const std::vector<uint8_t> &data);
  void publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state);
  void publish_state_(number::Number *number, float value);
  void publish_state_(sensor::Sensor *sensor, float value);
  void publish_state_(switch_::Switch *obj, const bool &state);
  void publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state);
};

}  // namespace dps
}  // namespace esphome

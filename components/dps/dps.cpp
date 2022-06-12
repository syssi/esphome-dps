#include "dps.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace dps {

static const char *const TAG = "dps";

static const uint8_t FUNCTION_READ_REGISTERS = 0x03;
static const uint8_t FUNCTION_WRITE_SINGLE_REGISTER = 0x06;
static const uint8_t FUNCTION_WRITE_MULTIPLE_REGISTERS = 0x10;

static const uint8_t PROTECTION_STATUS_SIZE = 4;
static const char *const PROTECTION_STATUS[PROTECTION_STATUS_SIZE] = {
    "Normal",        // 0x00
    "Over-Voltage",  // 0x01
    "Over-Current",  // 0x02
    "Over-Power",    // 0x03
};

void Dps::on_modbus_data(const std::vector<uint8_t> &data) {
  if (data.size() == 26) {
    this->on_status_data_(data);
    return;
  }

  if (data.size() == 4) {
    this->on_acknowledge_data_(data);
    return;
  }

  ESP_LOGW(TAG, "Invalid size (%zu) for DPS frame!", data.size());
  ESP_LOGW(TAG, "Payload: %s", format_hex_pretty(&data.front(), data.size()).c_str());
}

void Dps::on_acknowledge_data_(const std::vector<uint8_t> &data) {
  auto dps_get_16bit = [&](size_t i) -> uint16_t {
    return (uint16_t(data[i + 0]) << 8) | (uint16_t(data[i + 1]) << 0);
  };

  // Write register acknowledge
  //
  // -> 0x01 0x10 0x00 0x01 0x00 0x01 0x02 0x02 0xF7 0xE7 0x67
  // <- 0x01 0x10 0x00 0x01 0x00 0x01 .... ....

  uint16_t starting_address = dps_get_16bit(0);
  uint16_t registers_written = dps_get_16bit(2);

  if (registers_written == 0) {
    ESP_LOGW(TAG, "Updating register 0x%02X failed", starting_address);
    return;
  }

  ESP_LOGD(TAG, "Acknowledge received: %zu registers written at address 0x%02X", registers_written, starting_address);
}

void Dps::on_status_data_(const std::vector<uint8_t> &data) {
  auto dps_get_16bit = [&](size_t i) -> uint16_t {
    return (uint16_t(data[i + 0]) << 8) | (uint16_t(data[i + 1]) << 0);
  };

  ESP_LOGI(TAG, "Status frame received");

  // Status request (read register 0...13)
  // -> 0x01 0x03 0x00 0x00 0x00 0x0D 0x84 0x0F
  //
  // Status response
  // <- 0x01 0x03 0x1A 0x0E 0x10 0x03 0xE8 0x0E 0x0E 0x00 0xED 0x21 0x4F 0x10 0x87 0x00
  //    0x00 0x00 0x00 0x00 0x00 0x00 0x01 0x00 0x00 0x13 0x9C 0x00 0x11 0x87 0xBD
  //
  // Data: 0x0E 0x10 0x03 0xE8 0x0E 0x0E 0x00 0xED 0x21 0x4F 0x10 0x87 0x00
  //       0x00 0x00 0x00 0x00 0x00 0x00 0x01 0x00 0x00 0x13 0x9C 0x00 0x11
  //
  // *Data*
  //
  // Byte   Address Content: Description                      Decoded content               Coeff./Unit
  //   0    0x0E 0x10        Voltage setting                  3600 * 0.01 = 36.00V          0.01 V
  float voltage_setting = dps_get_16bit(0) * 0.01f;
  this->publish_state_(this->voltage_setting_sensor_, voltage_setting);
  this->publish_state_(this->voltage_setting_number_, voltage_setting);
  //   2    0x03 0xE8        Current setting                  1000 * 0.01 = 10.00A          0.01 A
  float current_setting = dps_get_16bit(2) * 0.01f;
  this->publish_state_(this->current_setting_sensor_, current_setting);
  this->publish_state_(this->current_setting_number_, current_setting);
  //   4    0x0E 0x0E        Output voltage display value     3598 * 0.01 = 35.98V          0.01 V
  this->publish_state_(this->output_voltage_sensor_, (float) dps_get_16bit(4) * 0.01f);
  //   6    0x00 0xED        Output current display value     0237 * 0.01 = 2.37A           0.01 A
  this->publish_state_(this->output_current_sensor_, (float) dps_get_16bit(6) * 0.01f);
  //   8    0x21 0x4F        Output power display value       8527 * 0.01 = 85.27W          0.01 W
  this->publish_state_(this->output_power_sensor_, (float) dps_get_16bit(8) * 0.01f);
  //  10    0x10 0x87        Input voltage display value      4231 * 0.01 = 42.31V          0.01 V
  this->publish_state_(this->input_voltage_sensor_, (float) dps_get_16bit(10) * 0.01f);
  //  12    0x00 0x00        Key lock                         0x00: off, 0x01: on
  bool key_lock = dps_get_16bit(12) == 0x0001;
  this->publish_state_(this->key_lock_binary_sensor_, key_lock);
  this->publish_state_(this->key_lock_switch_, key_lock);
  //  14    0x00 0x00        Protection status                0x00: normal, 0x01: over-voltage,
  //                                                          0x02: over-current, 0x03: over-power
  uint16_t raw_protection_status = data[14];
  if (raw_protection_status < PROTECTION_STATUS_SIZE) {
    this->publish_state_(this->protection_status_text_sensor_, PROTECTION_STATUS[raw_protection_status]);
  } else {
    this->publish_state_(this->protection_status_text_sensor_, "Unknown");
  }
  //  16    0x00 0x00        Constant current (CC mode)       0x00: CV mode, 0x01: CC mode
  this->publish_state_(this->constant_current_mode_binary_sensor_, dps_get_16bit(16) == 0x0001);
  //  18    0x00 0x01        Switch output state              0x00: off, 0x01: on
  bool output = dps_get_16bit(18) == 0x0001;
  this->publish_state_(this->output_binary_sensor_, output);
  this->publish_state_(this->output_switch_, output);
  //  20    0x00 0x00        Backlight brightness level       0...5
  this->publish_state_(this->backlight_brightness_sensor_, dps_get_16bit(20) * 20.0f);
  //  22    0x13 0x9C        Product model                    5020 = DPS5020
  this->publish_state_(this->device_model_text_sensor_, "DPS" + to_string(dps_get_16bit(22)));
  //  24    0x00 0x11        Firmware version                 17 * 0.1 = 1.7
  this->publish_state_(this->firmware_version_sensor_, dps_get_16bit(24) * 0.1f);
}

void Dps::update() {
  // Status request: 0x01 0x03 0x00 0x00 0x00 0x0D 0x84 0x0F
  this->send(FUNCTION_READ_REGISTERS, 0x0000, 0x000D);

  if (this->enable_fake_traffic_) {
    this->on_modbus_data({0x0E, 0x10, 0x03, 0xE8, 0x0E, 0x0E, 0x00, 0xED, 0x21, 0x4F, 0x10, 0x87, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x13, 0x9C, 0x00, 0x11});
  }
}

void Dps::write_register(uint16_t address, uint16_t value) {
  // Output on:    0x01 0x10 0x00 0x09 0x00 0x01 0x02 0x00 0x01 0x67 0x09
  // Output off:   0x01 0x10 0x00 0x09 0x00 0x01 0x02 0x00 0x00 0xa6 0xc9
  // Key lock on:  0x01 0x10 0x00 0x06 0x00 0x01 0x02 0x00 0x01 0x67 0xf6
  // Key lock off: 0x01 0x10 0x00 0x06 0x00 0x01 0x02 0x00 0x00 0xa6 0x36

  // Set voltage to 10V: 0x01 0x10 0x00 0x00 0x00 0x01 0x02 0x03 0xe8 0xa6 0xee
  // Set voltage to 20V: 0x01 0x10 0x00 0x00 0x00 0x01 0x02 0x07 0xd0 0xa5 0xfc
  uint8_t payload[2];
  payload[0] = value >> 8;
  payload[1] = value & 0xff;
  this->send(FUNCTION_WRITE_MULTIPLE_REGISTERS, address, 0x0001, sizeof(payload), payload);
}

void Dps::publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state) {
  if (binary_sensor == nullptr)
    return;

  binary_sensor->publish_state(state);
}

void Dps::publish_state_(number::Number *number, float value) {
  if (number == nullptr)
    return;

  number->publish_state(value);
}

void Dps::publish_state_(sensor::Sensor *sensor, float value) {
  if (sensor == nullptr)
    return;

  sensor->publish_state(value);
}

void Dps::publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state) {
  if (text_sensor == nullptr)
    return;

  text_sensor->publish_state(state);
}

void Dps::publish_state_(switch_::Switch *obj, const bool &state) {
  if (obj == nullptr)
    return;

  obj->publish_state(state);
}

void Dps::dump_config() {  // NOLINT(google-readability-function-size,readability-function-size)
  ESP_LOGCONFIG(TAG, "DPS:");
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", this->address_);
  LOG_BINARY_SENSOR("", "Output", this->output_binary_sensor_);
  LOG_BINARY_SENSOR("", "Key Lock", this->key_lock_binary_sensor_);
  LOG_BINARY_SENSOR("", "Constant Current Mode", this->constant_current_mode_binary_sensor_);
  LOG_SENSOR("", "Output Voltage", this->output_voltage_sensor_);
  LOG_SENSOR("", "Output Current", this->output_current_sensor_);
  LOG_SENSOR("", "Output Power", this->output_power_sensor_);
  LOG_SENSOR("", "Input Voltage", this->input_voltage_sensor_);
  LOG_SENSOR("", "Voltage Setting", this->voltage_setting_sensor_);
  LOG_SENSOR("", "Current Setting", this->current_setting_sensor_);
  LOG_SENSOR("", "Backlight Brightness", this->backlight_brightness_sensor_);
  LOG_SENSOR("", "Firmware Version", this->firmware_version_sensor_);
  LOG_TEXT_SENSOR("", "Protection Status", this->protection_status_text_sensor_);
  LOG_TEXT_SENSOR("", "Device Model", this->device_model_text_sensor_);
}

}  // namespace dps
}  // namespace esphome

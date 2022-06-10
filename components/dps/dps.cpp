#include "dps.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace dps {

static const char *const TAG = "dps";

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

  ESP_LOGW(TAG, "Invalid size (%zu) for DPS frame!", data.size());
  ESP_LOGW(TAG, "Payload: %s", format_hex_pretty(&data.front(), data.size()).c_str());
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
  this->publish_state_(this->voltage_setting_sensor_, (float) dps_get_16bit(0) * 0.01f);
  //   2    0x03 0xE8        Current setting                  1000 * 0.01 = 10.00A          0.01 A
  this->publish_state_(this->current_setting_sensor_, (float) dps_get_16bit(2) * 0.01f);
  //   4    0x0E 0x0E        Output voltage display value     3598 * 0.01 = 35.98V          0.01 V
  this->publish_state_(this->output_voltage_sensor_, (float) dps_get_16bit(4) * 0.01f);
  //   6    0x00 0xED        Output current display value     0237 * 0.01 = 2.37A           0.01 A
  this->publish_state_(this->output_current_sensor_, (float) dps_get_16bit(6) * 0.01f);
  //   8    0x21 0x4F        Output power display value       8527 * 0.01 = 85.27W          0.01 W
  this->publish_state_(this->output_power_sensor_, (float) dps_get_16bit(8) * 0.01f);
  //  10    0x10 0x87        Input voltage display value      4231 * 0.01 = 42.31V          0.01 V
  this->publish_state_(this->input_voltage_sensor_, (float) dps_get_16bit(10) * 0.01f);
  //  12    0x00 0x00        Key lock                         0x00: off, 0x01: on
  this->publish_state_(this->key_lock_binary_sensor_, dps_get_16bit(12) == 0x0001);
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
  this->publish_state_(this->output_binary_sensor_, dps_get_16bit(18) == 0x0001);
  //  20    0x00 0x00        Backlight brightness level       0...5
  this->publish_state_(this->backlight_brightness_sensor_, dps_get_16bit(20) * 20.0f);
  //  22    0x13 0x9C        Product model                    5020 = DPS5020
  this->publish_state_(this->device_model_text_sensor_, "DPS" + to_string(dps_get_16bit(22)));
  //  24    0x00 0x11        Firmware version                 17
  this->publish_state_(this->firmware_version_sensor_, dps_get_16bit(24));
}

void Dps::update() {
  this->send_raw({0x01, 0x03, 0x00, 0x00, 0x00, 0x0D});

  if (this->enable_fake_traffic_) {
    this->on_modbus_data({0x0E, 0x10, 0x03, 0xE8, 0x0E, 0x0E, 0x00, 0xED, 0x21, 0x4F, 0x10, 0x87, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x13, 0x9C, 0x00, 0x11});
  }
}

void Dps::write_register(uint8_t address, uint16_t value) { this->send(0x00, address, value); }

void Dps::publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state) {
  if (binary_sensor == nullptr)
    return;

  binary_sensor->publish_state(state);
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

#pragma once
#include <cstdint>
#include <vector>

namespace esphome::dps::testing {

// Frame sourced from esp8266-example-faker.yaml / esp32-example-faker.yaml
// Decoded:
//   voltage_setting = 36.00 V, current_setting = 10.00 A (LOW resolution, DPS5020)
//   output_voltage = 35.98 V, output_current = 2.37 A, output_power = 35.98 * 2.37 = 85.27 W
//   input_voltage = 42.31 V, key_lock = false, protection_status = "Normal"
//   constant_current_mode = false, output = true, backlight = 0 %
//   model = "DPS5020", firmware_version = 1.7
const std::vector<uint8_t> STATUS_FRAME = {
    0x0E, 0x10,  // [0-1]  voltage_setting: 3600 * 0.01 = 36.00 V
    0x03, 0xE8,  // [2-3]  current_setting: 1000 * 0.01 = 10.00 A
    0x0E, 0x0E,  // [4-5]  output_voltage:  3598 * 0.01 = 35.98 V
    0x00, 0xED,  // [6-7]  output_current:   237 * 0.01 =  2.37 A
    0x21, 0x4F,  // [8-9]  output_power (unused, calculated from V*I)
    0x10, 0x87,  // [10-11] input_voltage: 4231 * 0.01 = 42.31 V
    0x00, 0x00,  // [12-13] key_lock: 0 = false
    0x00, 0x00,  // [14-15] protection_status: 0 = Normal
    0x00, 0x00,  // [16-17] constant_current_mode: 0 = CV mode
    0x00, 0x01,  // [18-19] output: 1 = on
    0x00, 0x00,  // [20-21] backlight_brightness: 0 * 20 = 0 %
    0x13, 0x9C,  // [22-23] model: 5020 = DPS5020
    0x00, 0x11,  // [24-25] firmware_version: 17 * 0.1 = 1.7
};

}  // namespace esphome::dps::testing

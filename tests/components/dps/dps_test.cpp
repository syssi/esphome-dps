#include <gtest/gtest.h>
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "common.h"
#include "frames.h"

namespace esphome::dps::testing {

// Frame sourced from esp8266-example-faker.yaml / esp32-example-faker.yaml

class DpsStatusTest : public ::testing::Test {
 protected:
  TestableDps dps_;
  sensor::Sensor output_voltage_;
  sensor::Sensor output_current_;
  sensor::Sensor output_power_;
  sensor::Sensor input_voltage_;
  sensor::Sensor voltage_setting_;
  sensor::Sensor current_setting_;
  sensor::Sensor backlight_brightness_;
  sensor::Sensor firmware_version_;
  binary_sensor::BinarySensor output_;
  binary_sensor::BinarySensor key_lock_;
  binary_sensor::BinarySensor constant_current_mode_;
  text_sensor::TextSensor protection_status_;
  text_sensor::TextSensor device_model_;

  void SetUp() override {
    dps_.set_output_voltage_sensor(&output_voltage_);
    dps_.set_output_current_sensor(&output_current_);
    dps_.set_output_power_sensor(&output_power_);
    dps_.set_input_voltage_sensor(&input_voltage_);
    dps_.set_voltage_setting_sensor(&voltage_setting_);
    dps_.set_current_setting_sensor(&current_setting_);
    dps_.set_backlight_brightness_sensor(&backlight_brightness_);
    dps_.set_firmware_version_sensor(&firmware_version_);
    dps_.set_output_binary_sensor(&output_);
    dps_.set_key_lock_binary_sensor(&key_lock_);
    dps_.set_constant_current_mode_binary_sensor(&constant_current_mode_);
    dps_.set_protection_status_text_sensor(&protection_status_);
    dps_.set_device_model_text_sensor(&device_model_);
  }
};

TEST_F(DpsStatusTest, VoltageSetting) {
  dps_.on_status_data_(STATUS_FRAME);
  EXPECT_NEAR(voltage_setting_.state, 36.0f, 0.005f);
}

TEST_F(DpsStatusTest, CurrentSetting) {
  dps_.on_status_data_(STATUS_FRAME);
  // Model DPS5020 → LOW resolution (0.01)
  EXPECT_NEAR(current_setting_.state, 10.0f, 0.005f);
}

TEST_F(DpsStatusTest, OutputVoltage) {
  dps_.on_status_data_(STATUS_FRAME);
  EXPECT_NEAR(output_voltage_.state, 35.98f, 0.005f);
}

TEST_F(DpsStatusTest, OutputCurrent) {
  dps_.on_status_data_(STATUS_FRAME);
  EXPECT_NEAR(output_current_.state, 2.37f, 0.005f);
}

TEST_F(DpsStatusTest, OutputPowerCalculated) {
  // Power is calculated from voltage * current, not from the raw register
  dps_.on_status_data_(STATUS_FRAME);
  EXPECT_NEAR(output_power_.state, 35.98f * 2.37f, 0.05f);
}

TEST_F(DpsStatusTest, InputVoltage) {
  dps_.on_status_data_(STATUS_FRAME);
  EXPECT_NEAR(input_voltage_.state, 42.31f, 0.005f);
}

TEST_F(DpsStatusTest, KeyLockFalse) {
  dps_.on_status_data_(STATUS_FRAME);
  EXPECT_FALSE(key_lock_.state);
}

TEST_F(DpsStatusTest, ProtectionStatusNormal) {
  dps_.on_status_data_(STATUS_FRAME);
  EXPECT_EQ(protection_status_.state, "Normal");
}

TEST_F(DpsStatusTest, ConstantCurrentModeFalse) {
  dps_.on_status_data_(STATUS_FRAME);
  EXPECT_FALSE(constant_current_mode_.state);
}

TEST_F(DpsStatusTest, OutputTrue) {
  dps_.on_status_data_(STATUS_FRAME);
  EXPECT_TRUE(output_.state);
}

TEST_F(DpsStatusTest, BacklightBrightness) {
  dps_.on_status_data_(STATUS_FRAME);
  EXPECT_FLOAT_EQ(backlight_brightness_.state, 0.0f);
}

TEST_F(DpsStatusTest, DeviceModelDps5020) {
  dps_.on_status_data_(STATUS_FRAME);
  EXPECT_EQ(device_model_.state, "DPS5020");
}

TEST_F(DpsStatusTest, FirmwareVersion) {
  dps_.on_status_data_(STATUS_FRAME);
  EXPECT_NEAR(firmware_version_.state, 1.7f, 0.05f);
}

TEST_F(DpsStatusTest, NullSensorSafe) {
  TestableDps bare;
  EXPECT_NO_FATAL_FAILURE(bare.on_status_data_(STATUS_FRAME));
}

}  // namespace esphome::dps::testing

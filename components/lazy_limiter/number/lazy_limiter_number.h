#pragma once

#include "../lazy_limiter.h"
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "esphome/core/preferences.h"

namespace esphome {
namespace lazy_limiter {

class LazyLimiter;

class LazyLimiterNumber : public number::Number, public Component {
 public:
  void set_parent(LazyLimiter *parent) { this->parent_ = parent; }
  void set_initial_value(float initial_value) { initial_value_ = initial_value; }
  void set_restore_value(bool restore_value) { this->restore_value_ = restore_value; }
  void set_address(uint8_t address) { this->address_ = address; };

  void setup() override;
  void dump_config() override;

 protected:
  void control(float value) override;

  LazyLimiter *parent_;
  float initial_value_{NAN};
  bool restore_value_{false};
  uint8_t address_;

  ESPPreferenceObject pref_;
};

}  // namespace lazy_limiter
}  // namespace esphome

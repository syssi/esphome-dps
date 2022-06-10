#pragma once

#include "../lazy_limiter.h"
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace lazy_limiter {

enum LazyLimiterSwitchRestoreMode {
  LAZY_LIMITER_SWITCH_RESTORE_DEFAULT_OFF,
  LAZY_LIMITER_SWITCH_RESTORE_DEFAULT_ON,
  LAZY_LIMITER_SWITCH_ALWAYS_OFF,
  LAZY_LIMITER_SWITCH_ALWAYS_ON,
};

class LazyLimiter;

class LazyLimiterSwitch : public switch_::Switch, public Component {
 public:
  void set_parent(LazyLimiter *parent) { this->parent_ = parent; };
  void set_restore_mode(LazyLimiterSwitchRestoreMode restore_mode) { this->restore_mode_ = restore_mode; }

  void setup() override;
  void dump_config() override;

 protected:
  void write_state(bool state) override;
  LazyLimiter *parent_;
  LazyLimiterSwitchRestoreMode restore_mode_{LAZY_LIMITER_SWITCH_RESTORE_DEFAULT_OFF};
};

}  // namespace lazy_limiter
}  // namespace esphome

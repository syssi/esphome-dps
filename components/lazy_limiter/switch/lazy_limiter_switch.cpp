#include "lazy_limiter_switch.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace lazy_limiter {

static const char *const TAG = "lazy_limiter.switch";

void LazyLimiterSwitch::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Lazy Limiter Switch '%s'...", this->name_.c_str());

  bool initial_state = false;
  switch (this->restore_mode_) {
    case LAZY_LIMITER_SWITCH_RESTORE_DEFAULT_OFF:
      initial_state = this->get_initial_state().value_or(false);
      break;
    case LAZY_LIMITER_SWITCH_RESTORE_DEFAULT_ON:
      initial_state = this->get_initial_state().value_or(true);
      break;
    case LAZY_LIMITER_SWITCH_ALWAYS_OFF:
      initial_state = false;
      break;
    case LAZY_LIMITER_SWITCH_ALWAYS_ON:
      initial_state = true;
      break;
  }

  if (initial_state) {
    this->turn_on();
  } else {
    this->turn_off();
  }
}
void LazyLimiterSwitch::dump_config() {
  LOG_SWITCH("", "LazyLimiter Switch", this);
  const LogString *restore_mode = LOG_STR("");
  switch (this->restore_mode_) {
    case LAZY_LIMITER_SWITCH_RESTORE_DEFAULT_OFF:
      restore_mode = LOG_STR("Restore (Defaults to OFF)");
      break;
    case LAZY_LIMITER_SWITCH_RESTORE_DEFAULT_ON:
      restore_mode = LOG_STR("Restore (Defaults to ON)");
      break;
    case LAZY_LIMITER_SWITCH_ALWAYS_OFF:
      restore_mode = LOG_STR("Always OFF");
      break;
    case LAZY_LIMITER_SWITCH_ALWAYS_ON:
      restore_mode = LOG_STR("Always ON");
      break;
  }
  ESP_LOGCONFIG(TAG, "  Restore Mode: %s", LOG_STR_ARG(restore_mode));
}
void LazyLimiterSwitch::write_state(bool state) { this->publish_state(state); }

}  // namespace lazy_limiter
}  // namespace esphome

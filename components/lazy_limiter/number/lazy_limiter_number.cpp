#include "lazy_limiter_number.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lazy_limiter {

static const char *const TAG = "lazy_limiter.number";

void LazyLimiterNumber::setup() {
  float value;
  if (!this->restore_value_) {
    value = this->initial_value_;
  } else {
    this->pref_ = global_preferences->make_preference<float>(this->get_object_id_hash());
    if (!this->pref_.load(&value)) {
      if (!std::isnan(this->initial_value_)) {
        value = this->initial_value_;
      } else {
        value = this->traits.get_min_value();
      }
    }
  }

  if (this->address_ == 0x01) {
    this->parent_->set_max_power_demand((int16_t) value);
  }

  this->publish_state(value);
}

void LazyLimiterNumber::control(float value) {
  if (this->address_ == 0x01) {
    this->parent_->set_max_power_demand((int16_t) value);
  }

  this->publish_state(value);

  if (this->restore_value_)
    this->pref_.save(&value);
}
void LazyLimiterNumber::dump_config() { LOG_NUMBER("", "LazyLimiter Number", this); }

}  // namespace lazy_limiter
}  // namespace esphome

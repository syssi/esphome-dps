#include "dps_number.h"
#include "esphome/core/log.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::dps {

ESPHOME_LOG_TAG(TAG, "dps.number");

void DpsNumber::dump_config() { LOG_NUMBER("", "DPS Number", this); }
void DpsNumber::control(float value) {
  float resolution = 100.0f;

  if (this->holding_register_ == 0x0001) {
    resolution = 1.0f / this->parent_->current_resolution_factor();
  }

  this->parent_->write_register(this->holding_register_, (uint16_t) (value * resolution));
}

}  // namespace esphome::dps

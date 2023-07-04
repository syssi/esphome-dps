#include "dps_number.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dps {

static const char *const TAG = "dps.number";

void DpsNumber::dump_config() { LOG_NUMBER("", "DPS Number", this); }
void DpsNumber::control(float value) {
  float resolution = 100.0f;

  if (this->holding_register_ == 0x0001) {
    resolution = 1.0f / this->parent_->current_resolution_factor();
  }

  this->parent_->write_register(this->holding_register_, (uint16_t) (value * resolution));
}

}  // namespace dps
}  // namespace esphome

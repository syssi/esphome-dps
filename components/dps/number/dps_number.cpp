#include "dps_number.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dps {

static const char *const TAG = "dps.number";

void DpsNumber::control(float value) {
  this->parent_->write_register(this->holding_register_, (uint16_t)(value * (10.0f / this->traits.get_step())));
}
void DpsNumber::dump_config() { LOG_NUMBER(TAG, "DPS Number", this); }

}  // namespace dps
}  // namespace esphome

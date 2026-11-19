#include "dps_switch.h"

namespace esphome::dps {

void DpsSwitch::write_state(bool state) { this->parent_->write_register(this->holding_register_, (uint16_t) state); }

}  // namespace esphome::dps

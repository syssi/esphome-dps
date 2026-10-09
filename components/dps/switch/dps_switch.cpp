#include "dps_switch.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::dps {

ESPHOME_LOG_TAG(TAG, "dps.switch");

void DpsSwitch::dump_config() { LOG_SWITCH("", "DPS Switch", this); }
void DpsSwitch::write_state(bool state) { this->parent_->write_register(this->holding_register_, (uint16_t) state); }

}  // namespace esphome::dps

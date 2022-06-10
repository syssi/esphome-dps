#include "dps_switch.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace dps {

static const char *const TAG = "dps.switch";

void DpsSwitch::dump_config() { LOG_SWITCH("", "Dps Switch", this); }
void DpsSwitch::write_state(bool state) { this->parent_->write_register(this->holding_register_, (uint16_t) state); }

}  // namespace dps
}  // namespace esphome

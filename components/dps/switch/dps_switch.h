#pragma once

#include "../dps.h"
#include "esphome/components/switch/switch.h"

namespace esphome::dps {

class Dps;

class DpsSwitch : public switch_::Switch {
 public:
  void set_parent(Dps *parent) { this->parent_ = parent; };
  void set_holding_register(uint16_t holding_register) { this->holding_register_ = holding_register; };

 protected:
  void write_state(bool state) override;
  Dps *parent_;
  uint16_t holding_register_;
};

}  // namespace esphome::dps

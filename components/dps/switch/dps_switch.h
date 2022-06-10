#pragma once

#include "../dps.h"
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace dps {

class Dps;

class DpsSwitch : public switch_::Switch, public Component {
 public:
  void set_parent(Dps *parent) { this->parent_ = parent; };
  void set_holding_register(uint16_t holding_register) { this->holding_register_ = holding_register; };
  void dump_config() override;
  void loop() override {}
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void write_state(bool state) override;
  Dps *parent_;
  uint16_t holding_register_;
};

}  // namespace dps
}  // namespace esphome

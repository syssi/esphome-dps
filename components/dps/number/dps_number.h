#pragma once

#include "../dps.h"
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "esphome/core/preferences.h"

namespace esphome {
namespace dps {

class Dps;

class DpsNumber : public number::Number, public Component {
 public:
  void set_parent(Dps *parent) { this->parent_ = parent; };
  void set_holding_register(uint16_t holding_register) { this->holding_register_ = holding_register; };
  void dump_config() override;

 protected:
  void control(float value) override;

  Dps *parent_;
  uint16_t holding_register_;
};

}  // namespace dps
}  // namespace esphome

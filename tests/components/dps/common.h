#pragma once
#include <cstdint>
#include <vector>
#include "esphome/components/dps/dps.h"

namespace esphome::dps::testing {

class TestableDps : public Dps {
 public:
  using Dps::on_status_data_;
};

}  // namespace esphome::dps::testing

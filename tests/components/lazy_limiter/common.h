#pragma once
#include "esphome/components/lazy_limiter/lazy_limiter.h"

namespace esphome::lazy_limiter::testing {

class TestableLazyLimiter : public LazyLimiter {
 public:
  using LazyLimiter::calculate_power_demand_oem_;
  using LazyLimiter::calculate_power_demand_negative_measurements_;
};

}  // namespace esphome::lazy_limiter::testing

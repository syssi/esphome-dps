#include <gtest/gtest.h>
#include "common.h"

namespace esphome::lazy_limiter::testing {

class LazyLimiterOemTest : public ::testing::Test {
 protected:
  TestableLazyLimiter limiter_;

  void SetUp() override {
    limiter_.set_max_power_demand(2000);
    limiter_.set_min_power_demand(100);
    limiter_.set_buffer(10);
  }
};

TEST_F(LazyLimiterOemTest, AboveMaxPlusBuffer) {
  // 5000 > max(2000) + buffer(10) → clamp to max
  EXPECT_EQ(limiter_.calculate_power_demand_oem_(5000), 2000);
}

TEST_F(LazyLimiterOemTest, AboveMax) {
  // 2001 > max(2000) → abs(buffer - max) = abs(10 - 2000) = 1990
  EXPECT_EQ(limiter_.calculate_power_demand_oem_(2001), 1990);
}

TEST_F(LazyLimiterOemTest, AboveMin) {
  // 500 >= min(100) → (abs(500-10) + (500-10)) / 2 = 490
  EXPECT_EQ(limiter_.calculate_power_demand_oem_(500), 490);
}

TEST_F(LazyLimiterOemTest, AtMin) {
  // 100 >= min(100) → (abs(100-10) + (100-10)) / 2 = 90
  EXPECT_EQ(limiter_.calculate_power_demand_oem_(100), 90);
}

TEST_F(LazyLimiterOemTest, BelowMin) {
  // 90 < min(100) → 0
  EXPECT_EQ(limiter_.calculate_power_demand_oem_(90), 0);
}

class LazyLimiterNegMeasTest : public ::testing::Test {
 protected:
  TestableLazyLimiter limiter_;

  void SetUp() override {
    limiter_.set_max_power_demand(900);
    limiter_.set_min_power_demand(100);
    limiter_.set_buffer(10);
  }
};

TEST_F(LazyLimiterNegMeasTest, AboveMax) {
  // importing_now=1000, demand=1500 >= max(900) → 900
  EXPECT_EQ(limiter_.calculate_power_demand_negative_measurements_(1010, 500), 900);
}

TEST_F(LazyLimiterNegMeasTest, AtMax) {
  // importing_now=400, demand=900 >= max(900) → 900
  EXPECT_EQ(limiter_.calculate_power_demand_negative_measurements_(410, 500), 900);
}

TEST_F(LazyLimiterNegMeasTest, AboveMin) {
  // importing_now=300, demand=800 < max(900), 800 > min(100) → 800
  EXPECT_EQ(limiter_.calculate_power_demand_negative_measurements_(310, 500), 800);
}

TEST_F(LazyLimiterNegMeasTest, ZeroImporting) {
  // importing_now=-10, demand=490 < max(900), 490 > min(100) → 490... wait:
  // importing_now = 0 - 10 = -10, demand = -10 + 500 = 490 > min(100) → 490
  EXPECT_EQ(limiter_.calculate_power_demand_negative_measurements_(0, 500), 490);
}

TEST_F(LazyLimiterNegMeasTest, Exporting) {
  // importing_now=-500, demand=0 < max(900), 0 > min(100)? No → 0
  EXPECT_EQ(limiter_.calculate_power_demand_negative_measurements_(-490, 500), 0);
}

TEST_F(LazyLimiterNegMeasTest, LargeExport) {
  // importing_now=-700, demand=-200 < max(900), -200 > min(100)? No → 0
  EXPECT_EQ(limiter_.calculate_power_demand_negative_measurements_(-690, 500), 0);
}

}  // namespace esphome::lazy_limiter::testing

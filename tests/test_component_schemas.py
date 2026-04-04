"""Schema structure tests for dps ESPHome component modules."""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

import components.dps as hub  # noqa: E402
from components.dps import (  # noqa: E402
    binary_sensor,
    number,  # noqa: E402
    sensor,
    switch,  # noqa: E402
    text_sensor,
)


class TestHubConstants:
    def test_conf_dps_id_defined(self):
        assert hub.CONF_DPS_ID == "dps_id"

    def test_conf_current_resolution_defined(self):
        assert hub.CONF_CURRENT_RESOLUTION == "current_resolution"


class TestSensorDefs:
    def test_sensor_defs_completeness(self):
        assert sensor.CONF_OUTPUT_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_OUTPUT_CURRENT in sensor.SENSOR_DEFS
        assert sensor.CONF_INPUT_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_VOLTAGE_SETTING in sensor.SENSOR_DEFS
        assert sensor.CONF_CURRENT_SETTING in sensor.SENSOR_DEFS
        assert sensor.CONF_BACKLIGHT_BRIGHTNESS in sensor.SENSOR_DEFS
        assert sensor.CONF_FIRMWARE_VERSION in sensor.SENSOR_DEFS
        assert len(sensor.SENSOR_DEFS) == 8


class TestBinarySensorDefs:
    def test_binary_sensor_defs_completeness(self):
        from esphome.const import CONF_OUTPUT

        assert CONF_OUTPUT in binary_sensor.BINARY_SENSOR_DEFS
        assert binary_sensor.CONF_KEY_LOCK in binary_sensor.BINARY_SENSOR_DEFS
        assert (
            binary_sensor.CONF_CONSTANT_CURRENT_MODE in binary_sensor.BINARY_SENSOR_DEFS
        )
        assert len(binary_sensor.BINARY_SENSOR_DEFS) == 3


class TestTextSensors:
    def test_text_sensors_list(self):
        assert text_sensor.CONF_PROTECTION_STATUS in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_DEVICE_MODEL in text_sensor.TEXT_SENSORS
        assert len(text_sensor.TEXT_SENSORS) == 2


class TestSwitchConstants:
    def test_switches_dict(self):
        from esphome.const import CONF_OUTPUT

        assert CONF_OUTPUT in switch.SWITCHES
        assert switch.CONF_KEY_LOCK in switch.SWITCHES
        assert len(switch.SWITCHES) == 2

    def test_switch_addresses_are_unique(self):
        addresses = list(switch.SWITCHES.values())
        assert len(addresses) == len(set(addresses))


class TestNumberConstants:
    def test_numbers_dict(self):
        assert number.CONF_VOLTAGE_SETTING in number.NUMBERS
        assert number.CONF_CURRENT_SETTING in number.NUMBERS
        assert len(number.NUMBERS) == 2

    def test_number_addresses_are_unique(self):
        addresses = list(number.NUMBERS.values())
        assert len(addresses) == len(set(addresses))

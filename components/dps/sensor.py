import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_VOLTAGE,
    ICON_EMPTY,
    STATE_CLASS_MEASUREMENT,
    UNIT_AMPERE,
    UNIT_EMPTY,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_WATT,
)

from . import CONF_DPS_ID, DPS_COMPONENT_SCHEMA

try:
    from esphome.const import CONF_OUTPUT_POWER
except ImportError:
    CONF_OUTPUT_POWER = "output_power"

DEPENDENCIES = ["dps"]

CODEOWNERS = ["@syssi"]

CONF_OUTPUT_VOLTAGE = "output_voltage"
CONF_OUTPUT_CURRENT = "output_current"
CONF_INPUT_VOLTAGE = "input_voltage"
CONF_VOLTAGE_SETTING = "voltage_setting"
CONF_CURRENT_SETTING = "current_setting"
CONF_BACKLIGHT_BRIGHTNESS = "backlight_brightness"
CONF_FIRMWARE_VERSION = "firmware_version"

ICON_BACKLIGHT_BRIGHTNESS = "mdi:brightness-6"

SENSOR_DEFS = {
    CONF_OUTPUT_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_OUTPUT_CURRENT: {
        "unit_of_measurement": UNIT_AMPERE,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_CURRENT,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_OUTPUT_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_INPUT_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_VOLTAGE_SETTING: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_CURRENT_SETTING: {
        "unit_of_measurement": UNIT_AMPERE,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_CURRENT,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_BACKLIGHT_BRIGHTNESS: {
        "unit_of_measurement": UNIT_PERCENT,
        "icon": ICON_BACKLIGHT_BRIGHTNESS,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_FIRMWARE_VERSION: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
}

CONFIG_SCHEMA = DPS_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(key): sensor.sensor_schema(**kwargs)
        for key, kwargs in SENSOR_DEFS.items()
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_DPS_ID])
    for key in SENSOR_DEFS:
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))

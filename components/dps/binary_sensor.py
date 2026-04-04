import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import CONF_OUTPUT, ENTITY_CATEGORY_DIAGNOSTIC

from . import CONF_DPS_ID, DPS_COMPONENT_SCHEMA

DEPENDENCIES = ["dps"]

CODEOWNERS = ["@syssi"]

# CONF_OUTPUT from const
CONF_KEY_LOCK = "key_lock"
CONF_CONSTANT_CURRENT_MODE = "constant_current_mode"

BINARY_SENSOR_DEFS = {
    CONF_OUTPUT: {"icon": "mdi:power"},
    CONF_KEY_LOCK: {
        "icon": "mdi:play-box-lock-outline",
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_CONSTANT_CURRENT_MODE: {
        "icon": "mdi:current-dc",
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
}

CONFIG_SCHEMA = DPS_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(key): binary_sensor.binary_sensor_schema(**kwargs)
        for key, kwargs in BINARY_SENSOR_DEFS.items()
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_DPS_ID])
    for key in BINARY_SENSOR_DEFS:
        if key in config:
            conf = config[key]
            sens = await binary_sensor.new_binary_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_binary_sensor")(sens))

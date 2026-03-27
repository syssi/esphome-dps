import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv

from . import CONF_LAZY_LIMITER_ID, LAZY_LIMITER_COMPONENT_SCHEMA

DEPENDENCIES = ["lazy_limiter"]

CODEOWNERS = ["@syssi"]

CONF_OPERATION_MODE = "operation_mode"

ICON_OPERATION_MODE = "mdi:heart-pulse"

TEXT_SENSORS = [
    CONF_OPERATION_MODE,
]

CONFIG_SCHEMA = LAZY_LIMITER_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_OPERATION_MODE): text_sensor.text_sensor_schema(
            icon=ICON_OPERATION_MODE,
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_LAZY_LIMITER_ID])
    for key in TEXT_SENSORS:
        if key in config:
            conf = config[key]
            sens = await text_sensor.new_text_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))

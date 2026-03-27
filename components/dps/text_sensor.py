import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import ENTITY_CATEGORY_DIAGNOSTIC, ICON_EMPTY

from . import CONF_DPS_ID, DPS_COMPONENT_SCHEMA

DEPENDENCIES = ["dps"]

CODEOWNERS = ["@syssi"]

CONF_PROTECTION_STATUS = "protection_status"
CONF_DEVICE_MODEL = "device_model"

ICON_PROTECTION_STATUS = "mdi:heart-pulse"

TEXT_SENSORS = [
    CONF_PROTECTION_STATUS,
    CONF_DEVICE_MODEL,
]

CONFIG_SCHEMA = DPS_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_PROTECTION_STATUS): text_sensor.text_sensor_schema(
            icon=ICON_PROTECTION_STATUS
        ),
        cv.Optional(CONF_DEVICE_MODEL): text_sensor.text_sensor_schema(
            icon=ICON_EMPTY,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_DPS_ID])
    for key in TEXT_SENSORS:
        if key in config:
            conf = config[key]
            sens = await text_sensor.new_text_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))

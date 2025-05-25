import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID, ICON_EMPTY

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
            text_sensor.TextSensor, icon=ICON_PROTECTION_STATUS
        ),
        cv.Optional(CONF_DEVICE_MODEL): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon=ICON_EMPTY
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_DPS_ID])
    for key in TEXT_SENSORS:
        if key in config:
            conf = config[key]
            sens = cg.new_Pvariable(conf[CONF_ID])
            await text_sensor.register_text_sensor(sens, conf)
            cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))

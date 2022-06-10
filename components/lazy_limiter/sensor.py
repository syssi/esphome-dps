import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import DEVICE_CLASS_POWER, ICON_EMPTY, UNIT_WATT

from . import CONF_LAZY_LIMITER_ID, LazyLimiter

DEPENDENCIES = ["lazy_limiter"]

CONF_POWER_DEMAND = "power_demand"

# pylint: disable=too-many-function-args
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_LAZY_LIMITER_ID): cv.use_id(LazyLimiter),
        cv.Optional(CONF_POWER_DEMAND): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_LAZY_LIMITER_ID])
    if CONF_POWER_DEMAND in config:
        sens = await sensor.new_sensor(config[CONF_POWER_DEMAND])
        cg.add(hub.set_power_demand_sensor(sens))

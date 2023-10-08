import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import modbus
from esphome.const import CONF_ID

CODEOWNERS = ["@syssi"]

DEPENDENCIES = ["modbus"]
AUTO_LOAD = ["binary_sensor", "number", "sensor", "switch", "text_sensor"]
MULTI_CONF = True

CONF_DPS_ID = "dps_id"
CONF_CURRENT_RESOLUTION = "current_resolution"

dps_ns = cg.esphome_ns.namespace("dps")
Dps = dps_ns.class_("Dps", cg.PollingComponent, modbus.ModbusDevice)

CurrentResolution = dps_ns.enum("CurrentResolution")
CURRENT_RESOLUTION_OPTIONS = {
    "AUTO": CurrentResolution.DPS_CURRENT_RESOLUTION_AUTO,
    "LOW": CurrentResolution.DPS_CURRENT_RESOLUTION_LOW,
    "HIGH": CurrentResolution.DPS_CURRENT_RESOLUTION_HIGH,
}

DPS_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_DPS_ID): cv.use_id(Dps),
    }
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Dps),
            cv.Optional(CONF_CURRENT_RESOLUTION, default="AUTO"): cv.enum(
                CURRENT_RESOLUTION_OPTIONS, upper=True
            ),
        }
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(modbus.modbus_device_schema(0x01))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await modbus.register_modbus_device(var, config)

    cg.add(var.set_current_resolution(config[CONF_CURRENT_RESOLUTION]))

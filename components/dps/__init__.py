import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import modbus
from esphome.const import CONF_ID

AUTO_LOAD = ["modbus", "binary_sensor", "number", "sensor", "switch", "text_sensor"]
CODEOWNERS = ["@syssi"]
MULTI_CONF = True

CONF_DPS_ID = "dps_id"

dps_ns = cg.esphome_ns.namespace("dps")
Dps = dps_ns.class_("Dps", cg.PollingComponent, modbus.ModbusDevice)

DPS_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_DPS_ID): cv.use_id(Dps),
    }
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Dps),
        }
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(modbus.modbus_device_schema(0x01))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await modbus.register_modbus_device(var, config)

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import modbus
from esphome.const import CONF_ID

AUTO_LOAD = ["modbus", "button", "sensor", "switch"]
CODEOWNERS = ["@syssi"]
MULTI_CONF = True

CONF_DPS_ID = "dps_id"
CONF_ENABLE_FAKE_TRAFFIC = "enable_fake_traffic"

dps_ns = cg.esphome_ns.namespace("dps")
Dps = dps_ns.class_("Dps", cg.PollingComponent, modbus.ModbusDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Dps),
            cv.Optional(CONF_ENABLE_FAKE_TRAFFIC, default=False): cv.boolean,
        }
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(modbus.modbus_device_schema(0x01))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await modbus.register_modbus_device(var, config)

    cg.add(var.set_enable_fake_traffic(config[CONF_ENABLE_FAKE_TRAFFIC]))

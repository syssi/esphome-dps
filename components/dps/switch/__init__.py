import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import (
    CONF_ENTITY_CATEGORY,
    CONF_ICON,
    CONF_ID,
    CONF_OUTPUT,
    ENTITY_CATEGORY_CONFIG,
)

from .. import CONF_DPS_ID, DPS_COMPONENT_SCHEMA, dps_ns

DEPENDENCIES = ["dps"]

CODEOWNERS = ["@syssi"]

# CONF_OUTPUT from const
CONF_KEY_LOCK = "key_lock"

ICON_OUTPUT = "mdi:power"
ICON_KEY_LOCK = "mdi:play-box-lock-outline"

SWITCHES = {
    CONF_OUTPUT: 0x0009,
    CONF_KEY_LOCK: 0x0006,
}

DpsSwitch = dps_ns.class_("DpsSwitch", switch.Switch, cg.Component)

CONFIG_SCHEMA = DPS_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_OUTPUT): switch.SWITCH_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(DpsSwitch),
                cv.Optional(CONF_ICON, default=ICON_OUTPUT): cv.icon,
                cv.Optional(
                    CONF_ENTITY_CATEGORY, default=ENTITY_CATEGORY_CONFIG
                ): cv.entity_category,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_KEY_LOCK): switch.SWITCH_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(DpsSwitch),
                cv.Optional(CONF_ICON, default=ICON_KEY_LOCK): cv.icon,
                cv.Optional(
                    CONF_ENTITY_CATEGORY, default=ENTITY_CATEGORY_CONFIG
                ): cv.entity_category,
            }
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_DPS_ID])
    for key, address in SWITCHES.items():
        if key in config:
            conf = config[key]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await switch.register_switch(var, conf)
            cg.add(getattr(hub, f"set_{key}_switch")(var))
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address))

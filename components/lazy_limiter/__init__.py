import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@syssi"]

AUTO_LOAD = ["number", "sensor", "switch", "text_sensor"]
MULTI_CONF = True

CONF_LAZY_LIMITER_ID = "lazy_limiter_id"
CONF_POWER_ID = "power_id"
CONF_POWER_SENSOR_INACTIVITY_TIMEOUT = "power_sensor_inactivity_timeout"
CONF_POWER_DEMAND_CALCULATION = "power_demand_calculation"
CONF_MIN_POWER_DEMAND = "min_power_demand"
CONF_MAX_POWER_DEMAND = "max_power_demand"
CONF_BUFFER = "buffer"
CONF_POWER_DEMAND_DIVIDER = "power_demand_divider"

DEFAULT_BUFFER = 0
DEFAULT_MIN_POWER_DEMAND = 0
DEFAULT_MAX_POWER_DEMAND = 900
DEFAULT_POWER_DEMAND_DIVIDER = 1

lazy_limiter_ns = cg.esphome_ns.namespace("lazy_limiter")
LazyLimiter = lazy_limiter_ns.class_(
    "LazyLimiter",
    cg.PollingComponent,
)

PowerDemandCalculation = lazy_limiter_ns.enum("PowerDemandCalculation")
POWER_DEMAND_CALCULATION_OPTIONS = {
    "DUMB_OEM_BEHAVIOR": PowerDemandCalculation.POWER_DEMAND_CALCULATION_DUMB_OEM_BEHAVIOR,
    "NEGATIVE_MEASUREMENTS_REQUIRED": PowerDemandCalculation.POWER_DEMAND_CALCULATION_NEGATIVE_MEASUREMENTS_REQUIRED,
}


LAZY_LIMITER_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_LAZY_LIMITER_ID): cv.use_id(LazyLimiter),
    }
)


def validate_min_max(config):
    max_power_demand = cv.int_(config[CONF_MAX_POWER_DEMAND])
    min_power_demand = cv.int_(config[CONF_MIN_POWER_DEMAND])
    if (max_power_demand - min_power_demand) < 0:
        raise cv.Invalid(
            "Maximum power demand must be greater than minimum power demand."
        )

    return config


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(LazyLimiter),
            cv.Optional(
                CONF_POWER_DEMAND_CALCULATION, default="NEGATIVE_MEASUREMENTS_REQUIRED"
            ): cv.enum(POWER_DEMAND_CALCULATION_OPTIONS, upper=True),
            cv.Required(CONF_POWER_ID): cv.use_id(sensor.Sensor),
            cv.Optional(
                CONF_POWER_SENSOR_INACTIVITY_TIMEOUT, default="20s"
            ): cv.positive_time_period_seconds,
            cv.Optional(CONF_BUFFER, default=DEFAULT_BUFFER): cv.int_range(
                min=-200, max=200
            ),
            cv.Optional(
                CONF_POWER_DEMAND_DIVIDER, default=DEFAULT_POWER_DEMAND_DIVIDER
            ): cv.int_range(min=1, max=6),
            cv.Optional(
                CONF_MIN_POWER_DEMAND, default=DEFAULT_MIN_POWER_DEMAND
            ): cv.int_range(min=0, max=2000),
            cv.Optional(
                CONF_MAX_POWER_DEMAND, default=DEFAULT_MAX_POWER_DEMAND
            ): cv.int_range(min=1, max=2000),
        }
    ).extend(cv.polling_component_schema("3s")),
    validate_min_max,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    power_sensor = await cg.get_variable(config[CONF_POWER_ID])

    cg.add(var.set_power_sensor(power_sensor))
    cg.add(var.set_buffer(config[CONF_BUFFER]))
    cg.add(var.set_min_power_demand(config[CONF_MIN_POWER_DEMAND]))
    cg.add(var.set_max_power_demand(config[CONF_MAX_POWER_DEMAND]))
    cg.add(var.set_power_demand_divider(config[CONF_POWER_DEMAND_DIVIDER]))
    cg.add(
        var.set_power_sensor_inactivity_timeout(
            config[CONF_POWER_SENSOR_INACTIVITY_TIMEOUT]
        )
    )
    cg.add(var.set_power_demand_calculation(config[CONF_POWER_DEMAND_CALCULATION]))

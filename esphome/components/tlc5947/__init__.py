import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import (CONF_CLOCK_PIN, CONF_DATA_PIN, CONF_ID,
                           CONF_NUM_CHIPS, CONF_SEL_PIN)

AUTO_LOAD = ['output']
tlc5947_ns = cg.esphome_ns.namespace('tlc5947')
TLC5947 = tlc5947_ns.class_('TLC5947', cg.Component)

MULTI_CONF = True
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(TLC5947),
    cv.Required(CONF_DATA_PIN): pins.gpio_output_pin_schema,
    cv.Required(CONF_CLOCK_PIN): pins.gpio_output_pin_schema,
    cv.Required(CONF_SEL_PIN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_NUM_CHIPS, default=1): cv.int_range(min=1, max=85),
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    data = yield cg.gpio_pin_expression(config[CONF_DATA_PIN])
    cg.add(var.set_data_pin(data))
    clock = yield cg.gpio_pin_expression(config[CONF_CLOCK_PIN])
    cg.add(var.set_clock_pin(clock))
    latch = yield cg.gpio_pin_expression(config[CONF_SEL_PIN])
    cg.add(var.set_latch_pin(latch))
    cg.add(var.set_num_chips(config[CONF_NUM_CHIPS]))

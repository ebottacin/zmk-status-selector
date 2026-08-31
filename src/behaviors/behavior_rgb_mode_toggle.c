#define DT_DRV_COMPAT zmk_behavior_rgb_mode_toggle

#include <zephyr/device.h>

#include <drivers/behavior.h>

#include <zmk/behavior.h>

#include "rgb_mode.h"

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

struct behavior_rgb_mode_toggle_config {
    const char *rgb_mode_behavior_dev;
};

#define RGB_MODE_TOGGLE_INST(n)                                                                 \
    static const struct behavior_rgb_mode_toggle_config rgb_mode_toggle_config_##n = {          \
        .rgb_mode_behavior_dev = DEVICE_DT_NAME(DT_INST_PHANDLE(n, rgb_mode)),                  \
    };                                                                                           \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, NULL, &rgb_mode_toggle_config_##n, POST_KERNEL,      \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                \
                            &behavior_rgb_mode_toggle_driver_api);

static int on_rgb_mode_toggle_pressed(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_rgb_mode_toggle_config *config = dev->config;
    struct zmk_behavior_binding rgb_mode_binding = {
        .behavior_dev = config->rgb_mode_behavior_dev,
    };

    return zmk_rgb_mode_toggle(&rgb_mode_binding, event);
}

static int on_rgb_mode_toggle_released(struct zmk_behavior_binding *binding,
                                       struct zmk_behavior_binding_event event) {
    ARG_UNUSED(binding);
    ARG_UNUSED(event);
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_rgb_mode_toggle_driver_api = {
    .binding_pressed = on_rgb_mode_toggle_pressed,
    .binding_released = on_rgb_mode_toggle_released,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

DT_INST_FOREACH_STATUS_OKAY(RGB_MODE_TOGGLE_INST)

#endif
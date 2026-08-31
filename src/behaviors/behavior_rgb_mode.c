#define DT_DRV_COMPAT zmk_behavior_rgb_mode

#include <errno.h>

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <drivers/behavior.h>

#include <zmk/behavior.h>
#include <zmk/keymap.h>

#include "rgb_mode.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

struct behavior_rgb_mode_config {
    struct zmk_behavior_binding bindings[2];
};

struct behavior_rgb_mode_data {
    bool enabled;
    struct k_work apply_work;
    struct zmk_behavior_binding selected_binding;
    struct zmk_behavior_binding_event event;
};

static void rgb_mode_apply_work(struct k_work *work) {
    struct behavior_rgb_mode_data *data =
        CONTAINER_OF(work, struct behavior_rgb_mode_data, apply_work);

    zmk_behavior_invoke_binding(&data->selected_binding, data->event, true);
}

static int behavior_rgb_mode_init(const struct device *dev) {
    struct behavior_rgb_mode_data *data = dev->data;

    k_work_init(&data->apply_work, rgb_mode_apply_work);
    return 0;
}

#define RGB_MODE_BINDINGS(node_id)                                                               \
    {LISTIFY(DT_PROP_LEN(node_id, bindings), ZMK_KEYMAP_EXTRACT_BINDING, (, ), node_id)}

#define RGB_MODE_INST(n)                                                                         \
    BUILD_ASSERT(DT_INST_PROP_LEN(n, bindings) == 2,                                             \
                 "zmk,behavior-rgb-mode requires bindings in ON, OFF order");                  \
    static const struct behavior_rgb_mode_config rgb_mode_config_##n = {                         \
        .bindings = RGB_MODE_BINDINGS(DT_DRV_INST(n)),                                           \
    };                                                                                           \
    static struct behavior_rgb_mode_data rgb_mode_data_##n;                                     \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_rgb_mode_init, NULL, &rgb_mode_data_##n,                \
                            &rgb_mode_config_##n,                                                 \
                            POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                   \
                            &behavior_rgb_mode_driver_api);

int zmk_rgb_mode_apply(struct zmk_behavior_binding *binding,
                       struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    if (dev == NULL) {
        LOG_ERR("RGB mode behavior is unavailable");
        return -ENODEV;
    }

    const struct behavior_rgb_mode_config *config = dev->config;
    struct behavior_rgb_mode_data *data = dev->data;

    data->selected_binding = config->bindings[data->enabled ? 0 : 1];
    data->event = event;
    k_work_submit(&data->apply_work);

    return ZMK_BEHAVIOR_OPAQUE;
}

int zmk_rgb_mode_toggle(struct zmk_behavior_binding *binding,
                        struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    struct behavior_rgb_mode_data *data = dev->data;

    data->enabled = !data->enabled;
    LOG_INF("RGB mode %s", data->enabled ? "enabled" : "disabled");
    ARG_UNUSED(event);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_rgb_mode_pressed(struct zmk_behavior_binding *binding,
                               struct zmk_behavior_binding_event event) {
    return zmk_rgb_mode_apply(binding, event);
}

static int on_rgb_mode_released(struct zmk_behavior_binding *binding,
                                struct zmk_behavior_binding_event event) {
    ARG_UNUSED(binding);
    ARG_UNUSED(event);
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_rgb_mode_driver_api = {
    .binding_pressed = on_rgb_mode_pressed,
    .binding_released = on_rgb_mode_released,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

DT_INST_FOREACH_STATUS_OKAY(RGB_MODE_INST)

#endif
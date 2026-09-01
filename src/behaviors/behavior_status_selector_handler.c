#define DT_DRV_COMPAT zmk_behavior_status_selector_handler

#include <errno.h>

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <drivers/behavior.h>

#include <zmk/behavior.h>
#include <zmk/keymap.h>

#include "status_selector.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

struct behavior_status_selector_handler_config {
    struct zmk_behavior_binding bindings[2];
};

struct behavior_status_selector_handler_data {
    bool enabled;
    struct k_work apply_work;
    struct zmk_behavior_binding selected_binding;
    struct zmk_behavior_binding_event event;
};

static void status_selector_apply_work(struct k_work *work) {
    struct behavior_status_selector_handler_data *data =
        CONTAINER_OF(work, struct behavior_status_selector_handler_data, apply_work);

    zmk_behavior_invoke_binding(&data->selected_binding, data->event, true);
}

static int behavior_status_selector_handler_init(const struct device *dev) {
    struct behavior_status_selector_handler_data *data = dev->data;

    k_work_init(&data->apply_work, status_selector_apply_work);
    return 0;
}

#define STATUS_SELECTOR_BINDINGS(node_id)                                                        \
    {LISTIFY(DT_PROP_LEN(node_id, bindings), ZMK_KEYMAP_EXTRACT_BINDING, (, ), node_id)}

#define STATUS_SELECTOR_HANDLER_INST(n)                                                         \
    BUILD_ASSERT(DT_INST_PROP_LEN(n, bindings) == 2,                                             \
                 "status selector requires bindings in selected, unselected order");           \
    static const struct behavior_status_selector_handler_config status_selector_config_##n = {   \
        .bindings = STATUS_SELECTOR_BINDINGS(DT_DRV_INST(n)),                                    \
    };                                                                                           \
    static struct behavior_status_selector_handler_data status_selector_data_##n;               \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_status_selector_handler_init, NULL,                     \
                            &status_selector_data_##n, &status_selector_config_##n,             \
                            POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                   \
                            &behavior_status_selector_handler_driver_api);

int zmk_status_selector_apply(struct zmk_behavior_binding *binding,
                              struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    if (dev == NULL) {
        LOG_ERR("Status selector handler is unavailable");
        return -ENODEV;
    }

    const struct behavior_status_selector_handler_config *config = dev->config;
    struct behavior_status_selector_handler_data *data = dev->data;

    data->selected_binding = config->bindings[data->enabled ? 0 : 1];
    data->event = event;
    k_work_submit(&data->apply_work);

    return ZMK_BEHAVIOR_OPAQUE;
}

int zmk_status_selector_toggle(struct zmk_behavior_binding *binding,
                               struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    struct behavior_status_selector_handler_data *data = dev->data;

    data->enabled = !data->enabled;
    LOG_INF("Status selector %s", data->enabled ? "selected" : "unselected");
    ARG_UNUSED(event);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_status_selector_handler_pressed(struct zmk_behavior_binding *binding,
                                              struct zmk_behavior_binding_event event) {
    return zmk_status_selector_apply(binding, event);
}

static int on_status_selector_handler_released(struct zmk_behavior_binding *binding,
                                               struct zmk_behavior_binding_event event) {
    ARG_UNUSED(binding);
    ARG_UNUSED(event);
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_status_selector_handler_driver_api = {
    .binding_pressed = on_status_selector_handler_pressed,
    .binding_released = on_status_selector_handler_released,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

DT_INST_FOREACH_STATUS_OKAY(STATUS_SELECTOR_HANDLER_INST)

#endif
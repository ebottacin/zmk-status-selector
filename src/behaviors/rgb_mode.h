#pragma once

#include <zmk/behavior.h>

int zmk_rgb_mode_apply(struct zmk_behavior_binding *binding,
                       struct zmk_behavior_binding_event event);
int zmk_rgb_mode_toggle(struct zmk_behavior_binding *binding,
                        struct zmk_behavior_binding_event event);
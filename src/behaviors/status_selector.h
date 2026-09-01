#pragma once

#include <zmk/behavior.h>

int zmk_status_selector_apply(struct zmk_behavior_binding *binding,
                              struct zmk_behavior_binding_event event);
int zmk_status_selector_toggle(struct zmk_behavior_binding *binding,
                               struct zmk_behavior_binding_event event);
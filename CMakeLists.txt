if ((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
  target_sources_ifdef(CONFIG_ZMK_BEHAVIOR_STATUS_SELECTOR_HANDLER app PRIVATE src/behaviors/behavior_status_selector_handler.c)
  target_sources_ifdef(CONFIG_ZMK_BEHAVIOR_STATUS_SELECTOR_TOGGLE app PRIVATE src/behaviors/behavior_status_selector_toggle.c)
endif()
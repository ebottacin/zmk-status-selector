if ((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
  target_sources_ifdef(CONFIG_ZMK_BEHAVIOR_RGB_MODE app PRIVATE src/behaviors/behavior_rgb_mode.c)
  target_sources_ifdef(CONFIG_ZMK_BEHAVIOR_RGB_MODE_TOGGLE app PRIVATE src/behaviors/behavior_rgb_mode_toggle.c)
endif()
# zmk-rgb-mode

`zmk-rgb-mode` provides an in-memory ON/OFF RGB mode for ZMK.

## Configuration

```dts
behaviors {
	rgb_l0_mode: rgb_l0_mode {
		compatible = "zmk,behavior-rgb-mode";
		#binding-cells = <0>;
		bindings = <&rgb_layer0 &rgb_layer0_off>;
	};

	rgb_l0_mode_toggle: rgb_l0_mode_toggle {
		compatible = "zmk,behavior-rgb-mode-toggle";
		#binding-cells = <0>;
		rgb-mode = <&rgb_l0_mode>;
	};
};
```

The mode behavior applies the first binding while enabled and the second while disabled. Its initial
state is disabled and is not persisted across a reset or power loss. Use the toggle behavior as a
normal key binding to invert the state; invoke the mode behavior from a layer listener to apply it.

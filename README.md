# zmk-status-selector

`zmk-status-selector` provides an in-memory selected/unselected state for ZMK behaviors.

## Configuration

```dts
behaviors {
	status_handler: status_handler {
		compatible = "zmk,behavior-status-selector-handler";
		#binding-cells = <0>;
		bindings = <&selected_binding &unselected_binding>;
	};

	status_toggle: status_toggle {
		compatible = "zmk,behavior-status-selector-toggle";
		#binding-cells = <0>;
		handler = <&status_handler>;
	};
};
```

The handler applies the first binding while selected and the second while unselected. Its initial
state is unselected and is not persisted across a reset or power loss. Use the toggle behavior as a
normal key binding to invert the state; invoke the handler from a listener to apply it.

# zmk-status-selector

## What This Module Provides

`zmk-status-selector` provides a generic in-memory selected/unselected state for ZMK behaviors.

- selects one of two configured bindings
- provides a zero-parameter behavior that toggles the selected state
- leaves the application of the selected binding to the handler behavior
- keeps state local to the central side for split keyboards

The initial state is unselected. State is not persisted across reset or power loss.

### Split vs single-board support

- Split boards: the handler and toggle are compiled on the central side. Invoke the handler from
	central-side event handlers or listeners.
- Single-board builds: the same handler and toggle run locally without split transport.

## Main Configuration

Behavior nodes:

| Node | Type | Behavior |
| --- | --- | --- |
| Handler | `zmk,behavior-status-selector-handler` | Applies binding 0 when selected, binding 1 when unselected. |
| Toggle | `zmk,behavior-status-selector-toggle` | Inverts the state of its configured handler. |

Handler properties:

| Property | Type | Required | Behavior |
| --- | --- | --- | --- |
| `bindings` | phandle-array | Yes | Exactly two bindings: selected first, unselected second. |

Toggle properties:

| Property | Type | Required | Behavior |
| --- | --- | --- | --- |
| `handler` | phandle | Yes | Handler node whose state is toggled. |

## Example

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

Use the toggle behavior as a normal key binding. Invoke the handler from a listener or another
behavior when the selected binding must be applied.

## west.yml Integration Example

```yaml
manifest:
	remotes:
		- name: ebottacin
			url-base: https://github.com/ebottacin
	projects:
		- name: zmk-status-selector
			remote: ebottacin
			revision: main
```

## Build and Technical Notes

- Add the module to the ZMK West manifest before building.
- `CONFIG_ZMK_BEHAVIOR_STATUS_SELECTOR_HANDLER` and
	`CONFIG_ZMK_BEHAVIOR_STATUS_SELECTOR_TOGGLE` are enabled automatically when their compatible
	nodes are present in the devicetree.

# GlowOS

Adds a lot of extra functionality to WLED for projects designed by glowingkitty. Especially a lot of functions for battery-powered projects.

## Installation

define `USERMOD_GLOWOS` in `my_config.h`

### Define Your Options

* `USERMOD_GLOWOS`                                 - define this (in `my_config.h`) to have this usermod included wled00\usermods_list.cpp
* `USERMOD_GLOWOS_DEVICE_NAME`                - define the name of the device (designed by glowingkitty), to load the correct default settings

If you want to use GlowOS with your own device (not designed by glowingkitty), you can also define the variables yourself:
* `USERMOD_BATTERY_MEASUREMENT_PIN`                - define pin number, where you have battery measurement via voltage divider connected
* `USERMOD_BATTERY_MEASUREMENT_INTERVAL`           - battery check interval. defaults to 30 seconds
* `USERMOD_BATTERY_MIN_VOLTAGE`                    - minimum battery voltage. default is 3.0 (recommended minimum to extend total battery life)
* `USERMOD_BATTERY_MAX_VOLTAGE`                    - maximum battery voltage. default is 4.2 (18650 battery standard)

All parameters can be configured at runtime via the Usermods settings page.

## Important :warning:
* Make sure you know your battery specifications! All batteries are **NOT** the same!
* Example:

| Your battery specification table  |                 | Options you can define        | 
| :-------------------------------- |:--------------- | :---------------------------- |
| Capacity                          | 3500mAh 12,5 Wh |                               |
| Minimum capacity                  | 3350mAh 11,9 Wh |                               |
| Rated voltage                     | 3.6V - 3.7V     |                               |
| **Charging end voltage**          | **4,2V ± 0,05** | `USERMOD_BATTERY_MAX_VOLTAGE` |
| **Discharge voltage**             | **2,5V**        | `USERMOD_BATTERY_MIN_VOLTAGE` |
| Max. discharge current (constant) | 10A (10000mA)   |                               |
| max. charging current             | 1.7A (1700mA)   |                               |
| ...                               | ...             | ...                           |
| ..                                | ..              | ..                            |

Specification from:  [Molicel INR18650-M35A, 3500mAh 10A Lithium-ion battery, 3.6V - 3.7V](https://www.akkuteile.de/lithium-ionen-akkus/18650/molicel/molicel-inr18650-m35a-3500mah-10a-lithium-ionen-akku-3-6v-3-7v_100833)


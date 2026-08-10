# Meshtastic DIY V1 + Elecrow LR1262

This variant keeps the Meshtastic ESP32 DIY V1 pinout and replaces the generic/E22 radio setup with a configuration specific to the Elecrow LR1262 (Semtech SX1262).

## Wiring

| LR1262 signal | ESP32 / connection |
|---|---|
| VDD_3V3 | 3.3 V |
| GND | GND |
| LR_NSS | GPIO18 |
| LR_SCK | GPIO5 |
| LR_MISO | GPIO19 |
| LR_MOSI | GPIO27 |
| LR_NRESET | GPIO23 |
| LR_DIO1 | GPIO33 |
| LR_BUSY | GPIO32 |
| LR_DIO2 | Connect locally to LR1262 TX/RX_EN |
| TX/RX_EN | Connect locally to LR1262 DIO2 |
| VDD_SW | GPIO13 |
| DIO3 | No MCU wire; it powers the module TCXO internally |

Do **not** wire LR1262 physical DIO2 to GPIO32. GPIO32 is the Meshtastic DIY V1 `LORA_DIO2` name, but for SX1262 it is used as the dedicated **BUSY** input.

## VDD_SW behavior

`VDD_SW` powers the LR1262 module's internal RF switch. This variant maps it to GPIO13 through:

```cpp
#define LR1262_VDD_SW 13
#define SX126X_POWER_EN LR1262_VDD_SW
```

Meshtastic already drives `SX126X_POWER_EN` HIGH before `lora.begin()` and LOW in `SX126xInterface::sleep()`.

This patch series adds `SX126X_POWER_EN_RTC_HOLD`. On ESP32, after VDD_SW is driven LOW for deep sleep, the driver calls `rtc_gpio_hold_en()` so GPIO13 remains latched LOW while the CPU is asleep. Meshtastic's existing `initDeepSleep()` releases RTC GPIO holds after wake.

A 47 kOhm to 100 kOhm pulldown from VDD_SW to GND is still recommended on a custom PCB. It keeps the RF switch off during reset/bootloader time before firmware configures GPIO13.

## RF switch behavior

Wire LR1262 `DIO2` directly to LR1262 `TX/RX_EN`. The variant defines:

```cpp
#define SX126X_DIO2_AS_RF_SWITCH
#define SX126X_RXEN RADIOLIB_NC
#define SX126X_TXEN RADIOLIB_NC
```

Meshtastic/RadioLib therefore lets the SX1262 itself drive DIO2 for the module's TX/RX switch rather than consuming another ESP32 pin.

## Build

From the root of a Meshtastic firmware checkout:

```sh
pio run -e meshtastic-diy-lr1262
```

Upload over the normal ESP32 serial connection:

```sh
pio run -e meshtastic-diy-lr1262 -t upload
```

The root Meshtastic `platformio.ini` already includes `variants/*/diy/*/platformio.ini`, so no top-level PlatformIO edit is required.

## Apply the supplied patch series

From the root of your fork:

```sh
git checkout develop
git am /path/to/patches/0001-esp32-add-diy-lr1262-variant.patch
git am /path/to/patches/0002-sx126x-hold-power-enable-low-in-deep-sleep.patch
```

Or apply the combined mail patch:

```sh
git am /path/to/meshtastic-diy-lr1262-combined.patch
```

If upstream `develop` changes the surrounding `SX126xInterface::sleep()` code, the second patch may need a trivial rebase.

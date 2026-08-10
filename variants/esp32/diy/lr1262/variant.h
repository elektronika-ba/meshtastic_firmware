// Meshtastic DIY V1 pinout adapted for the Elecrow LR1262 (SX1262) module.
//
// LR1262 wiring notes:
//   BUSY     -> ESP32 GPIO32
//   DIO1     -> ESP32 GPIO33
//   DIO2     -> LR1262 TX/RX_EN (local connection; not an ESP32 GPIO)
//   VDD_SW   -> ESP32 GPIO13
//   DIO3     -> internal TCXO supply; no ESP32 connection
//
// GPIO13 is intentionally reused from the stock DIY V1 SX126X_TXEN assignment.
// On LR1262, DIO2 controls TX/RX_EN, so external MCU TXEN/RXEN are not used.

// OLED / I2C
#define I2C_SDA 21
#define I2C_SCL 22

// GPS - preserved from DIY V1
#undef GPS_RX_PIN
#undef GPS_TX_PIN
#define GPS_RX_PIN 12
#define GPS_TX_PIN 15
#define GPS_UBLOX

// DIY V1 board functions
#define BUTTON_PIN 39
#define BATTERY_PIN 35
#define ADC_CHANNEL ADC_CHANNEL_7
#define ADC_MULTIPLIER 1.85
#define EXT_PWR_DETECT 4
#define EXT_NOTIFY_OUT 12
#define LED_POWER 2

// LoRa SPI / control pins
#define LORA_DIO0 26 // Not used by SX1262; retained for DIY V1 compatibility
#define LORA_RESET 23
#define LORA_DIO1 33 // SX1262 DIO1 / IRQ
#define LORA_DIO2 32 // Legacy DIY V1 name: this is SX1262 BUSY, not physical DIO2
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_CS 18

// LR1262 RF-switch supply control. Meshtastic's SX126x driver drives
// SX126X_POWER_EN high before lora.begin() and low when the radio enters
// deep sleep. GPIO13 is an RTC-capable GPIO on the original ESP32.
#define LR1262_VDD_SW 13
#define SX126X_POWER_EN LR1262_VDD_SW

// Opt in to the small core patch supplied with this variant so VDD_SW stays
// latched LOW for the entire ESP32 deep-sleep interval.
#define SX126X_POWER_EN_RTC_HOLD

// This target is specifically for the Elecrow LR1262 / Semtech SX1262.
#define USE_SX1262

#define SX126X_CS LORA_CS
#define SX126X_DIO1 LORA_DIO1
#define SX126X_BUSY LORA_DIO2
#define SX126X_RESET LORA_RESET

// LR1262 physical DIO2 is wired locally to LR1262 TX/RX_EN. RadioLib then
// makes DIO2 HIGH for TX and LOW for RX. No ESP32 TXEN/RXEN GPIO is required.
#define SX126X_DIO2_AS_RF_SWITCH
#define SX126X_RXEN RADIOLIB_NC
#define SX126X_TXEN RADIOLIB_NC

// LR1262 uses an onboard TCXO supplied from SX1262 DIO3.
#define SX126X_DIO3_TCXO_VOLTAGE 1.8

// Bare SX1262 PA maximum for LR1262.
#define SX126X_MAX_POWER 22

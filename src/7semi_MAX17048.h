/*!
 * @file 7semi_MAX17048.h
 * @brief Arduino Library for MAX17048 Fuel Gauge IC
 *
 * @details
 * This library provides an interface to the MAX17048 LiPo battery fuel gauge via I2C.
 * It includes functions to read voltage, state-of-charge, charge rate, and alert flags.
 * It also supports quick start, soft reset, voltage alert configuration, and ALRT pin handling.
 *
 * @author  7semi
 * @license MIT License
 */

#ifndef _7SEMI_MAX17048_H_
#define _7SEMI_MAX17048_H_

#include <Arduino.h>
#include <Wire.h>

// I2C default address for MAX17048
#define MAX17048_ADDRESS 0x36

// Register Map
#define VCELL_REG    0x02  // Battery voltage
#define SOC_REG      0x04  // State of charge
#define MODE_REG     0x06  // Mode (quick start)
#define VERSION_REG  0x08  // IC version (includes chip ID)
#define HIBRT_REG    0x0A  // Hibernate threshold
#define CONFIG_REG   0x0C  // Configuration and ALRT latch
#define CRATE_REG    0x16  // Charge/discharge rate
#define VALRT_REG    0x14  // Voltage alert threshold (low=LSB, high=MSB)
#define COMMAND_REG  0xFE  // Reset command
#define STATUS_REG   0x1A  // Alert flags (VH, VL, POR, etc.)

class MAX17048_7semi {
public:
  MAX17048_7semi(uint8_t address = MAX17048_ADDRESS)
    : _address(address) {}

  // Initialize sensor
  bool begin(TwoWire *wire = &Wire);

  // Read battery voltage (in volts)
  float cellVoltage(void);

  // Read battery percentage (% SoC)
  float cellPercent(void);

  // Read charge/discharge rate (%/hr)
  float chargeRate(void);

  // Issue reset command to IC
  void reset(void);

  // Recalibrate SoC estimation
  void quickStart(void);

  // Set voltage alert limits (in volts)
  void setVoltageLimits(float minV, float maxV);

  // Read high alert voltage threshold
  float getMaxvoltage();

  // Read low alert voltage threshold
  float getMinvoltage();

  // Check if low voltage alert is triggered
  bool alertLowV();

  // Check if high voltage alert is triggered
  bool alertHighV();

  // Clear ALRT pin latch (bit 5 in CONFIG)
  void resetALRTPin();

private:
  TwoWire *_wire;     // I2C interface
  uint8_t _address;   // I2C address

  // Write 16-bit value to register
  void writeRegister(uint8_t reg, uint16_t value);

  // Read MSB of 2-byte register
  uint8_t readReg(uint8_t reg);

  // Read full 16-bit register value
  uint16_t read2Reg(uint8_t reg);

  // Write full 16-bit register value (little-endian)
  void write16(uint8_t reg, uint16_t value);
};

#endif // _7SEMI_MAX17048_H_

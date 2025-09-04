/*!
 * @file 7semi_MAX17048.cpp
 * @brief Arduino Library for MAX17048 Fuel Gauge IC
 *        Provides functions to read battery voltage, state-of-charge, and alerts.
 *
 * @details
 * This library supports the MAX17048 LiPo fuel gauge sensor over I2C.
 * It allows reading cell voltage, battery percentage, charge/discharge rate,
 * voltage alerts (min/max), and includes reset and quick-start support.
 *
 * @author  7semi
 * @license MIT License
 *
 * @note
 * Based on the official MAX17048 datasheet and Adafruit implementation.
 * ALRT pin logic and alert handling implemented per Section 9 of the datasheet.
 */

#include "7semi_MAX17048.h"

// Initialize the MAX17048 sensor
bool MAX17048_7semi::begin(TwoWire* wire) {
  _wire = wire;

  uint16_t version = read2Reg(VERSION_REG); // Check device version
  if (version == 0xFFFF || version == 0x0000) {
    return false; // Sensor not detected
  }

  return true;
}

// Read battery voltage in volts
float MAX17048_7semi::cellVoltage(void) {
  uint16_t cell_voltage = read2Reg(VCELL_REG);
  return cell_voltage * 78.125 / 1000000.0; // 78.125uV per LSB
}

// Read battery state-of-charge (percentage)
float MAX17048_7semi::cellPercent(void) {
  uint16_t soc = read2Reg(SOC_REG);
  return soc / 256.0; // 1/256 % per LSB
}

// Read charge/discharge rate in %/hr
float MAX17048_7semi::chargeRate(void) {
  int16_t rate = (int16_t)read2Reg(CRATE_REG);
  return rate * 0.208; // 0.208 %/hr per LSB
}

// Send reset command to the IC
void MAX17048_7semi::reset(void) {
  writeRegister(COMMAND_REG, 0x5400);
  delay(10); // Allow reset to complete
}

// Trigger quick start to recalibrate SoC
void MAX17048_7semi::quickStart(void) {
  writeRegister(MODE_REG, 0x4000);
  delay(2);
}

// Write 16-bit value to a register
void MAX17048_7semi::writeRegister(uint8_t reg, uint16_t value) {
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->write((value >> 8) & 0xFF);
  _wire->write(value & 0xFF);
  _wire->endTransmission();
}

// Read only MSB from a 16-bit register (used for VALRT min/max)
uint8_t MAX17048_7semi::readReg(uint8_t reg) {
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->endTransmission(false);

  _wire->requestFrom(_address, (uint8_t)2);
  uint8_t value = _wire->read(); // MSB
  _wire->read();                 // Discard LSB
  return value;
}

// Read 16-bit value from register
uint16_t MAX17048_7semi::read2Reg(uint8_t reg) {
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->endTransmission(false);

  _wire->requestFrom(_address, (uint8_t)2);
  uint16_t value = (_wire->read() << 8) | _wire->read();
  return value;
}

// Set voltage alert limits in volts
void MAX17048_7semi::setVoltageLimits(float minV, float maxV) {
  uint8_t minReg = (uint8_t)(minV / 0.02);  // LSB = 20mV
  uint8_t maxReg = (uint8_t)(maxV / 0.02);

  uint16_t valrt = (maxReg << 8) | minReg;
  write16(VALRT_REG, valrt);
}

// Get high voltage alert threshold
float MAX17048_7semi::getMaxvoltage() {
  return readReg(VALRT_REG + 1) * 0.02;
}

// Get low voltage alert threshold
float MAX17048_7semi::getMinvoltage() {
  return readReg(VALRT_REG) * 0.02;
}

// Write 16-bit value using little-endian format
void MAX17048_7semi::write16(uint8_t reg, uint16_t value) {
  _wire->beginTransmission(MAX17048_ADDRESS);
  _wire->write(reg);
  _wire->write(value & 0xFF);   // LSB
  _wire->write(value >> 8);     // MSB
  _wire->endTransmission();
}

// Check if high voltage alert is triggered
bool MAX17048_7semi::alertHighV() {
  uint8_t status = readReg(STATUS_REG);
  return status & 0x02; // Bit 2 = VH
}

// Check if low voltage alert is triggered
bool MAX17048_7semi::alertLowV() {
  uint8_t status = readReg(STATUS_REG);
  return status & 0x04; // Bit 3 = VL
}

// Clear ALRT latch in CONFIG register (bit 5)
void MAX17048_7semi::resetALRTPin() {
  uint16_t config = read2Reg(CONFIG_REG);
  config &= ~(1 << 5); // Clear ALRT bit
  write16(CONFIG_REG, config);
}

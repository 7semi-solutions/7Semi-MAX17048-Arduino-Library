/*!
 * @file Basic.ino
 * @brief Example for 7semi MAX17048 LiPo Fuel Gauge
 *
 * Connect the MAX17048 sensor via I2C.
 * SDA -> A4, SCL -> A5 on Arduino Uno (or use appropriate I2C pins).
 */

#include <Wire.h>
#include <7semi_MAX17048.h>

MAX17048_7semi battery;  // Create sensor object

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!battery.begin(&Wire)) {
    Serial.println("MAX17048 not detected. Check wiring.");
    while (1);
  }

  Serial.println(" MAX17048 initialized");

  // Optional: quick start or reset
  battery.quickStart();  // Recalibrate SoC
  // battery.reset();    // Full reset

  // Set alert voltage range (e.g., 3.2V min, 4.2V max)
  battery.setVoltageLimits(3.2, 4.2);
}

void loop() {
  float voltage = battery.cellVoltage();
  float soc     = battery.cellPercent();
  float rate    = battery.chargeRate();
  bool lowV     = battery.alertLowV();
  bool highV    = battery.alertHighV();

  Serial.print(" Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | SoC: ");
  Serial.print(soc, 1);
  Serial.print(" % | Rate: ");
  Serial.print(rate, 2);
  Serial.println(" %/hr");

  if (lowV) {
    Serial.println("Low Voltage Alert!");
  }

  if (highV) {
    Serial.println("High Voltage Alert!");
  }

  delay(1000);
}

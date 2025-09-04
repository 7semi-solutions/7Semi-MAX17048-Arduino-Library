# 7Semi-MAX17048-Arduino-Library

This Arduino library provides support for the **7Semi MAX17048 Fuel Gauge Sensor**, allowing you to easily monitor Li-ion battery voltage and state-of-charge (SOC) over I2C. It is based on Maxim Integrated’s MAX17048 ultra-low-power fuel gauge.

![Arduino](https://img.shields.io/badge/platform-arduino-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Status](https://img.shields.io/badge/status-active-brightgreen.svg)

---

## Hardware Required

- 7Semi MAX17048 Fuel Gauge Module  
- Arduino-compatible board  
- I2C connection (SDA, SCL)  

---

## Getting Started

### 1. Installation via Arduino Library Manager

1. Open the **Arduino IDE**
2. Go to:
   - `Sketch > Include Library > Manage Libraries…` (IDE 1.x)  
   - Or click 📚 (Library icon) in the sidebar (IDE 2.x)
3. Search for:
   -7Semi MAX17048
4. Click **Install**

Then include in your sketch:

#include <7semi_max17048.h>

### 2. Wiring

| MAX17048 Pin | Arduino Pin |
| ------------ | ----------- |
| SDA          | A4 (Uno)    |
| SCL          | A5 (Uno)    |
| VCC          | 3.3V        |
| GND          | GND         |

  

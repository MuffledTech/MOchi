# 🍡 Mochi-Bot: Interactive OLED Expressions

A minimalist, animated "Mochi" face for the **SH1106 1.3" I2C OLED display**. This project uses an Arduino-based logic to create a reactive character that blinks, looks around, and changes its mood based on touch input.

---

## ✨ Features

* **Dynamic Eyes:** Uses `u8g2` primitives to draw expressive eyes with pupils that look around randomly.
* **Natural Blinking:** A randomized timing system (1–6 seconds) to make the character feel "alive."
* **4 Interactive Modes:**
    1.  **Normal:** Calm, curious, and observant.
    2.  **Happy:** A playful vertical "jiggle" animation with wide eyes.
    3.  **Wink:** A friendly wink (featuring the `<` eye shape).
    4.  **Squint:** A cute `> <` expression for when Mochi is feeling shy or excited.
* **Touch Responsive:** Cycles through expressions instantly using a TTP223 (or similar) touch sensor.

---

## 🛠️ Hardware Requirements

* **Microcontroller:** Any Arduino-compatible board (Uno, Nano, ESP32).
* **Display:** 1.3" SH1106 OLED (I2C version).
* **Input:** TTP223 Touch Sensor.
* **Wiring Guide:**
    * **OLED VCC/GND** ➔ 5V/3.3V and GND.
    * **OLED SCL/SDA** ➔ A5/A4 (on Uno/Nano) or dedicated I2C pins.
    * **Touch Sensor SIG** ➔ Digital Pin **4**.

---

## 🚀 Getting Started

1.  **Install Library:** Open the Arduino Library Manager and install **U8g2** by oliver.
2.  **The Setup:** Ensure your constructor matches your specific OLED driver. This project is pre-configured for the `SH1106_128X64_NONAME_F_HW_I2C`.
3.  **Upload:** Flash the code to your board.
4.  **Interact:** Tap the touch sensor to cycle through the different moods!

---

## 📂 Project Structure

* `checkTouch()`: Handles state changes and prevents "double-triggering" of the touch sensor.
* `updateAnimations()`: The engine for the "Happy" bounce (using `sin()`) and the blink logic.
* `drawMochi()`: The rendering function that clears and redraws the face every frame for smooth motion.

---

## 🔧 Troubleshooting

* **Display is blank?** Check your I2C address. Most SH1106 screens use `0x3C`, but some require `0x3D`.
* **Face is flickering?** Ensure you are using `u8g2.clearBuffer()` and `u8g2.sendBuffer()` rather than `u8g2.clear()`.
* **Touch is too sensitive?** Add a small `delay(50);` in the `checkTouch()` function to further debounce the signal.

---

*Made with ❤️ for the Arduino Community.*

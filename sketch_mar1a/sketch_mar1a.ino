#include <U8g2lib.h>
#include <Wire.h>

// 1.3" SH1106 OLED Setup
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const int touchPin = 4;

// Animation Variables
int mode = 0; 
unsigned long lastBlinkTime = 0;
unsigned long lastLookTime = 0;
bool isBlinking = false;
int lookOffset = 0; 
int bounceY = 0; // For happy bouncing

// Timing constants
long nextBlinkInterval = 2000; // Starts at 2s
const long blinkDuration = 120; // Slightly faster blink
const long lookInterval = 4000;

bool lastTouchState = LOW;

void setup() {
  u8g2.begin();
  pinMode(touchPin, INPUT);
  randomSeed(analogRead(0)); // Seed randomness
}

void loop() {
  checkTouch();
  updateAnimations();
  
  u8g2.clearBuffer();
  drawMochi();
  u8g2.sendBuffer();
}

void checkTouch() {
  bool currentTouch = digitalRead(touchPin);
  if (currentTouch == HIGH && lastTouchState == LOW) {
    mode = (mode + 1) % 4;
    lookOffset = 0; 
  }
  lastTouchState = currentTouch;
}

void updateAnimations() {
  unsigned long now = millis();

  // --- NATURAL BLINKING ---
  if (!isBlinking && (now - lastBlinkTime > nextBlinkInterval)) {
    isBlinking = true;
    lastBlinkTime = now;
  } 
  else if (isBlinking && (now - lastBlinkTime > blinkDuration)) {
    isBlinking = false;
    lastBlinkTime = now;
    // Generate a random interval for the next blink (1 to 6 seconds)
    nextBlinkInterval = random(1000, 6000); 
  }

  // --- PLAYFUL BOUNCE (Happy Mode) ---
  if (mode == 1) {
    bounceY = (sin(now / 150.0) * 3); // Smooth up/down jitter
  } else {
    bounceY = 0;
  }

  // --- LOOKING AROUND ---
  if (mode == 0 && now - lastLookTime > lookInterval) {
    int r = random(0, 3);
    lookOffset = (r == 0) ? -6 : (r == 1) ? 6 : 0;
    lastLookTime = now;
  }
}

void drawMochi() {
  int centerX = 64;
  int eyeY = 32 + bounceY; // Apply bounce to eyes
  int leftEyeX = 40;
  int rightEyeX = 88;
  int mouthY = 48 + bounceY; // Apply bounce to mouth

  // --- DRAW EYES ---
  if (isBlinking && mode != 3) { // Don't blink if already squinting
    u8g2.drawRBox(leftEyeX - 10, eyeY, 20, 2, 1);
    u8g2.drawRBox(rightEyeX - 10, eyeY, 20, 2, 1);
  } 
  else {
    switch (mode) {
      case 0: // NORMAL
        u8g2.drawDisc(leftEyeX, eyeY, 12);
        u8g2.drawDisc(rightEyeX, eyeY, 12);
        u8g2.setDrawColor(0); 
        u8g2.drawDisc(leftEyeX + lookOffset + 2, eyeY - 3, 4);
        u8g2.drawDisc(rightEyeX + lookOffset + 2, eyeY - 3, 4);
        u8g2.setDrawColor(1);
        break;

      case 1: // HAPPY (Jiggling)
        u8g2.drawDisc(leftEyeX, eyeY, 14);
        u8g2.drawDisc(rightEyeX, eyeY, 14);
        u8g2.setDrawColor(0);
        u8g2.drawDisc(leftEyeX - 3, eyeY - 3, 5); 
        u8g2.drawDisc(rightEyeX - 3, eyeY - 3, 5);
        u8g2.setDrawColor(1);
        break;

      case 2: // WINK (Left open, Right is ">")
        u8g2.drawDisc(leftEyeX, eyeY, 12); 
        // Drawing "<" for right eye
        u8g2.drawLine(rightEyeX + 6, eyeY - 6, rightEyeX - 4, eyeY);
        u8g2.drawLine(rightEyeX + 6, eyeY + 6, rightEyeX - 4, eyeY);
        u8g2.drawLine(rightEyeX + 6, eyeY - 5, rightEyeX - 4, eyeY + 1);
        break;

      case 3: // SQUINT (> <)
        u8g2.drawLine(leftEyeX - 8, eyeY - 5, leftEyeX + 5, eyeY);
        u8g2.drawLine(leftEyeX - 8, eyeY + 5, leftEyeX + 5, eyeY);
        u8g2.drawLine(rightEyeX + 8, eyeY - 5, rightEyeX - 5, eyeY);
        u8g2.drawLine(rightEyeX + 8, eyeY + 5, rightEyeX - 5, eyeY);
        break;
    }
  }

  // --- DRAW MOUTH ---
  switch (mode) {
    case 1: // Happy Mouth (Wider)
      u8g2.drawRBox(centerX - 8, mouthY, 16, 4, 2);
      break;
    case 3: // Small "v" mouth
      u8g2.drawLine(centerX - 4, mouthY - 2, centerX, mouthY + 2);
      u8g2.drawLine(centerX + 4, mouthY - 2, centerX, mouthY + 2);
      break;
    default: // Neutral
      u8g2.drawHLine(centerX - 5, mouthY, 10);
      break;
  }
}
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    7
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    192
CRGB leds[NUM_LEDS];

// LED Base Definitions
#define BRIGHTNESS          255
#define FRAMES_PER_SECOND   25
uint8_t currentHUE = 0;

// Keep track of which LEDS should be illuminated next
int horizontalIndex;

// Keep track if moving left or right
#define LEFT 102
#define RIGHT 103
uint8_t leftOrRight;

void setup() {
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // Begin serial
  Serial.begin(57600);
  
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  // Assign LEDS to their horizontal positions
  // defineLedPositions();

  // Clear LEDS
  clearLeds();

  // Set Horizontal Index to 0 & set moving to right
  leftOrRight = RIGHT;
  horizontalIndex = 0;
}

void loop() {
  EVERY_N_MILLISECONDS(20) {
    fadeToBlackBy(leds, NUM_LEDS, 26);
  }
  
  // Load color into next horizontal set of LEDS
  horizontalSweep();

  // Update Horizontal Index
  if (leftOrRight == RIGHT) {
    // Case where direction changes
    if (horizontalIndex == 58) {
      leftOrRight = LEFT;
      currentHUE = random8();
    } else
      horizontalIndex++;
  } else {
    // Case where direction changes
    if (horizontalIndex == 0){
      leftOrRight = RIGHT;
      currentHUE = random8();
    } else
      horizontalIndex--;
  }

  // Send LED array out to actual led strip
  FastLED.show();

  // delay to keep frames modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

}
void horizontalSweep() {
  // minIndex & offset are used when led index grows inversely
  int ledIndex, minIndex, offset;
  int index = horizontalIndex;  

  // Illuminate LEDS 0-8 @ index 0
  if (index == 0) {
    for (ledIndex = 0; ledIndex <= 8; ledIndex++) {
      leds [ ledIndex ] = CHSV(currentHUE, 255, 255);
    }

  // Illuminate LEDS 9 - 17 @ indicies 1-9
  } else if (index <= 9) {
    ledIndex = index + 8;
    leds [ ledIndex ] = CHSV(currentHUE, 255, 255);

  // Illuminate LEDS 64 - 71 @ index 10
  } else if (index == 10) {
    for (ledIndex = 64; ledIndex <= 71; ledIndex++) {
      leds [ ledIndex ] = CHSV(currentHUE, 255, 255);
    }

  // Illuminate 3 strands of LEDS: [18-26],[63-55],[72-80] @ indicies 11-19
  } else if (index <= 19) {
    minIndex = 11;
    offset = 52;

    leds [ index + 7 ] = CHSV(currentHUE, 255, 255);
    leds [ index + 61 ] = CHSV(currentHUE, 255, 255);
    leds [ 2 * minIndex - index + offset ] = CHSV(currentHUE, 255, 255);

  // Illuminate LEDS 27-54 @ index 20
  } else if (index == 20) {
    for (ledIndex = 27; ledIndex <= 54; ledIndex++) {
      leds [ ledIndex ] = CHSV(currentHUE, 255, 255);
    }

  // Illuminate LEDS 81 - 89 @ indicies 21-29
  } else if (index <= 29) {
      ledIndex = index + 60;
      leds [ ledIndex ] = CHSV(currentHUE, 255, 255);

  // Illuminate 3 strands of LEDS: [90-98],[161-153],[162-170] @ indicies 30-38
  } else if (index <= 38) {
    minIndex = 30;
    offset = 131;
    leds [ index + 60 ] = CHSV(currentHUE, 255, 255);
    leds [ 2 * minIndex - index + offset ] = CHSV(currentHUE, 255, 255);
    leds [ index + 132 ] = CHSV(currentHUE, 255, 255);

  // Illuminate LEDS 99, [144-152], 171 @ index 39
  } else if (index == 39) {
    leds [ 99 ] = CHSV(currentHUE, 255, 255);
    leds [ 171] = CHSV(currentHUE, 255, 255);
    for (ledIndex = 144; ledIndex <= 152; ledIndex++) {
      leds [ ledIndex ] = CHSV(currentHUE, 255, 255);
    }

  // Illuminates 2 strands of LEDS: [100-107],[172-179] @ indicies 40-47
  } else if (index <= 47) {
    leds [ index + 60 ] = CHSV(currentHUE, 255, 255);
    leds [ index + 132 ] = CHSV(currentHUE, 255, 255);

  // Illuminates LEDS [108-125], 180 @ index 48
  } else if (index == 48) {
    leds [ 180 ] = CHSV(currentHUE, 255, 255);
    for (ledIndex = 108; ledIndex <= 125; ledIndex++) {
      leds [ ledIndex ] = CHSV(currentHUE, 255, 255);
    }

  // Illuminates 2 strands of LEDS: [126-134],[181-189] @ indicies 49-57
  } else if (index <= 57) {
    leds [ index + 77 ] = CHSV(currentHUE, 255, 255);
    leds [ index + 132 ] = CHSV(currentHUE, 255, 255);

  // Illuminates LEDS 135 - 143 @ index 58
  } else {
    for (ledIndex = 135; ledIndex <= 143; ledIndex++) {
      leds [ ledIndex ] = CHSV(currentHUE, 255, 255);
    }
  }
}

void clearLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(0, 0, 0);
  }
}

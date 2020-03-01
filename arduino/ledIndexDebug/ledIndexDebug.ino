#include <FastLED.h>

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    7
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    200
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND   120

uint8_t gHue = 0;
int ledIndexes[] = {0, 9, 27, 81, 55, 64, 72, 81, 89, 90, 108, 126, 135, 143, 144, 153, 161, 162, 189};

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  // Clear LEDS
  clearLeds();
}

void loop() {
  EVERY_N_MILLISECONDS(20) { gHue++; } // Slowly cycle through base color

  // Send leds array out to actual led strip
  FastLED.show();
  // delay to keep frames modest
  FastLED.delay(1000/FRAMES_PER_SECOND);

  // Update colors
  updateLeds();

}

void updateLeds(){
  for (int i : ledIndexes) {
    leds[i] += CHSV(gHue, 255, 192);
  }
}

void clearLeds(){
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(0, 0, 0);
  }
}

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

int ledPin = 6;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, ledPin, NEO_GRB + NEO_KHZ800);
const int pirPin = A0;
const int micPin = A1;
const int photoRezPin = A2;

int pirStat = 0; //ce citim de pe senzorul Pir
bool flagPir = false;
int micStat; //ce citim de pe microfon
bool flagMic = false;
int photoRezStat; //ce citim de pe senzorul de lumina
bool flagPhotoRez = true;


void setup() {    
 pinMode(pirPin, INPUT);
 pinMode(photoRezPin, INPUT);
 pinMode(micPin, INPUT);

 // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
 #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
 #endif
 // End of trinket special code

 strip.begin();
 strip.setBrightness(50);
 strip.show(); // Initialize all pixels to 'off'

 Serial.begin(9600);
}
void loop(){
  pirStat = analogRead(pirPin); 
  if (pirStat > 10){            // Daca detecteaza miscare
    flagPir = true;
    Serial.println("Detecteaza miscare");
    //delay(100);
  } 
  else {
    flagPir = false;
    Serial.println("Nu detecteaza miscare");
    //delay(100);
  }
  delay(100);

  micStat = analogRead(micPin);
  if(micStat > 5) // Daca detecteaza sunet
  {
    flagMic = true;
    Serial.println("Detecteaza sunet");
    //delay(100); 
  }
  else{
    flagMic = false;
    Serial.println("Nu detecteaza sunet");
    //delay(100);  
  }
  //Serial.println(micStat);
  delay(100);

  photoRezStat = analogRead(photoRezPin);
  
  if (photoRezStat > 600){ //Daca detecteaza lumina
    flagPhotoRez = true;
    Serial.println("Avem lumina");
    //Serial.println(photoRezStat);
    //delay(100);
  }
  else{
    flagPhotoRez = false;
    Serial.println("S-a luat curentu");
    //delay(100);
  }
  delay(100);

  if((flagPir == true && flagPhotoRez == false) || (flagMic == true && flagPhotoRez == false)){
    rainbowCycle(10);
    Serial.println("BANDA APRINSA");
    //delay(100);
  }
  else{
    colorWipe(strip.Color(0, 0, 0, 255), 50); // Black RGBW
    Serial.println("BANDA STINSA");
    //delay(100);
  }
  delay(100);
  
}


void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*3; j++) { // 3 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

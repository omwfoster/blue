
#include <Wire.h>
#include <HashMap.h>
#include "Countable.h"
#include <FastLED.h>
#include <NeoPixelEffects.h>
#include <SoftwareSerial.h>

#ifdef __AVR__
#include <avr/power.h>
#endif


#define NUM_LEDS 64
#define DATA_PIN 7
#define CLOCK_PIN 13
#define HASH_SIZE 26
#define rxd  62
#define txd  8

#define FRAMES_PER_SECOND   100

#define ZOOMING_BEATS_PER_MINUTE 122
CRGB grey = CHSV {0, 255, 255};



volatile unsigned long delay_ms = 25;
bool dir = REVERSE;
bool state_e5 = true;
bool state_e6 = true;

CRGB color_val;

NeoPixelEffects effects[9];
NeoPixelEffects * currentNPE;

CRGB gradhue1 = CHSV(0, 255, 255);
CRGB gradhue2 = CHSV(128, 255, 255);
uint8_t hueval = 0;

SoftwareSerial SerialBT(rxd, txd);
char str_command; //values to return current command instruction
int int_value;
int N_Pattern;

//Initialise the LED array, the LED Hue (ledh) array, and the LED Brightness (ledb) array.

const int LED_PIN = 13;
const CRGB TEST_COLOR = CRGB::Black;
const int LINE_BUFFER_SIZE = 20; // max line length is one less than this
char input_line[LINE_BUFFER_SIZE];
//CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;
volatile int led_position = 0;
//volatile int delay;


HashType<char const *, int> hashRawArray[HASH_SIZE];   /// hashmap array definition to implement character tranlation to led array location
HashMap<char const *, int> strangeLed = HashMap<char const *, int>(hashRawArray, HASH_SIZE);

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy( 64 );
  }
}

void scroll_hue(int8_t &begin_cursor, int8_t &end_cursor)
{
  uint8_t hue;
  for (int8_t i = begin_cursor; i < end_cursor; i++ ) {
    leds[i] = CHSV(hue++, 255, 255);
  }
}




boolean checkledstate()
{
  for (int i = 0; i < NUM_LEDS; i++) {
    if  (leds[i] != TEST_COLOR )
    {
      return true;
    }
  }
  return false;
}


void fadealltozero() {
  while (checkledstate())
  {
    leds.fadeToBlackBy( 128 );
    FastLED.show();
  }
}





int read_line(char * buffer, int bufsize) {

  if (SerialBT.available() == 0) {
    return 0 ;
  }

  for (int index = 0; index < bufsize; index++) {
    // Wait until characters are available
    while (SerialBT.available() == 0) {
    }

    char ch = SerialBT.read(); // read next character

    if (ch == '\n') {
      buffer[index] = '\0'; // end of line reached: null terminate string
      return index; // success: return length of string (zero if string is empty)
    }
    //test
    buffer[index] = ch; // Append character to buffer
  }

  // Reached end of buffer, but have not seen the end-of-line yet.
  // Discard the rest of the line (safer than returning a partial line).

  char ch;
  do {
    // Wait until characters are available
    while (SerialBT.available() == 0) {
    }
    ch = SerialBT.read(); // read next character (and discard it)

  } while (ch != '\n');

  buffer[0] = 0; // set buffer to empty string even though it should not be used
  return -1; // error: return negative one to indicate the input was too long
}

void chop(char * char_Buf, int buf_size) {
 

  // Read each command pair
  char* arg_Val = strtok(char_Buf, "\n");


  while (arg_Val != 0)
  {
    // Split the command in two values
    char* arg_Val = strchr(arg_Val, ';');
    if (arg_Val != 0)
    {
      // Actually split the string in 2: replace ';' with 0
      *arg_Val = 0;
      ++arg_Val;
      process_command(char_Buf, arg_Val);
    }
    // Find the next command in input string
    arg_Val = strtok(0, "\n");
  }

}


void pulse(CRGB * cursor_loc)
{
  // pulse led to highlight static led position


  for (int i = 100; i > 0 ; i--) // gradually drop the red output of the chosen led until off
  {
    cursor_loc->red = i;
    delay(5);
    FastLED.show();
  }
}






void running_light(int next_position)
{
  static uint8_t hue = 0 ;

  if (led_position < next_position) {
    for (int dot = led_position; dot < next_position; dot++) {
      leds[dot] = CHSV(hue++, 255, 255);                        // iterate forward as requiired
      leds.fadeToBlackBy(64); // incrementally fade the tail of the message
      leds[dot].red = 100;   // plain red led for creepy message
      FastLED.show();
      delay(delay_ms);
    }
  }
  else if (led_position > next_position) {
    for (int dot = led_position; dot > next_position; dot--) {
      leds[dot] = CHSV(hue++, 255, 255);                          // iterate backwards as required
      leds.fadeToBlackBy(64);
      leds[dot].red = 100;   // plain red led for creepy message
      FastLED.show();
      delay(delay_ms);
    }



  }
  fadealltozero(); // clear the leds prior to flashing output charater led
  pulse(&leds[next_position]);       // pulse on hashmapped led
  led_position = next_position;  // exit function setting global variable led_position
  return;
}




void strangerlite(char * buffer, int buf_size ) {


  // iterate through the imput buffer until 0a is reached
  for (int i = 0; i < buf_size; i++) {
    char c  = buffer[i];
    if (c  != '\0') {
      running_light((int)strangeLed.getValueOf(c));
    }
    else
    {
      return;
    }
  }

  return;
}




void process_command(char * str_arg, char *  str_val) {




  if (strcmp(str_arg, "message") == 0) {
    strangerlite(str_arg, 31);
  } else if (strcmp(str_arg, "pattern") == 0) {
    N_Pattern = atoi(str_val);
  }
  else if (strcmp(str_arg, "delay") == 0) {
    delay_ms = atoi(str_val);

  }
  else {
    //fadeall();
  }
}

void NPloop(int i) {
  if (i != 7) {
    effects[i].update();
  }
  if (effects[5].getEffect() == NONE) {
    if (state_e5) {
      effects[5].setEffect(FADE);
    } else {
      effects[5].setEffect(FILLIN);
    }
    state_e5 = !state_e5;
  }

  if (effects[6].getEffect() == NONE) {
    if (state_e6) {
      effects[6].setColor(CHSV(64, 255, 255));
      effects[6].setEffect(FILLIN);
    } else {
      effects[6].setColor(CHSV(192, 255, 255));
      effects[6].setEffect(FILLIN);
    }
    state_e6 = !state_e6;
  }

  FastLED.show();
}


void setup() {
  pinMode(LED_PIN, OUTPUT);
  SerialBT.begin(38400);    // default serial setup for debugging
  Serial.begin(9600);
  // SerialBT.begin(38400);
  N_Pattern = 1;

  LEDS.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS); // create led class to hold values <led type,output_pin,orderby which colours are processed within the 3 byte CRGB struct
  LEDS.setBrightness(84);
  // mapped values for character led mappings
  strangeLed[0]('a', 1);
  strangeLed[1]('b', 3);
  strangeLed[2]('c', 6);
  strangeLed[3]('d', 8);
  strangeLed[4]('e', 10);
  strangeLed[5]('f', 12);
  strangeLed[6]('g', 14);
  strangeLed[7]('h', 16);
  strangeLed[8]('i', 18);
  strangeLed[9]('j', 20);
  strangeLed[10]('k', 22);
  strangeLed[11]('l', 24);
  strangeLed[12]('m', 26);
  strangeLed[13]('n', 28);
  strangeLed[14]('o', 30);
  strangeLed[15]('p', 32);
  strangeLed[16]('q', 34);
  strangeLed[17]('r', 36);
  strangeLed[18]('s', 38);
  strangeLed[19]('t', 40);
  strangeLed[20]('u', 42);
  strangeLed[21]('v', 44);
  strangeLed[22]('w', 46);
  strangeLed[23]('x', 48);
  strangeLed[24]('y', 50);
  strangeLed[25]('z', 52);

  int rangeStart = 0;         // # pixel (> 0 and < NUMPIXELS - 2)
  int rangeEnd = NUM_LEDS - 1;

  int hue = 0;
  color_val.setHue(hue);
  effects[0] = NeoPixelEffects(leds, SINEWAVE, rangeStart, rangeEnd, 1, 5, color_val, true, dir);
  hue += 32;
  color_val.setHue(hue);
  effects[1] = NeoPixelEffects(leds, COMET, rangeStart, rangeEnd, 8, delay_ms, color_val, true, dir);
  hue += 32;
  color_val.setHue(hue);
  effects[2] = NeoPixelEffects(leds, LARSON, rangeStart, rangeEnd, 1, delay_ms, color_val, true, dir);
  hue += 32;
  color_val.setHue(hue);
  effects[3] = NeoPixelEffects(leds, PULSE, rangeStart, rangeEnd, 1, delay_ms, color_val, true, dir);
  hue += 32;
  color_val.setHue(hue);
  effects[4] = NeoPixelEffects(leds, STATIC, rangeStart, rangeEnd, 1, delay_ms, color_val, true, dir);
  hue += 32;
  color_val.setHue(hue);
  effects[5] = NeoPixelEffects(leds, FILLIN, rangeStart, rangeEnd, 1, delay_ms, color_val, true, dir);
  hue += 32;
  color_val.setHue(hue);
  effects[6] = NeoPixelEffects(leds, FILLIN, rangeStart, rangeEnd, 1, delay_ms, color_val, true, dir);
  hue += 32;
  color_val.setHue(hue);
  effects[7] = NeoPixelEffects(leds, NONE, rangeStart, rangeEnd, 1, delay_ms, color_val, true, dir);
  effects[7].fill_gradient(color_val, CHSV(96, 255, 255));
  hue += 32;
  color_val.setHue(hue);
  effects[8] = NeoPixelEffects(leds, RAINBOWWAVE, rangeStart, rangeEnd, 1, delay_ms, color_val, true, dir);

}



void loop() {



  NPloop(N_Pattern);


  clear_input_buffer(input_line, 20); // guarantee no bad data carried over from previous run
  static int read_state = 0;
  read_state = read_line(input_line, sizeof(input_line));

  if (  read_state < 0) {
    return; // skip command processing and try again on next iteration of loop
  }
  else if (read_state == 0) {
    return;
  }
  else
  {
    chop(input_line, sizeof(input_line));
  }


  // draw the light pattern into the 'leds' array


}


void  clear_input_buffer(char * local_buffer, int buf_len) {

  for (int index = 0; index < buf_len; index++) {
    local_buffer[index] = '\0'; //write 20 string terminators to array.
    //Guarantees that any string within bounds will be null terminated
  }
}

int print_buffer(char * local_buffer, int buf_len) {
  for (int index = 0; index < buf_len; index++) {
    //   Serial.print(local_buffer[index]);   //output buffer contents for debug
  }

}




void cylon() {
  static uint8_t hue = 0;
  //        Serial.print("x");
  // First slide the led in one direction
  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black

    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(delay_ms);
  }
  //        Serial.print("x");

  // Now go in the other direction.
  for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(delay_ms);
  }
}






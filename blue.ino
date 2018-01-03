
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



unsigned long delay_ms;
bool dir = REVERSE;
bool state_e5 = true;
bool state_e6 = true;

CRGB color_val;

NeoPixelEffects effects[9];
NeoPixelEffects * currentNPE;

CRGB gradhue1 = CHSV(0, 255, 255);
CRGB gradhue2 = CHSV(128, 255, 255);
uint8_t hueval = 0;

//SoftwareSerial Serial1(rxd, txd);
int int_value;
int N_Pattern;

//Initialise the LED array, the LED Hue (ledh) array, and the LED Brightness (ledb) array.

const int LED_PIN = 13;
const CRGB TEST_COLOR = CRGB::Black;
const int LINE_BUFFER_SIZE = 20; // max line length is one less than this
char input_line[LINE_BUFFER_SIZE + 1];
//CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;
int led_position;
//volatile int delay;


HashType<char const *, int> hashRawArray[HASH_SIZE];   /// hashmap array definition to implement character tranlation to led array location
HashMap<char const *, int> strangeLed = HashMap<char const *, int>(hashRawArray, HASH_SIZE);






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





int read_line(char * buffer, int  buf_size) {

  

  if (Serial1.available() == 0) {
    return 0 ;
  }
  
  
  for (int index = 0; index <  buf_size; index++) {
    // Wait until characters are available
    while (Serial1.available() == 0) {
    }

    char ch = Serial1.read(); // read next character

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
  //do {
    // Wait until characters are available
    while (Serial1.available() == 0) {
    }
    ch = Serial1.read(); // read next character (and discard it)

  //} while (ch != '\n');

  clear_input_buffer();
  return -1; // error: return negative one to indicate the input was too long
}

// separate arg/val pairs
//axecute argument through (process_command)

int chop(char * local_buffer, int buf_size) {

  char * Buffer_val = 0;

  //Buffer_arg = local_buffer;
   // Split the command in two values

  
  if (local_buffer != 0)
  {
    char* Buffer_val = strchr(local_buffer, ';');
    if (Buffer_val != 0)
    {
      // Actually split the string in 2: replace ';' with 0
      *Buffer_val = 0;
      ++Buffer_val;

      process_command(local_buffer, Buffer_val);     
    }
    // Find the next command in input string
  
  return 1;

  }
  return 0 ;
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




void strangerlite(char * local_buffer, int buf_size ) {
  // iterate through the imput buffer until 0a is reached
  for (int i = 0; i < buf_size; i++) {
    char c  = local_buffer[i];
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



// pattern match and executes argument\value pair


void process_command(char * str_arg, char *  str_val) {


  if (strcmp(str_arg, "message") == 0) {
    strangerlite(str_val, LINE_BUFFER_SIZE);
  

  // nb using global variable. state located in N_patter,
  // and currentNPE. beware. move towards pointer
  
  } else if (strcmp(str_arg, "pattern") == 0) {
   // trim_trailing_char(str_val);
    N_Pattern = atoi(str_val);
    currentNPE = &effects[N_Pattern];
  
  
  }
  else if (strcmp(str_arg, "delay") == 0) {
   //  trim_trailing_char(str_val);
    if(currentNPE)
      {
        currentNPE->setDelay(atoi(str_val));
      }
  }


  else if (strcmp(str_arg,"colour") ==0 ){
    set_colour(str_val,&color_val);

        if(currentNPE)
      {
        currentNPE->setColor(color_val);
      }
  }



  else {
  }
  print_buffer();
  clear_input_buffer();
  


  
  
}

int set_colour(char * char_RGB, CRGB * CRGB_reference )
{
  //RGB packet should be 11 character long. 3 x 3 bytes plus 2 x comma delimeters
  if(strlen(char_RGB)==11)
    {
      int i = 0;
      char * token = strtok(',',char_RGB);
        do
        {   
            CRGB_reference[i] = atoi(token);
            token = strtok(0,token) ;
            i++;
        }while(token);
        return 1;
    }
  return 0;
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
  Serial1.begin(9600);    // default serial setup for debugging
  Serial.begin(9600);
  clear_input_buffer();
  // Serial1.begin(38400);
  N_Pattern = 1;
  delay_ms = 25;
  led_position = 0;

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
  int read_state = 0;
  read_state = read_line(input_line, LINE_BUFFER_SIZE);
  if (  read_state < 0) {
    read_state = 0;
    return; // skip command processing and try again on next iteration of loop
  }
  else if (read_state == 0) {

    return;
  }
  else
  {
    
    chop(input_line, read_state);
    read_state = 0;

    return;
  }
}



// set all contents of allocated buffer space to null. Size is one larger than buffer size to guarantee null termination.

void  clear_input_buffer() {



  for(int  i = 0 ; i < LINE_BUFFER_SIZE; i++)
  {
    input_line[i] = 0;
  }
  // memset(input_line, 0, LINE_BUFFER_SIZE+1);
}

void trim_trailing_char(char * arg_val)
{
   char * p = strchr (arg_val, '\r'); // search for space
      if (p)     // if found truncate at space
        { *p = 0;}
}


//print contents of defined buffer. Assumes char[] contents . second argument is irrelevant as assumes null, and appends \n

int print_buffer() {

  
  for (int8_t i = 0; i < LINE_BUFFER_SIZE; i++ ) {
   Serial.write((char)input_line[i]);
  }
   //Serial.print('\r');

}

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
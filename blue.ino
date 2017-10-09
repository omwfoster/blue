
#include <Wire.h>
#include <HashMap.h>
#include "Countable.h"
#include <FastLED.h>
#define NUM_LEDS 64 
#define DATA_PIN 7
#define CLOCK_PIN 13
#define HASH_SIZE 26

#define FRAMES_PER_SECOND   100

#define ZOOMING_BEATS_PER_MINUTE 122


//Initialise the LED array, the LED Hue (ledh) array, and the LED Brightness (ledb) array.

const int LED_PIN = 13; 
const CRGB TEST_COLOR = CRGB::Black;
const int LINE_BUFFER_SIZE = 20; // max line length is one less than this
//CRGB leds[NUM_LEDS]; 
CRGBArray<NUM_LEDS> leds;
volatile int led_position = 0;

HashType<char const *, int> hashRawArray[HASH_SIZE];   /// hashmap array definition to implement character tranlation to led array location
HashMap<char const *, int> strangeLed = HashMap<char const *, int>(hashRawArray, HASH_SIZE);

void fadeall() { 
    for(int i = 0; i < NUM_LEDS; i++) { 
        leds[i].fadeToBlackBy( 64 ); } 
    }

void scroll_hue(int8_t &begin_cursor, int8_t &end_cursor)
{      
    uint8_t hue;
    for(int8_t i = begin_cursor; i < end_cursor; i++ ){
        leds[i] = CHSV(hue++, 255, 255);    
    } 
}
    



boolean checkledstate()
{
    for(int i = 0; i < NUM_LEDS;i++){         
        if  (leds[i] != TEST_COLOR )
        {
            //Serial.print("checkledstate");
            return true;   
        }   
    }
    return false;
}


void fadealltozero(){
    while(checkledstate())
    {
        leds.fadeToBlackBy( 128 );
        FastLED.show(); 
    }
}





int read_line(char * buffer, int bufsize) {

    if(Serial.available() == 0){
    //Serial.print("nothing to read");  
    return 0 ;}

    clear_input_buffer(buffer, 20); // guarantee no bad data carried over from previous run
    for (int index = 0; index < bufsize; index++) {
        // Wait until characters are available
            while (Serial.available() == 0) {
            }

            char ch = Serial.read(); // read next character
            // Serial.print(ch); // echo it back: useful with the serial monitor (optional)

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
        while (Serial.available() == 0) {
        }
        ch = Serial.read(); // read next character (and discard it)
    // Serial.print(ch); // echo it back
    }while (ch != '\n'); 

    buffer[0] = 0; // set buffer to empty string even though it should not be used
    return-1; // error: return negative one to indicate the input was too long
}


void setup() {
    pinMode(LED_PIN, OUTPUT); 
    Serial.begin(9600);    // default serial setup for debugging
    Serial.println("Welcome");
    LEDS.addLeds<WS2812,DATA_PIN,GRB>(leds,NUM_LEDS); // create led class to hold values <led type,output_pin,orderby which colours are processed within the 3 byte CRGB struct
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
}

void loop() {
    Serial.print("> "); 
    // Read command

    char line[LINE_BUFFER_SIZE]; 
    //Serial.printNumber(Serial.available());
    int read_state = read_line(line, sizeof(line));
    
    if (  read_state < 0) {
        Serial.println("Error: line too long"); 
    return; // skip command processing and try again on next iteration of loop
    }
    else if (read_state == 0){
        //discostrobe();
        cylon();
        FastLED.show();
        return;
    }

    // Process command


    if (strcmp(line, "off") == 0) {
        digitalWrite(LED_PIN, LOW); 
    }else if (strcmp(line, "on") == 0) {
        
        digitalWrite(LED_PIN, HIGH); 
    }else if (strcmp(line, "") == 0) {
    // Empty line: no command
    }else {
        //fadeall();
        strangerlite(line,20);      
        Serial.println("stranger output");
    }
     // draw the light pattern into the 'leds' array

}


int  clear_input_buffer(char * local_buffer, int buf_len) {

    for (int index = 0; index < buf_len; index++) {
    local_buffer[index] = '\0'; //write 20 string terminators to array. 
                            //Guarantees that any string within bounds will be null terminated
    }
}

int print_buffer(char * local_buffer, int buf_len) {
    for (int index = 0; index < buf_len; index++) {
    Serial.print(local_buffer[index]);   //output buffer contents for debug
    }
    
 }


    

void cylon() {  
        static uint8_t hue = 0; 
//        Serial.print("x"); 
        // First slide the led in one direction 
        for(int i = 0; i < NUM_LEDS; i++) { 
            // Set the i'th led to red  
            leds[i] = CHSV(hue++, 255, 255); 
            // Show the leds 
            FastLED.show();  
            // now that we've shown the leds, reset the i'th led to black 
            
            fadeall(); 
            // Wait a little bit before we loop around and do it again 
            //delay(30); 
        } 
//        Serial.print("x"); 
     
        // Now go in the other direction.   
        for(int i = (NUM_LEDS)-1; i >= 0; i--) { 
            // Set the i'th led to red  
            leds[i] = CHSV(hue++, 255, 255); 
            // Show the leds 
            FastLED.show(); 
            fadeall(); 
            // Wait a little bit before we loop around and do it again 
            //delay(30); 
        } 
} 


void strangerlite(char * buffer, int buf_size ) {  
 

    // iterate through the imput buffer until 0a is reached
    for(int i = 0; i < buf_size; i++) { 
    char c  = buffer[i];   
        if (c  != '\0'){   
        //     Serial.print((int)strangeLed.getValueOf(c));
         running_light((int)strangeLed.getValueOf(c));  
         }
    } 
 
    
} 



void running_light(int next_position)
{
    static uint8_t hue = 0 ;

    if(led_position < next_position) {
        for(int dot = led_position; dot < next_position; dot++) {   
            leds[dot] = CHSV(hue++, 255, 255);                        // iterate forward as requiired
            leds.fadeToBlackBy(64); // incrementally fade the tail of the message
            leds[dot].red = 100;   // plain red led for creepy message
            FastLED.show(); 
            delay(20);
        }
    }
    else if(led_position > next_position){
        for(int dot = led_position; dot > next_position; dot--) {   
            leds[dot] = CHSV(hue++, 255, 255);                          // iterate backwards as required            
            leds.fadeToBlackBy(64); 
            leds[dot].red = 100;   // plain red led for creepy message
            FastLED.show(); 
            delay(20);
        }

      
     
    }
    fadealltozero(); // clear the leds prior to flashing output charater led
    pulse(&leds[next_position]);       // pulse on hashmapped led
    led_position = next_position;  // exit function setting global variable led_position

}


void pulse(CRGB * cursor_loc)
{   
    // pulse led to highlight static led position


    for(int i =100;i > 0 ; i--)// gradually drop the red output of the chosen led until off
    {
    cursor_loc->red = i;
    FastLED.show();  
    }
}


void discostrobe()
{
  // First, we black out all the LEDs
  fill_solid( leds, NUM_LEDS, CRGB::Black);

  // To achive the strobe effect, we actually only draw lit pixels
  // every Nth frame (e.g. every 4th frame).  
  // sStrobePhase is a counter that runs from zero to kStrobeCycleLength-1,
  // and then resets to zero.  
  const uint8_t kStrobeCycleLength = 4; // light every Nth frame
  static uint8_t sStrobePhase = 0;
  sStrobePhase = sStrobePhase + 1;
  if( sStrobePhase >= kStrobeCycleLength ) { 
    sStrobePhase = 0; 
  }

  // We only draw lit pixels when we're in strobe phase zero; 
  // in all the other phases we leave the LEDs all black.
  if( sStrobePhase == 0 ) {

    // The dash spacing cycles from 4 to 9 and back, 8x/min (about every 7.5 sec)
    uint8_t dashperiod= beatsin8( 8/*cycles per minute*/, 4,10);
    // The width of the dashes is a fraction of the dashperiod, with a minimum of one pixel
    uint8_t dashwidth = (dashperiod / 4) + 1;
    
    // The distance that the dashes move each cycles varies 
    // between 1 pixel/cycle and half-the-dashperiod/cycle.
    // At the maximum speed, it's impossible to visually distinguish
    // whether the dashes are moving left or right, and the code takes
    // advantage of that moment to reverse the direction of the dashes.
    // So it looks like they're speeding up faster and faster to the
    // right, and then they start slowing down, but as they do it becomes
    // visible that they're no longer moving right; they've been 
    // moving left.  Easier to see than t o explain.
    //
    // The dashes zoom back and forth at a speed that 'goes well' with
    // most dance music, a little faster than 120 Beats Per Minute.  You
    // can adjust this for faster or slower 'zooming' back and forth.
    uint8_t zoomBPM = ZOOMING_BEATS_PER_MINUTE;
    int8_t  dashmotionspeed = beatsin8( (zoomBPM /2), 1,dashperiod);
    // This is where we reverse the direction under cover of high speed
    // visual aliasing.
    if( dashmotionspeed >= (dashperiod/2)) { 
      dashmotionspeed = 0 - (dashperiod - dashmotionspeed );
    }

    
    // The hueShift controls how much the hue of each dash varies from 
    // the adjacent dash.  If hueShift is zero, all the dashes are the 
    // same color. If hueShift is 128, alterating dashes will be two
    // different colors.  And if hueShift is range of 10..40, the
    // dashes will make rainbows.
    // Initially, I just had hueShift cycle from 0..130 using beatsin8.
    // It looked great with very low values, and with high values, but
    // a bit 'busy' in the middle, which I didnt like.
    //   uint8_t hueShift = beatsin8(2,0,130);
    //
    // So instead I layered in a bunch of 'cubic easings'
    // (see http://easings.net/#easeInOutCubic )
    // so that the resultant wave cycle spends a great deal of time
    // "at the bottom" (solid color dashes), and at the top ("two
    // color stripes"), and makes quick transitions between them.
    uint8_t cycle = beat8(2); // two cycles per minute
    uint8_t easedcycle = ease8InOutCubic( ease8InOutCubic( cycle));
    uint8_t wavecycle = cubicwave8( easedcycle);
    uint8_t hueShift = scale8( wavecycle,130);


    // Each frame of the animation can be repeated multiple times.
    // This slows down the apparent motion, and gives a more static
    // strobe effect.  After experimentation, I set the default to 1.
    uint8_t strobesPerPosition = 1; // try 1..4


    // Now that all the parameters for this frame are calculated,
    // we call the 'worker' function that does the next part of the work.
    discoWorker( dashperiod, dashwidth, dashmotionspeed, strobesPerPosition, hueShift);
  }  
}


// discoWorker updates the positions of the dashes, and calls the draw function
//
void discoWorker( 
    uint8_t dashperiod, uint8_t dashwidth, int8_t  dashmotionspeed,
    uint8_t stroberepeats,
    uint8_t huedelta)
 {
  static uint8_t sRepeatCounter = 0;
  static int8_t sStartPosition = 0;
  static uint8_t sStartHue = 0;

  // Always keep the hue shifting a little
  sStartHue += 1;

  // Increment the strobe repeat counter, and
  // move the dash starting position when needed.
  sRepeatCounter = sRepeatCounter + 1;
  if( sRepeatCounter>= stroberepeats) {
    sRepeatCounter = 0;
    
    sStartPosition = sStartPosition + dashmotionspeed;
    
    // These adjustments take care of making sure that the
    // starting hue is adjusted to keep the apparent color of 
    // each dash the same, even when the state position wraps around.
    if( sStartPosition >= dashperiod ) {
      while( sStartPosition >= dashperiod) { sStartPosition -= dashperiod; }
      sStartHue  -= huedelta;
    } else if( sStartPosition < 0) {
      while( sStartPosition < 0) { sStartPosition += dashperiod; }
      sStartHue  += huedelta;
    }
  }

  // draw dashes with full brightness (value), and somewhat
  // desaturated (whitened) so that the LEDs actually throw more light.
  const uint8_t kSaturation = 208;
  const uint8_t kValue = 255;

  // call the function that actually just draws the dashes now
  drawRainbowDashes( sStartPosition, NUM_LEDS-1, 
                     dashperiod, dashwidth, 
                     sStartHue, huedelta, 
                     kSaturation, kValue);
}


// drawRainbowDashes - draw rainbow-colored 'dashes' of light along the led strip:
//   starting from 'startpos', up to and including 'lastpos'
//   with a given 'period' and 'width'
//   starting from a given hue, which changes for each successive dash by a 'huedelta'
//   at a given saturation and value.
//
//   period = 5, width = 2 would be  _ _ _ X X _ _ _ Y Y _ _ _ Z Z _ _ _ A A _ _ _ 
//                                   \-------/       \-/
//                                   period 5      width 2
//
static void drawRainbowDashes( 
  uint8_t startpos, uint16_t lastpos, uint8_t period, uint8_t width, 
  uint8_t huestart, uint8_t huedelta, uint8_t saturation, uint8_t value)
{
  uint8_t hue = huestart;
  for( uint16_t i = startpos; i <= lastpos; i += period) {
    CRGB color = CHSV( hue, saturation, value);
    
    // draw one dash
    uint16_t pos = i;
    for( uint8_t w = 0; w < width; w++) {
      leds[ pos ] = color;
      pos++;
      if( pos >= NUM_LEDS) {
        break;
      }
    }
    
    hue += huedelta;
  }
}


// delayToSyncFrameRate - delay how many milliseconds are needed
//   to maintain a stable frame rate.
static void delayToSyncFrameRate( uint8_t framesPerSecond)
{
  static uint32_t msprev = 0;
  uint32_t mscur = millis();
  uint16_t msdelta = mscur - msprev;
  uint16_t mstargetdelta = 1000 / framesPerSecond;
  if( msdelta < mstargetdelta) {
    delay( mstargetdelta - msdelta);
  }
  msprev = mscur;
}






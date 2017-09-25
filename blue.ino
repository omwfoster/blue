
#include <Wire.h>
#include <HashMap.h>
#include "Countable.h"
#include <FastLED.h>
#define NUM_LEDS 64 
#define DATA_PIN 7
#define CLOCK_PIN 13
#define HASH_SIZE 26

//Initialise the LED array, the LED Hue (ledh) array, and the LED Brightness (ledb) array.
CRGB leds[NUM_LEDS];
byte ledh[NUM_LEDS];
byte ledb[NUM_LEDS];

//Pin connections
const int potPin = A0;      // The potentiometer signal pin is connected to Arduino's Analog Pin 0
const int yPin = A4;        // Y pin on accelerometer is connected to Arduino's Analog Pin 4
                            // The accelerometer's X Pin and the Z Pin were not used in this sketch

//Global Variables ---------------------------------------------------------------------------------
byte potVal;                // potVal:      stores the potentiometer signal value
byte prevPotVal=0;          // prevPotVal:  stores the previous potentiometer value
int LEDSpeed=1;             // LEDSpeed:    stores the "speed" of the LED animation sequence
int maxLEDSpeed = 50;       // maxLEDSpeed: identifies the maximum speed of the LED animation sequence
int LEDAccel=0;             // LEDAccel:    stores the acceleration value of the LED animation sequence (to speed it up or slow it down)
int LEDPosition=72;         // LEDPosition: identifies the LED within the strip to modify (leading LED). The number will be between 0-143.  (Zero to NUM_LEDS-1)
int oldPos=0;               // oldPos:      holds the previous position of the leading LED
byte hue = 0;               // hue:         stores the leading LED's hue value
byte intensity = 150;       // intensity:   the default brightness of the leading LED
byte bright = 80;           // bright:      this variable is used to modify the brightness of the trailing LEDs
int animationDelay = 0;     // animationDelay: is used in the animation Speed calculation. The greater the animationDelay, the slower the LED sequence.
int effect = 0;             // effect:      is used to differentiate and select one out of the four effects
int sparkTest = 0;          // sparkTest:   variable used in the "sparkle" LED animation sequence 
boolean constSpeed = false; // constSpeed:  toggle between constant and variable speed.




const int LED_PIN = 13; 
const int LINE_BUFFER_SIZE = 20; // max line length is one less than this
CRGB leds[NUM_LEDS]; 



HashType<char const *, int> hashRawArray[HASH_SIZE];
HashMap<char const *, int> strangeLed = HashMap<char const *, int>(hashRawArray, HASH_SIZE);


void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } } 

int read_line(char * buffer, int bufsize) {
    clear_input_buffer(buffer, 20);
    for (int index = 0; index < bufsize; index++) {
        // Wait until characters are available
            while (Serial.available() == 0) {
 //               cylon();//test leds are functioning when idle
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
    Serial.begin(9600); 
    Serial.println("Welcome");
    LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS); 
    LEDS.setBrightness(84); 

    strangeLed[0]('a', 1);
    strangeLed[1]('b', 2);
    strangeLed[2]('c', 3);
    strangeLed[3]('d', 4);
    strangeLed[4]('e', 5);
    strangeLed[5]('f', 6);
    strangeLed[6]('g', 7);
    strangeLed[7]('h', 8);
    strangeLed[8]('i', 9);
    strangeLed[9]('j', 10);
    strangeLed[10]('k', 11);
    strangeLed[11]('l', 12);
    strangeLed[12]('m', 13);
    strangeLed[13]('n', 14);
    strangeLed[14]('o', 15);
    strangeLed[15]('p', 16);
    strangeLed[16]('q', 17);
    strangeLed[17]('r', 18);
    strangeLed[18]('s', 19);
    strangeLed[19]('t', 20);  
    strangeLed[20]('u', 21);
    strangeLed[21]('v', 22);
    strangeLed[22]('w', 23);
    strangeLed[23]('x', 24);
    strangeLed[24]('y', 25);
    strangeLed[25]('z', 26);
}

void loop() {
    Serial.print("> "); 

    // Read command

    char line[LINE_BUFFER_SIZE]; 
    //Serial.printNumber(Serial.available());
    clear_input_buffer[line,LINE_BUFFER_SIZE];
    if (  read_line(line, sizeof(line)) < 0) {
      //  Serial.println("Error: line too long"); 
    return; // skip command processing and try again on next iteration of loop
    }

    // Process command

    if (strcmp(line, "off") == 0) {
        digitalWrite(LED_PIN, LOW); 
    }else if (strcmp(line, "on") == 0) {
        digitalWrite(LED_PIN, HIGH); 
    }else if (strcmp(line, "") == 0) {
    // Empty line: no command
    }else {
        strangerlite(line,20);
      //  Serial.print("Error: unknown command: \"");
       // Serial.print(line);
      //  Serial.println("\" (available commands: \"off\", \"on\")"); 
    }
}

int  clear_input_buffer(char * local_buffer, int buf_len) {
for (int index = 0; index < buf_len; index++) {
local_buffer[index] = '\0'; 
}
}

int print_buffer(char * local_buffer, int buf_len) {
    for (int index = 0; index < buf_len; index++) {
    Serial.print(local_buffer[index]);
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
            // leds[i] = CRGB::Black; 
            fadeall(); 
            // Wait a little bit before we loop around and do it again 
            delay(10); 
        } 
//        Serial.print("x"); 
     
        // Now go in the other direction.   
        for(int i = (NUM_LEDS)-1; i >= 0; i--) { 
            // Set the i'th led to red  
            leds[i] = CHSV(hue++, 255, 255); 
            // Show the leds 
            FastLED.show(); 
            // now that we've shown the leds, reset the i'th led to black 
            // leds[i] = CRGB::Black; 
            fadeall(); 
            // Wait a little bit before we loop around and do it again 
            delay(10); 
        } 
} 


void strangerlite(char * buffer, int buf_size ) {  
    static uint8_t hue = 0; 
    fadeall();

    // First slide the led in one direction 
    for(int i = 0; i < buf_size; i++) { 
    char c  = buffer[i];   
    if (c  != '\0'){   
         Serial.print((int)strangeLed.getValueOf(c));
         Serial.print('\n');
    }
    

    

    } 
 
  //  strangeLed.debug(); //output hashmap contents
    
} 



void running_light(int start,int finish)
{
    for(int dot = start; dot < finish; dot++) { 
        adjustSpeed();
        constrainLEDs();
    }


}



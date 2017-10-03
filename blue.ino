<<<<<<< HEAD

#include <Wire.h>
#include <HashMap.h>
#include "Countable.h"
#include <FastLED.h>
#define NUM_LEDS 64 
#define DATA_PIN 7
#define CLOCK_PIN 13
#define HASH_SIZE 26

//Initialise the LED array, the LED Hue (ledh) array, and the LED Brightness (ledb) array.

=======
#include "FastLED.h"

#define NUM_LEDS 64
#define DATA_PIN 7
#define CLOCK_PIN 13
>>>>>>> ce7d2158f326307c0b7911af733af423d8cf0cb2


const int LED_PIN = 13; 
const int LINE_BUFFER_SIZE = 20; // max line length is one less than this
CRGB leds[NUM_LEDS]; 
volatile int led_position = 0;

CRGB leds[NUM_LEDS];


<<<<<<< HEAD
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
=======
void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

int  clear_input_buffer(char * local_buffer, int buf_len) {
    for (int index = 0; index < buf_len; index++) {
    local_buffer[index] = '\0'; 
    }
    }

int read_line(char * buffer, int bufsize) {
    
    
    for (int index = 0; index < bufsize; index++) {
        Serial.print(char(index));
            // Wait until characters are available
         while (Serial.available() == 0) {
        }
 
    char ch = Serial.read(); // read next character
    Serial.print(ch); // echo it back: useful with the serial monitor (optional)

        if (ch == '\n') {
            buffer[index] = '\0'; // end of line reached: null terminate string
            Serial.print("eol");
            return index; // success: return length of string (zero if string is empty)
        }
>>>>>>> ce7d2158f326307c0b7911af733af423d8cf0cb2
    //test
    buffer[index] = ch; // Append character to buffer
    }

    // Reached end of buffer, but have not seen the end-of-line yet.
    // Discard the rest of the line (safer than returning a partial line).

<<<<<<< HEAD
char ch; 
do {
=======
    char ch; 
    do {
>>>>>>> ce7d2158f326307c0b7911af733af423d8cf0cb2
    // Wait until characters are available
    while (Serial.available() == 0) {
    }
    ch = Serial.read(); // read next character (and discard it)
<<<<<<< HEAD
   // Serial.print(ch); // echo it back
}while (ch != '\n'); 
=======
    Serial.print(ch); // echo it back
    }while (ch != '\n'); 
>>>>>>> ce7d2158f326307c0b7911af733af423d8cf0cb2

    buffer[0] = 0; // set buffer to empty string even though it should not be used
    return-1; // error: return negative one to indicate the input was too long
    }


void setup() {
<<<<<<< HEAD
    pinMode(LED_PIN, OUTPUT); 
    Serial.begin(9600); 
    Serial.println("Welcome");
    LEDS.addLeds<WS2812,DATA_PIN,GRB>(leds,NUM_LEDS); 
    LEDS.setBrightness(84); 

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
    clear_input_buffer[line,LINE_BUFFER_SIZE];
    if (  read_line(line, sizeof(line)) < 0) {
        Serial.println("Error: line too long"); 
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
        Serial.println("stranger output");
       // Serial.print(line);
      //  Serial.println("\" (available commands: \"off\", \"on\")"); 
    }

    cylon();
}
=======

    pinMode(LED_PIN, OUTPUT); 
    Serial.begin(9600); 
    Serial.println("Welcome");
    LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
    LEDS.setBrightness(20);

}

void loop() {

Serial.print("> "); 
cylon();

// Read command
char line[LINE_BUFFER_SIZE]; 
clear_input_buffer(line, 20);

    if ( read_line(line, sizeof(line) < 0)) {   // edit out (Serial.read() != 0) && condition to test for faulty conditionals
        Serial.println("Error: line too long"); 
        return; // skip command processing and try again on next iteration of loop

     }

// Process command

>>>>>>> ce7d2158f326307c0b7911af733af423d8cf0cb2

    if (strcmp(line, "off") == 0) {
        digitalWrite(LED_PIN, LOW);
        }else if (strcmp(line, "on") == 0) {
        digitalWrite(LED_PIN, HIGH); 
        }else if (strcmp(line, "") == 0) {
        
        
    // Empty line: no command
        }else {
        Serial.print("Error: unknown command: \"");
        Serial.println(line);
        Serial.println("\" (available commands: \"off\", \"on\")"); 
        }
    }







int print_buffer(char * local_buffer, int buf_len) {
    for (int index = 0; index < buf_len; index++) {
    Serial.print(local_buffer[index]);
    }
    
    }


<<<<<<< HEAD
    

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

    // iterate through the 
    for(int i = 0; i < buf_size; i++) { 
    char c  = buffer[i];   
    if (c  != '\0'){   
    //     Serial.print((int)strangeLed.getValueOf(c));
         running_light((int)strangeLed.getValueOf(c));     
    }
    

    

    } 
 
  //  strangeLed.debug(); //output hashmap contents
    
} 



void running_light(int next_position)
{


    if(led_position < next_position) {
        for(int dot = led_position; dot < next_position; dot++) {                           // iterate forward as requiired
            fadeLEDs(64);  
            leds[dot].red = 100;   // plain red led for creepy message
            FastLED.show(); 
            delay(10);

        }
    pulse(&leds[led_position]);
    }
    else if(led_position > next_position){
        for(int dot = led_position; dot > next_position; dot--) {                           // iterate backwards as required
            fadeLEDs(64);  
            leds[dot].red = 100;   //  // plain red led for creepy message
            FastLED.show(); 
            delay(10);
        }
     pulse(&leds[led_position]);
    }

    led_position = next_position;
    delay(1000);

}


void pulse(CRGB * cursor_loc)
{
    cursor_loc->fadeLightBy(50);
}





=======

    void cylon() { 
        static uint8_t hue = 0;
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
    
>>>>>>> ce7d2158f326307c0b7911af733af423d8cf0cb2

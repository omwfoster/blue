#include "FastLED.h"

#define NUM_LEDS 64
#define DATA_PIN 7
#define CLOCK_PIN 13


const int LED_PIN = 13; 
const int LINE_BUFFER_SIZE = 80; // max line length is one less than this

CRGB leds[NUM_LEDS];


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
    Serial.print(ch); // echo it back
    }while (ch != '\n'); 

    buffer[0] = 0; // set buffer to empty string even though it should not be used
    return-1; // error: return negative one to indicate the input was too long
    }


void setup() {

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
    

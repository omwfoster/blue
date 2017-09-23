
#include <Wire.h>
#include <HashMap.h>
#include "Countable.h"
#include <FastLED.h>
#define NUM_LEDS 64 
#define DATA_PIN 7
#define CLOCK_PIN 13
#define HASH_SIZE 26



const int LED_PIN = 13; 
const int LINE_BUFFER_SIZE = 80; // max line length is one less than this
CRGB leds[NUM_LEDS]; 



HashType<char const *, int> hashRawArray[HASH_SIZE];
HashMap<char const *, int> strangeLed = HashMap<char const *, int>(hashRawArray, HASH_SIZE);


void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } } 

int read_line(char * buffer, int bufsize) {
    clear_input_buffer(buffer, 20);
    for (int index = 0; index < bufsize; index++) {
        // Wait until characters are available
            while (Serial.available() == 0) {
                cylon();
            }

            char ch = Serial.read(); // read next character
            Serial.print(ch); // echo it back: useful with the serial monitor (optional)

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
    LEDS.setBrightness(84); 

    strangeLed[0]("a", 1);
    strangeLed[1]("b", 1);
    strangeLed[2]("c", 1);
    strangeLed[3]("d", 1);
    strangeLed[4]("e", 1);
    strangeLed[5]("f", 1);
    strangeLed[6]("g", 1);
    strangeLed[7]("h", 1);
    strangeLed[8]("i", 1);
    strangeLed[9]("j", 1);
    strangeLed[10]("k", 1);
    strangeLed[11]("l", 1);
    strangeLed[12]("m", 1);
    strangeLed[13]("n", 1);
    strangeLed[14]("o", 1);
    strangeLed[15]("p", 1);
    strangeLed[16]("q", 1);
    strangeLed[17]("r", 1);
    strangeLed[18]("s", 1);
    strangeLed[19]("t", 1);  
    strangeLed[20]("u", 1);
    strangeLed[21]("v", 1);
    strangeLed[22]("w", 1);
    strangeLed[23]("x", 1);
    strangeLed[24]("y", 1);
    strangeLed[25]("z", 1);
    strangeLed[26]("x", 1);
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
        Serial.print("Error: unknown command: \"");
        Serial.print(line);
        Serial.println("\" (available commands: \"off\", \"on\")"); 
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
        Serial.print("x"); 
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
        Serial.print("x"); 
     
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
    Serial.print("x"); 
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
    Serial.print("x"); 
 
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


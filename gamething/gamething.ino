#include <Bounce2.h>
#include <keyboard.h>

/*  PiBoy MCU Control

 Connects to Rpi through SPI connection for button input, power control,
 and controls the power LED. 4-way directional pad + 14 buttons. Optional
 low battery and charging input and RGB LED for status.

 The circuit:
 * Connects up to 14 individual buttons (including directional). Uses
 internal pull-up and buttons short to ground.
 * Power button must be toggle switch (not a momentary switch). Uses 
internal pull-up and shorts to ground for on position.


 * Note: Coded specifically for the Attiny48/88 either 28-pin or 32-pin 
(but may work with other chips). The 32-pin has optional extra features. 
Optional features can be selected, Power management IC input and RGB 
status LED.

 created 2016
 by VRPC <http://www.vrtualcomputers.com>

 This code is public domain.

 References:
	https://github.com/cuddleburrito/gamething-controller
	https://github.com/thomasfredericks/Bounce2/wiki

*/

//---------
// USE THESE FOR THE buttonPresets ARRAY BELOW
#define KEY_LEFT_CTRL  0x80
#define KEY_LEFT_SHIFT 0x81
#define KEY_LEFT_ALT   0x82
#define KEY_LEFT_GUI   0x83  
#define KEY_RIGHT_CTRL 0x84
#define KEY_RIGHT_SHIFT 0x85 //Default Select
#define KEY_RIGHT_ALT  0x86
#define KEY_RIGHT_GUI  0x87

#define KEY_UP_ARROW   0xDA   //Default Up
#define KEY_DOWN_ARROW 0xD9   //Default Down
#define KEY_LEFT_ARROW 0xD8   //Default Left
#define KEY_RIGHT_ARROW 0xD7  //Default Right
#define KEY_BACKSPACE  0xB2
#define KEY_TAB        0xB3
#define KEY_RETURN 0xB0       //Default Start
#define KEY_ESC        0xB1

//---------

//#define PWR_LED 13 //
#define PWR_SW 2
//#define MOSI  //Setup SPI pins RX (Default 12)
//#define MISO  //Setup SPI pins TX (Default 11, Rpi is not 5v tolerant)
// SPI SCK pin default 13
//#define SCL //Setup I2C pins (Default 19/A5)
//#define SDA //Setup I2C pins (Default 18/A4)

//========== END CONFIGURATION SETTINGS ==========

// int RXLED = 17;  // The RX LED has a defined Arduino pin
// Instantiate button state array
boolean buttonPressed[15];
// Instantiate a Bounce object array to store each debouncer in
Bounce debouncers[15];
//Instantiate a counter array for each button to debounce count timer
int debounceCount[15];

  //Button layout based on Super Nintendo + optional L2&R2 for PSX games
int buttonPins[] = {
  2, //Power Switch
  4, //D-Pad up
  5, //D-Pad down
  6, //D-Pad left
  7,  //D-Pad right
  9, //button B (Down) (Cancel) (Keyboard Z)
  8, //button A (Right) (Accept) (Keyboard X)
  7, //button Y (Left)
  3, //button X (Up)
  4, //button Start (Keyboard: Return)
  2, //button Select (Keyboard: Right Shift)
  18, //Trigger L1
  19 //Trigger R1 (Remember to add comma if using options)
//  18, //Trigger L2 (Optional)
//  19 //Trigger R2 (Optional)
};

char buttonPresets[] = { 
  KEY_UP_ARROW, //joystick up
  KEY_DOWN_ARROW, //joystick down
  KEY_LEFT_ARROW, //joystick left
  KEY_RIGHT_ARROW, // joystick right
  KEY_LEFT_CTRL, //button 1
  KEY_LEFT_ALT, //button 2
  ' ', //button 3
  KEY_Right_SHIFT, //button 4
  'z', //button 5
  'x', //button 6
  'a', //button 7
  's', //button 8
  '9', //button 9
  'q', //Trigger L1
  'w', //Trigger R1
//  'q', //Trigger L2
//  'w' //Trigger R2
};

//========== END CONFIGURATION SETTINGS ==========

int RXLED = 17;  // The RX LED has a defined Arduino pin
// Instantiate button state array
boolean buttonPressed[16];
// Instantiate a Bounce object array to store each debouncer in
Bounce debouncers[16];
//Instantiate a counter array for each button to debounce count timer
int debounceCount[16];

void setup() {
  // put your setup code here, to run once:

  pinMode(RXLED, OUTPUT);  // Set RX LED as an output
 // TX LED is set as an output behind the scenes 
 
   //Setup the LED :
 pinMode(LED_PIN,OUTPUT);
 digitalWrite(RXLED, LOW);
  
 
 Keyboard.begin();
 
  // Create debounce instances :
   for (int i = 0; i < 16; i++) {
     debouncers[i] = Bounce();
     debounceCount[i] = 0;
     pinMode(buttonPins[i],INPUT_PULLUP);
     (debouncers[i]).attach(buttonPins[i]);
     (debouncers[i]).interval(BOUNCE_WAIT);
        delay(100);
     buttonPressed[i] = false;, 
   }
     
}

void loop() {
  
  for (int j = 0; j < 16; j++) { //iterate over each button (pin)
    
     (debouncers[j]).update(); //check current value
     int value = (debouncers[j]).read();
     
     if ( value == LOW ) { //if button pressed
       
       if(debounceCount[j] == BOUNCE_COUNT && buttonPressed[j] == false) { //the button has been held down long enough and it hasn't been previously registered as pressed
          Keyboard.press(char(buttonPresets[j])); //Keyboard.write('1');
          buttonPressed[j] = true;
        } else {
            if(debounceCount[j] < BOUNCE_COUNT) { 
              debounceCount[j] = debounceCount[j] + 1; //else increment the count
            }
        }
        
      } else { //button is not pressed
        
        if(debounceCount[j] > 0) {
          debounceCount[j] = debounceCount[j] - 1; //keep decreasing count unless 0
        } else {
           Keyboard.release(char(buttonPresets[j])); //if 0 then release button
           buttonPressed[j] = false;
        }
        
      }

  }

}

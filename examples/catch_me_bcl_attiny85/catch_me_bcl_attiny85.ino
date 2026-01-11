/*
 * Catch me bi-color for ATtiny85
 *  
 * Created 2025 by Robert Sedak
 * 
 * This example code is in the public domain.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHOR(S) OR 
 *  CONTRIBUTOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 *  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
 *  THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Charlieplexing: D0, D1, D2
 * Green LED: D3
 * Push-button: D4
 *
 * Press button when GREEN LED is turned on. 
 * If the push-button is pressed when GREEN LED is turned on, the duration for which the 
 * LED remains lit is reduced by approximately 12%.
 * If you press push-button when RED LED is turned on:
 *   - The LED light duration is configured to the initial level.
 *   - LED0 blinks a number of times equal to the tens digit of the current level number.
 *   - LED1 blinks a number of times equal to the units digit of the current level number.
 * 
 * Game levels: 21
 * 
 * 
 * Programmed with Arduino IDE and 
 * Arduino core for attiny13 - http://drazzy.com/package_drazzy.com_index.json
 *  
 * Tested on ATtiny85 running at 1MHz.
 * Programing parameters with Arduino IDE:
 * Board: ATtiny25/45/85 (No bootloader)
 * Chip: ATtiny85
 * Clock: 4MHz (internal)
 * Timer 1 Clock: CPU
 * LTO: Enabled
 * millis()/micros(): Disabled
 * Save EEPROM: EEPROM retained
 * BOD: Disabled
 * 
 */

const byte OCR0A_initial_value = 3;      // Inital value for OCR0A to trigger interrupt every  3.07ms
const byte OCR0B_initial_value = 32;     // Inital value for OCR0B to trigger interrupt every 32.76ms


/* -------------------------------------------------------------------------------------
*  The following definitions clarify the operation of Charlieplexed LEDs, 
*  focusing on the flow of current between pairs of ports.
    - 7:5 Bits: These bits determine which port should be set to a high state for green (active).
    - 4:2 Bits: These bits determine which port should be set to a high state for red (active).
    - 1:0 Bits: These bits determine which port should be set to a low state for common pin(inactive).
   -------------------------------------------------------------------------------------
*/
const byte CharlieLED[7] = {
  0b01000100,                            // LED 1
  0b10001100,                            // LED 2
  0b01000001,                            // LED 3
  0b10001101,                            // LED 4
  0b00100010,                            // LED 5  
  0b10001110,                            // LED 6
  0b00100011                             // LED 7
};


/* ------------------------------------------------------------------
*  Specifies the sequence of green LEDs that will be turned on.
*  ------------------------------------------------------------------
*/
const byte greenLedSequence[7] = {0, 4, 1, 5, 2, 6, 3};


/* ------------------------------------------------------------------
*  Defines the various playing levels along with the corresponding 
*  durations for which the LED lights will remain turned on.
*  ------------------------------------------------------------------
*/
const byte levels = 21;
const byte duration[levels] = {
  154, 135, 119, 104, 92, 81, 71,
  62, 54, 48, 42, 38, 33, 30,
  27, 23, 21, 19, 17, 15, 12,
};

/* --------------------------------------------------------------------
*   Lists the variables necessary for the program to function correctly.
*  --------------------------------------------------------------------
*/
volatile uint16_t currentticks = 0;
volatile uint16_t lastticks = 0;
volatile bool previousSwitchReading = 1;
volatile bool buttonIsPressed = 0;
volatile bool buttonWasPressed = 0;
volatile byte currentLED = 0;
volatile byte LEDstep = 1;
volatile byte level = 0;
volatile byte greenLED = 3;
volatile byte greenLedPosition = 0;


void setup() {
  pinMode(0, INPUT);      // set pin 0 to 
  digitalWrite(0, LOW);   // Hi-Z state
  pinMode(1, INPUT);      // set pin 1 to 
  digitalWrite(1, LOW);   // Hi-Z state
  pinMode(2, INPUT);      // set pin 2 to 
  digitalWrite(2, LOW);   // Hi-Z state
  pinMode(2, INPUT);      // set pin 2 to 
  digitalWrite(2, LOW);   // Hi-Z state
  pinMode(3, INPUT);      // set pin 3 to 
  digitalWrite(3, LOW);   // Hi-Z state
  pinMode(4, INPUT);      // set pin 4 to 
  digitalWrite(4, LOW);   // Hi-Z state
  pinMode(5, INPUT_PULLUP);
  
  cli();                                   // Disable global interrupts
  TCCR0A = 0;                              // Clean the register
  TCCR0B = 0;                              // Clean the register
  TCCR0B |= (1<<CS02) | (1<<CS00);         // Start timer prescaler to 1024
  TIFR  |= (1 << OCF0A) | (1 << OCF0B);    // Enable the Timer/Counter0 Compare Match Interrupt
  TIMSK |= (1 << OCIE0A) | (1 << OCIE0B);  // Enable the Counter Output Compare Match A and B Interrupt
  OCR0A = OCR0A_initial_value;             // Set OCR0A
  OCR0B = OCR0B_initial_value;             // Set OCR0B
  TCNT0 = 0;                               // Reset Timer/Counter Register
  sei();
}


/* ------------------------------------------------------------------
*  An interrupt function that counts ticks, 
*  serving as a substitute for the traditional delay function.
*  ------------------------------------------------------------------
*/
ISR(TIM0_COMPA_vect)
{
  OCR0A += OCR0A_initial_value;            // set counter for the next trigger
  ++currentticks;                          // increase counter
} // end of function ISR()

/* ------------------------------------------------------------------
*  An interrupt function that detects when a push-button is pressed, 
*  incorporating a debounce algorithm to ensure reliable operation.
*  ------------------------------------------------------------------
*/
ISR(TIM0_COMPB_vect)
{
  OCR0B += OCR0B_initial_value;       // set counter for the next trigger
  bool currentSwitchReading = digitalRead(5);
  if (currentSwitchReading == previousSwitchReading)
  {
    if (!currentSwitchReading){
      buttonIsPressed = 1;
    } else {
      buttonIsPressed = 0;
    }
  }
  previousSwitchReading = currentSwitchReading;
} // end of function ISR()

/* ------------------------------------------------------------------
*  Substitute for the traditional delay function.
*  ------------------------------------------------------------------
*/
void delayLED(int interval) {
  currentticks = 0;               // I want 8 bit numbers comparation
  lastticks = currentticks;
  while (interval > currentticks - lastticks) {};
} // end of function delayLED()


/* ------------------------------------------------------------------
*  This function blinks #thisLED a specified number of #times. 
*  Its purpose is to provide visual confirmation that the push-button
*  has been pressed and to inform the player of the total number of 
*  times the button was activated while the green LED was turned on.
*  ------------------------------------------------------------------
*/
void blinkLED(byte thisLED, byte times) {
  for (int i = 1; i <= times; i++) {
    LEDON(thisLED);
    delayLED(100);
    LEDOFF(thisLED);
    delayLED(100);
  }
} // end of function blinkLED()


void loop() {
  LEDON(currentLED);
  currentticks = 0;          // I want 8 bit numbers comparation
  lastticks = currentticks;
  switch(currentLED){
    case 6:                  // let's switch directioN from highest LED to lowest
      LEDstep = -1;
      break;
    case 0:                  // let's switch direction from lowest LED to highest
      LEDstep = 1;
      break;
    default:                 // do nothing, everything is fine
      break;
  }
  while (duration[level] > currentticks - lastticks) {
    if (buttonIsPressed) {
      buttonWasPressed = 1;
      break;
    }
  }
  while (buttonIsPressed == 1) {};

  LEDOFF(currentLED);
  
  if (buttonWasPressed) {     // when push-button is presses, fun time 
    delayLED(100);            // wait so playe see LED is not lit
    blinkLED(currentLED, 2);  // let current LED blink two times
    if (currentLED == greenLED && level < levels) {  // button had been pusshed when green LED was turned on
      level++;                                       // move to the next game level
      greenLED = greenLedSequence[greenLedPosition]; // change green LED position
      greenLedPosition++;
      if (greenLedPosition == 7) {
        greenLedPosition = 0;
      }
    } else {                  // show how many times player pushed button when green LED was turned on
      blinkLED(0, level/10);  // show tens
      blinkLED(1, level%10);  // show units
      level = 0;              // start over
    }
    buttonWasPressed = 0;
  }

  currentLED += LEDstep;        // change the LED to be turned on
} // end of function loop()


/* --------------------------
*  Turn the LED #thisLED off.
* ---------------------------
*/
void LEDOFF(byte thisLED) {
  // turn off
  byte pin1 = (CharlieLED[thisLED] & 0b00011100) >> 2;  // find pin1 for red
  if(thisLED == greenLED) {
    pin1 = (CharlieLED[thisLED] & 0b11100000) >> 5;  // find pin1 for green
  }
  byte pin2 = CharlieLED[thisLED] & 0b00000011;         // find pin2
  pinMode(pin1 , INPUT);    // set pin1 to
  digitalWrite(pin1, LOW);  // Hi-Z state
  pinMode(pin2, INPUT);     // set pin2 to
  digitalWrite(pin2, LOW);  // Hi-Z state
} // end of function LEDOFF()


/* -------------------------
*  Turn the LED #thisLED on.
*  -------------------------
*/
void LEDON(byte thisLED) {
  // turn on the one that's in focus
  byte pin1 = (CharlieLED[thisLED] & 0b00011100) >> 2;  // find pin1 for red
  if(thisLED == greenLED) {
    pin1 = (CharlieLED[thisLED] & 0b11100000) >> 5;  // find pin1 for green
  }
  byte pin2 = CharlieLED[thisLED] & 0b00000011;         // find pin2
  pinMode(pin1, OUTPUT);
  digitalWrite(pin1, HIGH);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin2, LOW);
} // end of function LEDON()

/*
 * Mushroom driver 1.0
 * Author: anilsson of 1069 Design
 * Interrupt on digital pin 1
 * 10 K resistor on digital pin 1 to GND
 */

#include <avr/sleep.h>
#include <avr/wdt.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

const int redPin = 2;
const int grnPin = 1;
const int bluPin = 0;
const int analogPin = 3;
const int statusLed = 4;
int val = 0;

typedef void (*FunctionCallback)(int);
FunctionCallback functions[] = { &redtoyellow,&yellowtogreen,&greentocyan,&cyantoblue,&bluetomagenta,&magenatored };

//This runs each time the watch dog wakes us up from sleep
ISR(WDT_vect) {
  //Don't do anything. This is just here so that we wake up.
  digitalWrite(statusLed, HIGH);
}

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(grnPin, OUTPUT);
  pinMode(bluPin, OUTPUT);
  system_sleep(); // First thing we do is sleep until wake up is triggered
}

void loop()
{
  digitalWrite(statusLed, HIGH);
  for(int func = 0; func < 10; func ++){
    val = analogRead(analogPin);    // read the input pin
    if (val < 100){
      (*functions[func])(200);
    } else {
      off();
      system_sleep();
    }
  }
}

void off() {
  digitalWrite(redPin, LOW);
  digitalWrite(grnPin, LOW);
  digitalWrite(bluPin, LOW);
}


/* Sleep function
 * source: http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/
 */
void system_sleep() {
  cbi(ADCSRA, ADEN); // Switch Analog to Digital converter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode
  setup_watchdog(6); //Setup watchdog to go off after 1sec
  digitalWrite(statusLed, LOW);
  sleep_mode(); // System sleeps here
  sbi(ADCSRA, ADEN); // Switch Analog to Digital converter ON
}


//Sets the watchdog timer to wake us up, but not reset
//0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
//6=1sec, 7=2sec, 8=4sec, 9=8sec
//From: http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/
void setup_watchdog(int timerPrescaler) {

  if (timerPrescaler > 9 ) timerPrescaler = 9; //Limit incoming amount to legal settings

  byte bb = timerPrescaler & 7;
  if (timerPrescaler > 7) bb |= (1 << 5); //Set the special 5th bit if necessary

  //This order of commands is important and cannot be combined
  MCUSR &= ~(1 << WDRF); //Clear the watch dog reset
  WDTCR |= (1 << WDCE) | (1 << WDE); //Set WD_change enable, set WD enable
  WDTCR = bb; //Set new watchdog timeout value
  WDTCR |= _BV(WDIE); //Set the interrupt enable, this will keep unit from resetting after each int
}


void redtoyellow(int microseconds)
{
  digitalWrite(redPin, HIGH);
  digitalWrite(bluPin, LOW);
  // fade up green
  for (byte i = 1; i < microseconds; i++) {
    byte on = i;
    byte off = microseconds - on;
    for ( byte a = 0; a < microseconds; a++ ) {
      digitalWrite(grnPin, HIGH);
      delayMicroseconds(on);
      digitalWrite(grnPin, LOW);
      delayMicroseconds(off);
    }
  }
}

void yellowtogreen(int  microseconds)
{
  digitalWrite(grnPin, HIGH);
  digitalWrite(bluPin, LOW);
  // fade down red
  for (byte i = 1; i < microseconds; i++) {
    byte on = microseconds - i;
    byte off = i;
    for ( byte a = 0; a < microseconds; a++ ) {
      digitalWrite(redPin, HIGH);
      delayMicroseconds(on);
      digitalWrite(redPin, LOW);
      delayMicroseconds(off);
    }
  }
}

void greentocyan(int  microseconds)
{
  digitalWrite(grnPin, HIGH);
  digitalWrite(redPin, LOW);
  // fade up blue
  for (byte i = 1; i < microseconds; i++) {
    byte on = i;
    byte off = microseconds - on;
    for ( byte a = 0; a < microseconds; a++ ) {
      digitalWrite(bluPin, HIGH);
      delayMicroseconds(on);
      digitalWrite(bluPin, LOW);
      delayMicroseconds(off);
    }
  }
}

void cyantoblue(int  microseconds)
{
  digitalWrite(bluPin, HIGH);
  digitalWrite(redPin, LOW);
  // fade down green
  for (byte i = 1; i < microseconds; i++) {
    byte on = microseconds - i;
    byte off = i;
    for ( byte a = 0; a < microseconds; a++ ) {
      digitalWrite(grnPin, HIGH);
      delayMicroseconds(on);
      digitalWrite(grnPin, LOW);
      delayMicroseconds(off);
    }
  }
}

void bluetomagenta(int  microseconds)
{
  digitalWrite(bluPin, HIGH);
  digitalWrite(grnPin, LOW);
  // fade up red
  for (byte i = 1; i < microseconds; i++) {
    byte on = i;
    byte off = microseconds - on;
    for ( byte a = 0; a < microseconds; a++ ) {
      digitalWrite(redPin, HIGH);
      delayMicroseconds(on);
      digitalWrite(redPin, LOW);
      delayMicroseconds(off);
    }
  }
}

void magenatored(int  microseconds)
{
  digitalWrite(redPin, HIGH);
  digitalWrite(grnPin, LOW);
  // fade down blue
  for (byte i = 1; i < microseconds; i++) {
    byte on = microseconds - i;
    byte off = i;
    for ( byte a = 0; a < microseconds; a++ ) {
      digitalWrite(bluPin, HIGH);
      delayMicroseconds(on);
      digitalWrite(bluPin, LOW);
      delayMicroseconds(off);
    }
  }
}


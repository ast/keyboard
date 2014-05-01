#include <MsTimer2.h>

#include "types.h"

key_t keys[88];

#define NUM_BANKS 11

// For scanning banks
bank_t banks[NUM_BANKS];
bank_t prev_banks[NUM_BANKS];

void trigger(key_t *key, event_t event) {

  if(event == KEY_PRESSED) {
    key->state = KEY_IS_GOING_DOWN;
    
  } else if (event == KEY_DOWN) {
    key->state = KEY_IS_DOWN;
    usbMIDI.sendNoteOn(key->midi_note, 127- key->t, 1);
    key->t = 0;
    
  } else if (event == KEY_RELEASED) {
    key->state = KEY_IS_GOING_UP;
    
  } else if ( event == KEY_UP) {
    key->state = KEY_IS_UP;
    usbMIDI.sendNoteOff(key->midi_note,  127 - key->t, 1);
    key->t = 0;
    
  }
  
}

void setup() {
  DDRD = 0b11111111; // Set D0-D7 as outputs 
  
  DDRC= 0b00000000; // Set D0-D7 as inputs
  PORTC = 0b11111111; // Enable pull up resistors
  
  DDRF = DDRF | 0b11111110;
  PORTF = PORTF | 0b00000001;
  
  // Init array
  memset(banks, 0xff, sizeof(prev_banks));
  
  // Init keys
  for ( int key = 0; key < 88; key++) {
    keys[key].midi_note = 21 + key;
    keys[key].t = 0;
  }
  
  MsTimer2::set(1, timeout); // 1ms period
  MsTimer2::start();
}

void timeout() {
   scan();
   increment();
   
   footpedal();

}

void increment() {
     // Advance timers
   for(int key = 0; key < 88; key++) {
     state_t state = keys[key].state;
     if(state == KEY_IS_GOING_UP || state == KEY_IS_GOING_DOWN) {
       keys[key].t++;
     }
   }
}

void footpedal() {
  
  static int prev_val = 1;
  
  int val = PINF & 0b00000001;
  
  if(val != prev_val) {
    usbMIDI.sendControlChange(64, (val ? 1 : 64), 1);
    prev_val = val;
  }
  
}

void scan() {

  // Scan and store
  for(int bank = 0; bank < NUM_BANKS; bank++) {
    prev_banks[bank] = banks[bank]; // Store previous state so we can look for changes
    
    PORTD = bank * 2; // Selects bottom row
    delayMicroseconds(10); // Debounce
    banks[bank].bottom = PINC;
    
    PORTD = bank * 2 + 1; // Selects top row
    delayMicroseconds(10); // Debounce
    banks[bank].top = PINC;
  }
  
  // Process
  for(int bank = 0; bank < NUM_BANKS; bank++) {
    
     byte diff;
    
    // Check top switches and fire events
    diff = banks[bank].top ^ prev_banks[bank].top;
    if(diff) {
      for(int key = 0; key < 8; key++) {
        if(bitRead(diff, key)) { 
          event_t event = bitRead(banks[bank].top, key) ? KEY_UP : KEY_PRESSED;
          trigger(&keys[bank * 8 + key], event);
        }
      }
    }
    
    // Check bottom switches and fire events
    diff = banks[bank].bottom ^ prev_banks[bank].bottom;
    if(diff) {
      for(int key = 0; key < 8; key++) {
        if(bitRead(diff, key)) { 
          event_t event = bitRead(banks[bank].bottom, key) ? KEY_RELEASED : KEY_DOWN;           
          trigger(&keys[bank * 8 + key], event);
        }
      }
    }
    
  }

}

void loop() { } // Idle

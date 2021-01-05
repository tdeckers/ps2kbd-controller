/*
 * File:   main.c
 * Author: tom@ducbase.com
 *
 * Created on December 18, 2020, 9:12 PM
 */



// PIC16F1826 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "keymap.h"

#define _XTAL_FREQ 8000000 // this is used by the __delay_ms(xx) and __delay_us(xx) functions

#define byte unsigned char

#define KBCLK       RA0     // bidir
#define KBCLK_DIR   TRISA0
#define KBDATA      RA1     // bidir
#define KBDATA_DIR  TRISA1
#define DATARDY     RA2     // out
#define DATARDY_DIR TRISA2
#define DATAACK     RA3     // in
#define DATAACK_DIR TRISA3
#define RST         RA4     // in
#define RST_DIR     TRISA4

#define DEBUG        RA6
#define DEBUG_DIR    TRISA6

#define DIR_OUT 0
#define DIR_IN  1

/*
 * special register bit defs (for test opcode)
 */
#define SCRLLOCK	0			// 01
#define NUMLOCK		1			// 02
#define CAPSLOCK	2			// 04
#define CONTROL     3			// 08
#define SHIFT		4			// 10
#define EXT			5			// 20
#define EXTSH		6			// 40  
#define RLSE		7			// 80  key release code

int special = 0;
byte data;
byte lastbyte;
byte parity;
int cntr;

byte read_bit;


void setup(void) {
    di();   // disable interrupts(XC8 code)
        
    OSCCON = 0b01110000;  // 8 Mhz
    // configure I/O direction: 1=input / 0=output
    TRISB = 0; // port A all output (to host)
    ANSELA = 0; // use digital I/O
    ANSELB = 0;
    KBCLK_DIR = DIR_IN; // start as input.
    KBDATA_DIR = DIR_IN; // start as input.
    DATARDY_DIR = DIR_OUT;
    DATARDY = 1;        // active low
    DATAACK_DIR = DIR_IN;
    RST_DIR = DIR_IN;
    DEBUG_DIR = DIR_OUT;
    DEBUG = 0;  
}

/*
 * https://stackoverflow.com/questions/21617970/how-to-check-if-value-has-even-parity-of-bits-or-odd
 * but modified
 */
static byte check_parity(byte data, byte parity) {
    static byte count, i;
    count = 0;
    for (i = 0; i < 8; i++) {
        if (data & (1 << i)) {
            count++;
        }
    }
    if (parity == 1) {
        count++;
    }
    if ((count % 2)) {
        return 1;
    }

    return 0;
}

/*
 * Reverse the bits in a byte.
 * https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
 */
static byte reverse(byte b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

/*
 * process single clock pulse.  On the falling edge, capture data bit and 
 * return. This bit will be ignored for send().
 */
static byte highLow() {
    while (!KBCLK) {
    } // wait while clock is low
    while (KBCLK) {
    } // wait while clock is high
    return KBDATA;
}

/*
 * disable keyboard from sending.  Pulling the clock low inhibits the keyboard.
 */
static void disable() {
    KBCLK_DIR = DIR_OUT;
    KBCLK = 0;
}

/*
 * enable keyboard for receiving.  Ensure clock is high first.
 */
static void enable() {
    KBCLK_DIR = DIR_OUT;
    KBCLK = 1;
    KBCLK_DIR = DIR_IN;    
}

/*
 * get a single byte from the keyboard.
 */
static byte get() {
    static byte stop;
    static byte i;
    
    KBCLK_DIR = DIR_IN;
    KBDATA_DIR = DIR_IN;
    // Need to be quick in this section.  Need to keep up with keyboard clock.
    while (KBCLK && KBDATA) {
        // wait while clock is high
        // wait while data is high.
    }
    highLow();   // cycle through the start bit.
    // process 8 data bits.
    for (i = 0; i < 8; i++) {
        read_bit = highLow();
        // shift bits in from left (wrong order).  We're not fast enough to
        // do clever things here.  We're reverse the bits later.
        data = (data << 1) + read_bit;     
    }
    parity = highLow(); // get parity bit (odd parity)
    // TODO: validate odd parity.  send 0xFE and do get again.

    stop = highLow();   // stop bit must be 1.
    // TODO: if stop != 1: error.  send 0xFE and do get again.
    
    disable(); // disable the keyboard while we're processing
    // End of quick section.  We got all bits.
    data = reverse(data);  // reverse the bits here.
    
    enable(); // enable the keyboard before we leave.
    
    return data;
}

/*
 * send a single byte to the keyboard.
 */
static void send(byte d) {
    static byte bits[8];
    static byte i;
    // prepare the data.  We don't want to do this during the pulses.
    // We might be too slow.
    for (i = 0; i < 8; i++) {
        bits[i] = (d & (1 << i)) >= 1;
    }
    // calculate the parity.  We can use the check function.
    parity = check_parity(d, 1);
    KBCLK_DIR = DIR_OUT;
    KBDATA_DIR = DIR_OUT;
    // Pull keyboard clock low for 100us to indicate we want to send.
    // During this 100us, pull data low.
    KBCLK = 0;
    __delay_us(80);
    KBDATA = 0;     // data needs to go low before clock goes high again.
    __delay_us(20);
    KBCLK = 1;    
    KBCLK_DIR = DIR_IN;

    // keyboard will now start sending clock that we use to send data    
    for (i = 0; i < 8; i++) {
        //lowHigh(d & (1 << i));
        highLow();
        __delay_us(15);     // wait for the center of the pulse to change.
        KBDATA = bits[i];
    }
    
    highLow();              // wait for the parity bit clock
    __delay_us(15);
    KBDATA = parity;        // send parity bit.
    highLow();              // wait for the stop bit clock.
    __delay_us(15);
    KBDATA = 1;             // send stop.
    KBDATA_DIR = DIR_IN;    // prepare for ACK
    read_bit = highLow();   // read ACK
    // TODO check read_bit = 0 (ack)
}

static void setleds() {
    static int done;
    done = -1;
    while (done < 0) {          // as long as we didn't get an ack...
        send(0xED);             // send command indication
        data = get();           // get response
        if (data == 0xFA) {     // if ack...
            send( special & 0x07 ) ;  // ... send led data ... 
            done = 1;           // and set done.            
        }        
    }
}

static void initkbd() {
    special |= 1 << NUMLOCK;
    static int done;
    done = -1;
    
    while (done < 0) {
        send(0xFF);             // send command reset
        data = get();           // get response
        if (data == 0xFA) {     // if ack ..
            data = get();       // 
            if (data == 0xAA) { // get ok.
                done = 1;       // we're done.
            }            
        }

    }    
    
    setleds();
}

static byte lookup(byte d) {
    static byte retVal;
    if (special & (1<<SHIFT) || special & (1<<EXTSH)) {
        retVal = keymap_shifted[d];
    } else {
        
        retVal = keymap[d];
    }    
    
    // process numpad keys when numlock is on.
    if (d >= 0x69 && d <= 0x7D          // if numpad            
            && d != 0x78) {             // not F11 
        if (! (special & (1 << NUMLOCK)) ) {    // if numlock off, 
            retVal = keymap_shifted[d];         // return shifted values.
        }
    }
    
    if (special & (1 << CONTROL)) {     // control key active?
        // not sure what to do here.
    }
    
    if (special & (1 << RLSE)) {        // release code
        retVal = 0x00;                  // clear any code that got set
    }
    
    if ((special & (1 << CAPSLOCK))     // caps lock on?
            && retVal >= 0x61 && retVal <= 0x7A ) {     // and character?
        retVal &= 0xDF;                 // to upper case.
    }
    return retVal;
}

/*
 * process key release code.
 */
static void rlsproc(int * retVal, byte * data) {
    special &= ~(1<<RLSE);  // clear release flag
    if (*data == 0x11) {     // alt released?
        *retVal = 0xE0;
    } else if (*data == 0x12 || *data == 0x59) {  // shift released?
        special &= ~(1<<SHIFT);
    } else if (*data == 0x14) {  // ctrl released?
        special &= ~(1<<CONTROL);
    } else {
         // else don't care, get next key.
    }    
}

static void extproc(int * retVal, byte * data) {
    special &= ~(1 << EXT);         // clear the ext flag
    if (*data == 0x12) {
        special |= 1 << EXTSH;      // set ext shift flag.
    } else if (*data == 0x7C) {      // E07C - print screen?
        special &= ~(1<<EXTSH);     // turn off ext shift flag?
        *retVal = lookup(0x0F);
    } else if (*data == 0x4A) {      // keypad div key
        *retVal = 0x2F;
    } else if (*data == 0xF0) {      // was it a release code?
        *data = get();
        if (*data == 0x12) {         // was it ext shift?
            special &= ~(1<<EXTSH);
        }
        rlsproc(retVal, data);
    } else if (*data == 0x7E) {        // ctrl-brk key?
        *retVal = 0x02;
    } else if (*data > 0x68 ) {  // if key is keypad key
        *retVal = lookup(*data);
    }    
}

/*
 * main tests for functional key presses.
 * 
 * ref: exttst
 */
static void testkeys(int * retVal, byte * data) {
    if (*data == 0x14) {                    // CTRL key
        special |= 1 << CONTROL;
    } else if (*data == 0x58) {             // caps lock
        if ( special & (1 << CAPSLOCK) ) {  // if 1
            special &= ~(1<<CAPSLOCK);      // clear it.
        } else {                            // else set it.
            special |= 1 << CAPSLOCK;
        }
        setleds();
        
    } else if (*data == 0x77) {             // num lock
        if ( special & (1 << NUMLOCK) ) {   // if 1
            special &= ~(1<<NUMLOCK);       // clear it.
        } else {                            // else set it.
            special |= 1 << NUMLOCK;
        }
        setleds();
    } else if (*data == 0x7E) {             // scroll lock
        if ( special & (1 << SCRLLOCK) ) {  // if 1...
            special &= ~(1<<SCRLLOCK);      // clear it,
        } else {                            // else set it.
            special |= 1 << SCRLLOCK;
        }
        setleds();
        *retVal = lookup(0x7E);     // setleds might have changed *data!
        
    } else if (*data < 0x80) {
        *retVal = lookup(*data);
            
    } else if (*data == 0x83) {     // F7 key?
        *retVal = lookup(0x02);     // lookup relocated value.
        
    } else if (*data == 0xE0) {  // ext code
        special |= 1 << EXT;
    
    } else if (*data == 0xE1) {     // BRK key
        // flush 7 break key codes.
        get(); get(); get(); get(); get(); get(); get();
        *retVal = 0x03;
        
    } else if (*data == 0xF0) {  // key release code?
        special |= 1 << RLSE;   // set release state
                                   // get the next byte;
    }
}

static byte kbdinput() {
    static int retVal;
    retVal = -1;
    while(retVal == -1) {
        data = get();
        if (special & 1 << RLSE) {  // if release flag set
            rlsproc(&retVal, &data);
            
        } else if (special & 1 << EXT) {    // is ext flag active?
            extproc(&retVal, &data);
            
        } else if (data == 0x12 || data == 0x59) {
            special |= 1 << SHIFT;  // set shift state                    
                                    // no further action on shift.
        } else {
            testkeys(&retVal, &data);
        }
        // as long as we have no valid key, get next (loop around)

    }
    
    return retVal;
}

void main(void) {
    setup();
    
    initkbd();
    
    while (1) {
        data = kbdinput();
        PORTB = data;       // present data to 6522
        DATARDY = 0;        // activate data ready
        while (DATAACK) {   // wait for ack (active low)        
        }
        // got ACK.
        DATARDY = 1;
    }    

    return;
}
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

#define DEBUG_OK        RA6
#define DEBUG_OK_DIR    TRISA6
#define DEBUG_NOK       RA7
#define DEBUG_NOK_DIR   TRISA7

#define DIR_OUT 0
#define DIR_IN  1

/*
 * special register bit defs (for test opcode)
 */
#define SCRLLOCK	0			; 01
#define NUMLOCK		1			; 02
#define CAPSLOCK	2			; 04
#define CONTROL     3			; 08
#define SHIFT		4			; 10
#define EXT			5			; 20
#define EXTSH		6			; 40  
#define RLSE		7			; 80  key release code

byte special;
byte data;
byte lastbyte;
byte parity;
int cntr;

byte read_bit;

void blink_ok(int times) {
    static byte i;
    for (i = 0; i < times; i++) {
        DEBUG_OK = 1;
        __delay_ms(100);
        DEBUG_OK = 0;
        __delay_ms(100);
    }
}

void blink_nok(int times) {
    static byte i;
    for (i = 0; i < times; i++) {
        DEBUG_NOK = 1;
        __delay_ms(150);
        DEBUG_NOK = 0;
        __delay_ms(150);
    }
}

void setup(void) {
    OSCCON = 0b01110000;  // 8 Mhz
    // configure I/O direction: 1=input / 0=output
    TRISB = 0; // port A all output (to host)
    ANSELA = 0; // use digital I/O
    ANSELB = 0;
    KBCLK_DIR = DIR_IN; // start as input.
    KBDATA_DIR = DIR_IN; // start as input.
    DATARDY_DIR = DIR_OUT;
    DATAACK_DIR = DIR_IN;
    RST_DIR = DIR_IN;
    DEBUG_OK_DIR = DIR_OUT;
    DEBUG_NOK_DIR = DIR_OUT;
    DEBUG_NOK = 0;
    DEBUG_OK = 0;
}

/*
 * https://stackoverflow.com/questions/21617970/how-to-check-if-value-has-even-parity-of-bits-or-odd
 * but modified
 */
byte check_parity(byte data, byte parity) {
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
byte reverse(byte b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

/*
 * process single clock pulse.  On the falling edge, capture data bit and 
 * return. This bit will be ignored for send().
 */
byte highLow() {
    while (!KBCLK) {
    } // wait while clock is low
    while (KBCLK) {
    } // wait while clock is high
    return KBDATA;
}

/*
 * disable keyboard from sending.  Pulling the clock low inhibits the keyboard.
 */
void disable() {
    KBCLK_DIR = DIR_OUT;
    KBCLK = 0;
}

/*
 * enable keyboard for receiving.  Ensure clock is high first.
 */
void enable() {
    KBCLK_DIR = DIR_OUT;
    KBCLK = 1;
    KBCLK_DIR = DIR_IN;    
}

/*
 * get a single byte from the keyboard.
 */
byte get() {
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
    RA2 = 0; // debug
    RA2 = 1; // debug
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
    RA2 = 0; // debug    
    data = reverse(data);  // reverse the bits here.
    RA2 = 1; // debug
    
    enable(); // enable the keyboard before we leave.
    
    return data;
}

/*
 * send a single byte to the keyboard.
 */
void send(byte d) {
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

void main(void) {
    RA2 = 1; // debug;
    setup();
        
    __delay_ms(2000);
    blink_ok(2);    // debug.
    
    byte leds[] = {2, 6, 5, 1, 0, 1, 5, 6, 2, 0};
    int cnt = 0;
    while (1) {
        send(0xED);
        data = get();
        if (data == 0xFA) {
            send(leds[cnt++]);
        }        
        if (cnt >= (sizeof leds / sizeof leds[0])) {
            cnt = 0;
        }
        __delay_ms(100); // 1 Second Delay
    }

    return;
}
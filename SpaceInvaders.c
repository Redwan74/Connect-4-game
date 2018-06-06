// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "Sound.h"
#include "UART.h"
#include <stdio.h>
#include <stdlib.h>


// *************************** Images ***************************
const unsigned char Logo[] ={	// Connect 4 Symbol
 0x42, 0x4D, 0x4A, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xD4, 0x03, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x07, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x80, 0x00, 0x00, 0x08, 0xF8, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x08, 0x88, 0x80, 0x00, 0x00, 0x88, 0x88, 0x88, 0x00, 0x03, 0x00, 0x00, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88,
 0x88, 0x00, 0x88, 0x88, 0x8F, 0x00, 0x8F, 0xF7, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88, 0x88, 0x80, 0x78, 0x88,
 0xFF, 0x07, 0xFF, 0xFF, 0x00, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88, 0x88, 0x80, 0x07, 0x8F, 0xF0, 0x07, 0xF8, 0x7F,
 0x00, 0x88, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF7, 0x0F, 0x00, 0x88, 0x30, 0x88,
 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF7, 0x08, 0x00, 0x88, 0x88, 0x8F, 0x70, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x00, 0x00, 0x00, 0x07, 0x88, 0x70, 0x0F, 0xF0, 0x08, 0x08, 0x80, 0x0F, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x80, 0x00, 0x00, 0x78, 0x88, 0x88, 0x0F, 0xF0, 0x0F, 0x08, 0x80, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x88, 0x88, 0x88, 0x88, 0x00, 0xFF, 0x88, 0x8F, 0x00, 0x00, 0x08, 0x08, 0x80, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88,
 0x88, 0x00, 0x88, 0x88, 0xF8, 0x00, 0x00, 0x00, 0x08, 0x88, 0x7F, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88,
 0xF7, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x70, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x28, 0x87, 0x70, 0x00, 0x00, 0x00, 0x00, 0x08, 0x80, 0x07, 0x07, 0x70, 0x08, 0x70,
 0x7F, 0x8F, 0x70, 0x07, 0x78, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x88, 0x88, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x08, 0x80, 0x08, 0x07, 0x80, 0x88, 0x70, 0x7F, 0x87, 0x70, 0x88,
 0x88, 0x80, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0x77, 0x78, 0x70, 0x00, 0x88, 0x73, 0x0F, 0x80, 0x88, 0x07, 0x70, 0x88, 0x70, 0x78, 0x00, 0x08, 0xF8, 0x77, 0x80, 0x00, 0xF7,
 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x87, 0x00, 0x07, 0x70, 0x08, 0x88, 0x88, 0x08, 0x88, 0x88, 0x07, 0x78, 0x88, 0x70, 0x7F, 0x88, 0x08, 0xF7, 0x00, 0x00, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00,
 0x77, 0x88, 0x87, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x08, 0x88, 0x88, 0x07, 0x88, 0x87, 0x70, 0x7F, 0xF8, 0x08, 0xF7, 0x00, 0x00, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x68, 0x88, 0x70, 0x00,
 0x00, 0x00, 0x88, 0x88, 0x88, 0x08, 0x88, 0x88, 0x07, 0x88, 0x00, 0x70, 0x88, 0x70, 0x08, 0xF7, 0x00, 0x20, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x68, 0x88, 0x70, 0x00, 0x00, 0x00, 0x08, 0x88,
 0x87, 0x08, 0x88, 0x08, 0x08, 0x87, 0x00, 0x80, 0xF8, 0x77, 0x08, 0xF7, 0x33, 0x00, 0x08, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x68, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x08, 0x80, 0x08,
 0x07, 0xF7, 0x03, 0x80, 0x7F, 0x88, 0x00, 0xFF, 0x78, 0x00, 0x8F, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x78, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x80, 0x00, 0x00,
 0x8F, 0xFF, 0x70, 0x8F, 0xFF, 0x00, 0xFF, 0x88, 0xF7, 0x00, 0x00, 0x00, 0x08, 0x8F, 0xF7, 0x00, 0x06, 0x70, 0x00, 0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xF7, 0x00, 0x07, 0x70, 0x78, 0x8F, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x8E, 0x70, 0x68, 0x00, 0x78, 0x88, 0x88, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x0F, 0x8E, 0x88, 0xF8, 0x00, 0x78, 0x88, 0x88, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x78, 0x87,
 0x70, 0x00, 0x7F, 0x88, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xFF,
 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

};

const unsigned char cell[] = {	// Cell Symbol
0x42,0x4d,0x8e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x00,0x00,0x00,0x28,0x00,
0x00,0x00,0x08,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x01,0x00,0x04,0x00,0x00,0x00,
0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x80,
0x00,0x00,0x00,0x80,0x80,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x80,0x00,0x80,0x80,
0x00,0x00,0x80,0x80,0x80,0x00,0xc0,0xc0,0xc0,0x00,0x00,0x00,0xff,0x00,0x00,0xff,
0x00,0x00,0x00,0xff,0xff,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0xff,
0x00,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0xff,0xff,0xf0,0x00,0x00,0x0f,0xf0,0x00,0x00,0x0f
,0xf0,0x00,0x00,0x0f,0xf0,0x00,0x00,0x0f,0xff,0xff,0xff,0xff
  
};

const unsigned char oo[] ={	// AI Symbol
 0x42, 0x4D, 0x8E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0x00, 0x8F, 0x00, 0xF0, 0x00,
 0x0F, 0x00, 0xF0, 0x00, 0x0F, 0x00, 0xF8, 0x00, 0x8F, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF,

};

const unsigned char xx[] ={	// P1 Symbol
 0x42, 0x4D, 0x8E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x23, 0x2E, 0x00, 0x00, 0x23, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x08, 0xFF, 0x80, 0x00, 0x80, 0x88, 0x08, 0x00, 0xF8, 0x00,
 0x8F, 0x00, 0xF8, 0x00, 0x8F, 0x00, 0x80, 0x88, 0x08, 0x00, 0x08, 0xFF, 0x80, 0x00, 0xFF,

};
const unsigned char empty[] = {
0x42,0x4d,0x8e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x00,0x00,0x00,0x28,0x00,
0x00,0x00,0x08,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x01,0x00,0x04,0x00,0x00,0x00,
0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x80,
0x00,0x00,0x00,0x80,0x80,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x80,0x00,0x80,0x80,
0x00,0x00,0x80,0x80,0x80,0x00,0xc0,0xc0,0xc0,0x00,0x00,0x00,0xff,0x00,0x00,0xff,
0x00,0x00,0x00,0xff,0xff,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0xff,
0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

};
// *************************** End of Images ***************************

// Port F Intialization
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}

//------------UART0_Init------------
/*
void UART_Init(void){
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART1; // activate UART0
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port A
  UART0_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
  UART0_IBRD_R = 520;                    // IBRD = int(80,000,000 / (16 * 9600)) = int(520.8333)
  UART0_FBRD_R = 53;                    // FBRD = round(0.83333 * 64) = 53
  UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART0_CTL_R |= UART_CTL_UARTEN;       // enable UART
  GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1,PA0
  GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1,PA0
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTA_AMSEL_R &= ~0x03;          // disable analog functionality on PA1,PA0
}

unsigned char UART_InChar(void){
	
	while((UART1_FR_R&UART_FR_RXFE) != 0)
		continue;
  return((unsigned char)(UART1_DR_R&0xFF));
}

unsigned char UART_InCharNonBlocking(void){
  
	if((UART1_FR_R&UART_FR_RXFE) == 0){
    return((unsigned char)(UART1_DR_R&0xFF));
  } 
	else{
    return 0;
  }
}

void UART_OutString(unsigned char buffer[]){
   
	while(*buffer)
	{
		UART_OutChar(*buffer);
		buffer++;
	}

}

void UART_OutChar(unsigned char data){

  while((UART1_FR_R&UART_FR_TXFF) != 0);
  UART1_DR_R = data;
}
*/
// ------------End of UART0_Init------------

//------------UART1_Init------------
// Assumes a 80 MHz bus clock, creates 115200 baud rate
void UART_Init(void){            // should be called only once
  SYSCTL_RCGC1_R |= 0x00000002;  // activate UART1
  SYSCTL_RCGC2_R |= 0x00000004;  // activate port C
  UART1_CTL_R &= ~0x00000001;    // disable UART
  UART1_IBRD_R = 43;     // IBRD = int(80,000,000/(16*115,200)) = int(43.40278)
  UART1_FBRD_R = 26;     // FBRD = round(0.40278 * 64) = 26
  UART1_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs
  UART1_CTL_R |= 0x00000001;     // enable UART
  GPIO_PORTC_AFSEL_R |= 0x30;    // enable alt funct on PC5-4
  GPIO_PORTC_DEN_R |= 0x30;      // configure PC5-4 as UART1
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
  GPIO_PORTC_AMSEL_R &= ~0x30;   // disable analog on PC5-4
}
// Wait for new input, then return ASCII code
unsigned char UART_InChar(void){
  while((UART1_FR_R&0x0010) != 0);      // wait until RXFE is 0
  return((unsigned char)(UART1_DR_R&0xFF));
}
// Wait for buffer to be not full, then output
void UART_OutChar(unsigned char data){
  while((UART1_FR_R&0x0020) != 0);      // wait until TXFF is 0
  UART1_DR_R = data;
}
// Immediately return input or 0 if no input
unsigned char UART_InCharNonBlocking(void){
  if((UART1_FR_R&UART_FR_RXFE) == 0){
    return((unsigned char)(UART1_DR_R&0xFF));
  } else{
    return 0;
  }
}
//------------End of UART1_Init------------

// *************************** Functions Prototype ***************************
void PortF_Init(void);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Timer2_Init(unsigned long period);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
void Delay(void);
unsigned long TimerCount;
unsigned long Semaphore;

// Connect 4 Functions
void startingScreen(void);
void selectMode(void);
void cellsTable(void);

void player(void);
void computer(void);
// includes :
	int AIManager(void);
	// includes :
		int GetValue(int t);
		int NegaMax(int Depth);
	int winning(void);
	int playPosition(char z);
// *************************** End of Functions Prototype ***************************
// Variables Intialization
char matrix[8]={0};
char input[43];	// There are 42 places to play in the board, this array represent them + the one we use to move above columns
int PlayOut = 0;
int EVA = 0;
// EVA and PlayOut are working while NegaMax Function is working .. in NegaMax's way it will definitely make any winning move
// as it goes deep so when this happens PlayOut increases by 1 and EVA increases by 1 if its a winnning move or decreases by 1 if it the opposite
// so we can evaluate neutral moves for MiniMax by dividing EVA / PlayOut * 100 so we get a percentage
int provocation = 0;
int count;
// *************************** End of Variables Intialization ***************************

// For Port F
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

int main(void){
	
	PortF_Init(); 
	TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
	EnableInterrupts();  // The grader uses interrupts
  Random_Init(1);
	UART_Init();	// For AI vs AI Mode ( Player Function )
  Nokia5110_Init();
	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();	// This function & the previous are used when to print images
	
	//Connect 4 Driver
	startingScreen();
	selectMode();
}

void startingScreen(void){
	int x;
	Nokia5110_SetCursor(1,1);
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(15, 35, Logo, 0);	// On the screen : Col no. 10 ---- Row no. 35
	//Last parameter 0 means OFF if color is 0 and ON if in range 1 to 15 as 16 color pixels
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(2,5);
	Nokia5110_OutString("Welcome !");
	Delay100ms(100);
	for(x=0 ; x<6 ; x++){
		Nokia5110_SetCursor(1,x);
		Nokia5110_OutString("           ");
		Delay100ms(5);
	}
}
// End of startingScreen

void selectMode(){
	int i;
	for(i=0;i<44;i++)
	input[i]=' ';
	Nokia5110_SetCursor(0,1);
	Nokia5110_OutString("SelectMode :");
	Nokia5110_SetCursor(0,3);
	Nokia5110_OutString("P1vsAI > SW1");
	Nokia5110_SetCursor(0,5);
	Nokia5110_OutString("AIvsAI > SW2");
	
	while(1){
		Delay();
		if((GPIO_PORTF_DATA_R&0x10)==0){	// if SW1 is pressed
			Nokia5110_Clear();
			cellsTable();
			computer();
		}
		else if((GPIO_PORTF_DATA_R&0x01)==0){	// if SW2 is pressed
			Nokia5110_Clear();
			cellsTable(); 
			player();
		}
	}
}
// End of selectMode

void cellsTable(void){
	int ii=47,jj,yy=0,kk;
	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();	// This function & the previous are used when to print images
	for( kk=0; kk<6; ii++){
		yy=7;
		for(jj=0; jj<7; jj++){
			Nokia5110_PrintBMP(yy, ii, cell,0);
	    Nokia5110_DisplayBuffer(); 
			yy+=7;
		}
		ii-=8; kk++;
	} 
	Nokia5110_PrintBMP(0, 6,oo ,0);	
	Nokia5110_DisplayBuffer(); 
}
// End of cellsTable

void Delay(void){
	unsigned long volatile time;
  time = 727240*200/91;	// To delay 0.1 sec
  while(time){
		time--;
  }
}
// End of delay

void computer(void){
	int w;
	char winningtemp;
	char winningtemp2;
	while(1){
		w = AIManager();
		while(input[w-7]==' ' && w>7){
			w-=7;
		}
		input[w]='O';
		Nokia5110_PrintBMP(7*((w%7)+1), 12+(7*(5-(w/7))),oo ,0);   //col->w%7   row-->5-w/7 ======== This line for the computer symbol while playing
   	Nokia5110_DisplayBuffer(); 
		
		Delay();
		winningtemp = winning();
    if(winningtemp != 0){	// Means one of the player or the computer won OR both draw
			if(winningtemp == 1){	// if player won
				Delay100ms(50);
				Nokia5110_Clear();
				Nokia5110_SetCursor(1,2);
				Nokia5110_OutString("You Win !");
				break;
			}
      else if (winningtemp == 2){	// if Computer won
				Delay100ms(50);
				Nokia5110_Clear();
				Nokia5110_SetCursor(1,2);
				Nokia5110_OutString("You Lose !");
				break;
			}
      else if (winningtemp == 3){	// draw
				Delay100ms(50);
				Nokia5110_Clear();
				Nokia5110_SetCursor(1,2);
				Nokia5110_OutString("Draw !");
				break;
			}
    }	// End of if
				
    else{
			int x;
			Nokia5110_PrintBMP(0, 6,oo ,0);
	    Nokia5110_DisplayBuffer();
			x = playPosition('X');
			while(x == -1){
				Nokia5110_PrintBMP(0, 6,oo ,0);
				Nokia5110_DisplayBuffer();
				x = playPosition('X');
			}
      x = x/7;
			Nokia5110_PrintBMP((count+1)*7, 12+(7*(5-x)), xx ,0);   
   	  Nokia5110_DisplayBuffer(); 
			
			winningtemp2= winning();
			if(winningtemp2 != 0){
				if(winningtemp2 == 1){	// if player won
					Delay100ms(50);
					Nokia5110_Clear();
					Nokia5110_SetCursor(1,2);
					Nokia5110_OutString("You Win !");
					break;
				}
        else if (winningtemp2 == 2){	// if computer won
					Delay100ms(50);
					Nokia5110_Clear();
					Nokia5110_SetCursor(1,2);
					Nokia5110_OutString("You Lose !");
					break;
				}
        else if (winningtemp2 == 3){	// draw
					Delay100ms(50);
					Nokia5110_Clear();
					Nokia5110_SetCursor(1,2);
					Nokia5110_OutString("Draw !");
					break;
				}
			}
		} // End of else
	}	// End of while
}
// End of computer

/* ******************* I don't understand this For AI vs AI via UART ****************** */
void player(void){
	char r;
	char a;
	int x;
	char winningtemp;
	char winningtemp2;
	r = rand();
	UART_OutChar(r);
	while((GPIO_PORTB_DATA_R&0x00)==0);
	a = UART_InChar();
	Delay();
	if(r > a){
		while(1){ 	
			Nokia5110_PrintBMP(0, 6,oo ,0);
	    Nokia5110_DisplayBuffer();
			
			x =  playPosition('X');
			while(x == -1){
				Nokia5110_PrintBMP(0, 6,oo ,0);
	      Nokia5110_DisplayBuffer();
 				x = playPosition('X');
			}
			UART_OutChar(count+1);
      x = x/7;
			Nokia5110_PrintBMP((count+1)*7, 12+(7*(5-x)), xx ,0);   
   	  Nokia5110_DisplayBuffer(); 
      winningtemp = winning();
      if(winningtemp != 0){	// Means one of the player or the computer won OR both draw
				if(winningtemp == 1){	// if player won
					Nokia5110_SetCursor(11,3);
					Nokia5110_OutChar('2');
					break;
				}
        else if (winningtemp == 2){	// if computer won
					Nokia5110_SetCursor(11,3);
					Nokia5110_OutChar('1');
					break;
				}
        else if (winningtemp == 3){	//draw
					Nokia5110_SetCursor(11,3);  
					Nokia5110_OutChar('0');
					break;
				}
      }
			else{
				while((GPIO_PORTB_DATA_R&0x00)==0);
				a = UART_InChar();
				x = GetValue(a-1);
        while(x == -1){
					while((GPIO_PORTB_DATA_R&0x00)==0);
					a = UART_InChar();
					x = GetValue(a-1);
        }
				while(input[x-7]==' '&&x>0){
					x-=7;
				}
				input[x]='O';
				Nokia5110_PrintBMP(7*a, 12+(7*(5-x)), oo ,0);   
				Nokia5110_DisplayBuffer(); 
				winningtemp2= winning();
        if(winningtemp2 != 0){
					if(winningtemp2 == 1){	// if player won
						Nokia5110_SetCursor(8,3);
						Nokia5110_OutString("win");
						break;
					}
          else if (winningtemp2 == 2){	 // if computer won
						Nokia5110_SetCursor(8,3);
						Nokia5110_OutString("lose");
						Nokia5110_SetCursor(8,4);
						Nokia5110_OutString("er");
						break;
					}
          else if (winningtemp2 == 3){	//draw
						Nokia5110_SetCursor(11,3);  
						Nokia5110_OutChar('0');
						break;
					}
        }
			}
		}
	}
	else{
		while(1){
			while((GPIO_PORTB_DATA_R&0x00)==0);
			a = UART_InChar();
			x = GetValue(a-1);
			while(x == -1){
				while((GPIO_PORTB_DATA_R&0x00)==0)
					continue;
	      a = UART_InChar();
				x = GetValue(a-1);
      }
			while( input[x-7]==' ' && x>0 ){
				x -= 7;
			}
			input[x]='O';
			Nokia5110_PrintBMP(a*7, 12+(7*(5-x)),oo ,0);   
			Nokia5110_DisplayBuffer(); 
			winningtemp = winning();
      if(winningtemp != 0){	
				if(winningtemp == 1){	// if player won
					Nokia5110_SetCursor(11,3);
					Nokia5110_OutChar('2');
					break;
				}
        else if (winningtemp == 2){	// if computer won
					Nokia5110_SetCursor(11,3);
					Nokia5110_OutChar('1');
					break;
				}
        else if (winningtemp == 3){	//draw
					Nokia5110_SetCursor(11,3);  
					Nokia5110_OutChar('0');
					break;
				}
      }
      else{
				Nokia5110_PrintBMP(0, 6,oo ,0);
				Nokia5110_DisplayBuffer();
				x = playPosition('X');
				while(x == -1){
					Nokia5110_PrintBMP(0, 6,oo ,0);
					Nokia5110_DisplayBuffer();
					x = playPosition('X');
				}
				UART_OutChar(count+1);
        x = x/7;
				Nokia5110_PrintBMP((count+1)*7, 12+(7*(5-x)), xx ,0);   
   	    Nokia5110_DisplayBuffer(); 
        winningtemp2 = winning();
        if(winningtemp2 != 0){
					if(winningtemp2 == 1){	// if player one
						Nokia5110_SetCursor(8,3);
						Nokia5110_OutString("win");
						break;
					}
          else if (winningtemp2 == 2){  //if computer won
						Nokia5110_SetCursor(8,3);
						Nokia5110_OutString("lose");
						Nokia5110_SetCursor(8,4);
						Nokia5110_OutString("er");
						break;
					}
          else if (winningtemp2 == 3){	//draw
						Nokia5110_SetCursor(11,3);  
						Nokia5110_OutChar('0');
						break;
					}
        }
			}				 
		}
	}
}
// End of player

int playPosition(char z){	// Function that asks you to enter where you like to play
	char x;
	count = -1;
	Nokia5110_PrintBMP(0, 6,oo ,0);
	Nokia5110_DisplayBuffer();
	while(1){
		while((GPIO_PORTF_DATA_R&0x01)==1){
			int a=GPIO_PORTF_DATA_R&0x10;
			if(a == 0){
				if(count < 6){
					count++;
					Nokia5110_PrintBMP((count+1)*7, 6,oo ,0);
					Nokia5110_PrintBMP((count)*7, 6, empty ,0);
					Nokia5110_DisplayBuffer(); 
				}
				else{
					Nokia5110_PrintBMP((count+1)*7, 6, empty ,0);
					count=-1; 
					Nokia5110_PrintBMP(0, 6,oo ,0);
					Nokia5110_DisplayBuffer();
				}
				Delay();		
			} 
		}
		
		Nokia5110_PrintBMP((count+1)*7, 6, empty ,0);
		Nokia5110_PrintBMP(0, 6,oo ,0);
	  Nokia5110_DisplayBuffer(); 
		x = GetValue(count);
		if( x==-1 )
			return -1;
		while(input[x-7]==' '&&x>6){
			x-=7;
		}
		input[x]=z;
		break;
	}	// End of while(1)
	return x;
}
// End of playerPosition

// *************************** AI ***************************
int GetValue(int column){	// Pass this function a column that you want to play in and it will return its value in input array.
	int i;
	int n;
	if(column > 7)
		return -1;
	if(column == 0) {
		if(input[0]==' ')
			return 0;
		else if(input[7]==' ')
			return 7;
		else if(input[14]==' ')
			return 14;
		else if(input[21]==' ')
			return 21;
		else if(input[28]==' ')
			return 28;
		else  if(input[35]==' ')
			return 35;
		else 
			return -1;
	}
  for( i = 0 ; i<= 6 ; i++){
		if( input[column+7*i] == ' '  ){
			n = column+7*i;
      break;
		}
  }
  if(n > 42)
		return -1;
  return n;
}
// End of GetValue

// AI algorithm
int AIManager(void){
	int column;  
	float temp; 
	int PlayNumber;	
	float chance[2] = {9999999 , 0 };
	for( column = 0 ; column <7 ; column ++){
		PlayOut = 0;
    EVA = 0;
    PlayNumber = GetValue(column);
    
		if(PlayNumber != -1){
			input[PlayNumber] = 'O';
			if(winning()==2){
				input[PlayNumber]=' ';
        return PlayNumber ;
      }
      temp = -(100*NegaMax(1));
      if(PlayOut != 0)
				temp -= ((100*EVA)/PlayOut);
      if(-temp >= 100)
				provocation = 1;
			if( chance[0] > temp ){
				chance[0] = temp;
        chance[1] = PlayNumber;
       }
       input[PlayNumber] = ' ';
    }
  }
  return chance[1];
}
// End of AI algorithm

int NegaMax(int Depth){	// MiniMax algorithm in NegaMax form -- Back Tracking
	char XO;
	int column;
	int PlayNumber[8] = {0,0,0,0,0,0,0,0}; // The values of the input[] for every column
  int chance=0;
  if(Depth % 2 != 0)
		XO='X';
  else
    XO='O';
	
	for( column = 0 ; column < 7 ; column ++)
		PlayNumber[column]=GetValue(column);
	for( column = 0 ; column < 7 ; column++){
		if(PlayNumber[column] != 0){
			input[PlayNumber[column]]=XO;
			if(winning() != 0){
				PlayOut ++;
        if(XO=='O')
					EVA ++;
        else
          EVA --;
        input[PlayNumber[column]]=' ';
        return -1;
      }
      input[PlayNumber[column]]=' ';
    }
  }
  if(Depth <= 3){
		int column;
    for(column = 0 ; column < 7 ; column ++){
			int temp=0;
      if(PlayNumber[column] != 0){
				input[PlayNumber[column]]=XO;
				if(winning() != 0){
					PlayOut++;
          if(XO=='O')
						EVA++;
          else
            EVA--;
          input[PlayNumber[column]]=' ';
          return -1;
        }
        temp = NegaMax(Depth+1);
        if(column == 1)
					chance = temp;
				if(chance < temp)
					chance = temp;
				input[PlayNumber[column]]=' ';
      }
		}
	}
	return -chance;
}
// End of NegaMax
// *************************** End of AI ***************************

int winning(void){	// Winning algorithm
	int temp=0;
	int i;
  for( i = 0 ; i<= 42 ; i++){
		if(input[i] != ' '){
			int var = (int)(i/7);
      temp++;
      
			if(i - var* 7 <= 3)
        if(input[i] == input [i+1] && input[i] == input[i+2] && input[i] == input[i+3]){	// Horizontal Check
					if(input[i] == 'X')
						return 1 ;
          else
            return 2;
				}
				
      if( i <= 21 )
				if ( input[i] == input[i+7] && input[i] == input[i+14] && input[i] == input[i+21]  ){	// Vertical Check
					if(input[i] == 'X' )
						return 1 ;
          else
            return 2;
				}
      
			if( i - var* 7 <= 3 && i<=18  )
				if(input[i] == input[i+8] && input[i] == input[i+16] && input[i]==input[i+24]){	// Diagonal Check (Right)
					if(input[i] == 'X')
						return 1 ;
          else
						return 2;
				}
			if( i - var* 7 >= 3 && i <= 21   )
        if(input[i] == input[i+6] && input[i] == input[i+12] && input[i]==input[i+18]){// Diagonal Check (Left)
					if(input[i] == 'X' )
						return 1 ;
          else
            return 2;
				}
		}

	}
  if(temp == 42)	// End of the cells : There're no elements remaining for checking which means no WIN & no LOSE 
		return 3;
	return 0;
}
// End of winning algorithm

void Timer2_Init(unsigned long period){ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TimerCount = 0;
  Semaphore = 0;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}

void Timer2A_Handler(void){ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  TimerCount++;
  Semaphore = 1; // trigger
}

void Delay100ms(unsigned long count){
	unsigned long volatile time;
  while(count>0){
    time = 72724;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
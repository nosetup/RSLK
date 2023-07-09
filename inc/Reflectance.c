// Reflectance.c
// Provide functions to take measurements using the kit's built-in
// QTRX reflectance sensor array.  Pololu part number 3672. This works by outputting to the
// sensor, waiting, then reading the digital value of each of the
// eight phototransistors.  The more reflective the target surface is,
// the faster the voltage decays.
// Daniel and Jonathan Valvano
// July 11, 2019

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2019, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/

// reflectance even LED illuminate connected to P5.3
// reflectance odd LED illuminate connected to P9.2
// reflectance sensor 1 connected to P7.0 (robot's right, robot off road to left)
// reflectance sensor 2 connected to P7.1
// reflectance sensor 3 connected to P7.2
// reflectance sensor 4 connected to P7.3 center
// reflectance sensor 5 connected to P7.4 center
// reflectance sensor 6 connected to P7.5
// reflectance sensor 7 connected to P7.6
// reflectance sensor 8 connected to P7.7 (robot's left, robot off road to right)

#include <stdint.h>
#include "msp432.h"
#include "..\inc\Clock.h"

// ------------Reflectance_Init------------
// Initialize the GPIO pins associated with the QTR-8RC
// reflectance sensor.  Infrared illumination LEDs are
// initially off.
// Input: none
// Output: none
// reflectance even LED illuminate connected to P5.3
// reflectance odd LED illuminate connected to P9.2
// reflectance sensor 1 connected to P7.0 (robot's right, robot off road to left)
// reflectance sensor 2 connected to P7.1
// reflectance sensor 3 connected to P7.2
// reflectance sensor 4 connected to P7.3 center
// reflectance sensor 5 connected to P7.4 center
// reflectance sensor 6 connected to P7.5
// reflectance sensor 7 connected to P7.6
// reflectance sensor 8 connected to P7.7 (robot's left, robot off road to right)

void Reflectance_Init(void){
    // write this as part of Lab 6 (6.4.2)
    // INI INPUT
    P7->SEL0 &= ~0xFF;
    P7->SEL1 &= ~0xFF;    // 1) configure P7.0 thru 7.7 as GPIO
    P7->REN &= ~0xFF; // 3) disable pull resistors

    // INI OUTPUT
    P5->SEL0 &= ~0x08;
    P5->SEL1 &= ~0x08;    // 1) configure P5.3 as GPIO
    P5->DIR |= 0x08;      // 2) make P5.3 out
    P5->DS |= 0x08;       // 3) activate increased drive strength
    P5->OUT &= ~0x08;     //    all LEDs off

    P9->SEL0 &= ~0x04;
    P9->SEL1 &= ~0x04;    // 1) configure P9.2 as GPIO
    P9->DIR |= 0x04;      // 2) make P9.2 out
    P9->DS |= 0x04;       // 3) activate increased drive strength
    P9->OUT &= ~0x04;     //    all LEDs off


    P4->SEL0 &= ~0x01;
    P4->SEL1 &= ~0x01;    // 1) configure P4.0 as GPIO
    P4->DIR |= 0x01;      // 2) make P4.0 out
    P4->DS |= 0x01;       // 3) activate increased drive strength
    P4->OUT &= ~0x01;     //    all LEDs off

    //VISIBLE RED LED
    P1->SEL0 &= ~0x01;
    P1->SEL1 &= ~0x01;   // 1) configure P1.0 as GPIO
    P1->DIR |= 0x01;     // 2) make P1.0 out
}

// ------------Reflectance_Read------------
// Read the eight sensors
// Turn on the 8 IR LEDs
// Pulse the 8 sensors high for 10 us
// Make the sensor pins input
// wait t us
// Read sensors
// Turn off the 8 IR LEDs
// Input: time to wait in usec
// Output: sensor readings
// Assumes: Reflectance_Init() has been called
// add ^1 to invert values #define QTR0 ((*((volatile uint8_t *)(0x42098C00)))^1)
#define QTR0 ((*((volatile uint8_t *)(0x42098C00)))) // P7.0 SENSOR#0 Port 7 Input P7IN   060h
#define QTR1 ((*((volatile uint8_t *)(0x42098C04)))) // P7.1 SENSOR#1
#define QTR2 ((*((volatile uint8_t *)(0x42098C08)))) // P7.2 SENSOR#2
#define QTR3 ((*((volatile uint8_t *)(0x42098C0C)))) // P7.3 SENSOR#3
#define QTR4 ((*((volatile uint8_t *)(0x42098C10)))) // P7.4 SENSOR#4
#define QTR5 ((*((volatile uint8_t *)(0x42098C14)))) // P7.5 SENSOR#5
#define QTR6 ((*((volatile uint8_t *)(0x42098C18)))) // P7.6 SENSOR#6
#define QTR7 ((*((volatile uint8_t *)(0x42098C1C)))) // P7.7 SENSOR#7
#define SCOPEOUT (*((volatile uint8_t *)(0x42098460))) // P4.0 OUTPUT Port 4 Output P4OUT
#define CTRLODD (*((volatile uint8_t *)(0x42099048))) // P9.2 IR Port 9 Output P9OUT    082h
#define CTRLEVEN (*((volatile uint8_t *)(0x4209884C))) // P5.3 IR Port 5 Output P5OUT   042h
#define REDLED (*((volatile uint8_t *)(0x42098040))) // P1.0 REDLED

uint8_t Reflectance_Read(uint32_t time){ // TIME TO WAIT uSEC
uint8_t result = 0; // SENSOR READING
    // write this as part of Lab 6

// Read the eight sensors
//???

REDLED = 0;
// Turn on the 8 IR LEDs
CTRLODD = 1; // SET 9.2 by using bit band
// P9->OUT = (P9->OUT&~0x04)|0x04; // SET LED ENABLE
CTRLEVEN = 1; // SET 5.3 by using bit band
// P5->OUT = (P5->OUT&~0x08)|0x08; // SET LED ENABLE
SCOPEOUT = 1; // TEST POINT
  // P7->IN&0x01;


// Pulse the 8 sensors high for 10 us
//???

P7->DIR |= 0xFF;      // 2) all into output
P7->DS |= 0xFF;       // 3) activate increased drive strength
P7->OUT = (P7->OUT&~0xFF)|0xFF; // SET ALL HIGH
Clock_Delay1us(10);

// Make the sensor pins input
P7->DIR &= ~0xFF;     // 2) make P7.0 thru 7.7 input
//P7->REN |= 0xFF;     // 3) enable pull resistors
//P7->OUT |= 0x00;     //    are pull-up

// wait t us
Clock_Delay1us(time);

// if we are on center REDLED ON
if(QTR3 && QTR4){
    REDLED = 1;
}
else
{
    REDLED = 0;
}
result = result | QTR7 << 7 | QTR6 << 6 | QTR5 << 5 | QTR4 << 4 | QTR3 << 3 | QTR2 << 2 | QTR1 << 1 | QTR0;

// Turn off the 8 IR LEDs
CTRLODD = 0; // CLEAR 9.2 by using bit band
// P9->OUT = (P9->OUT&~0x04)|0x04; // SET LED ENABLE
 CTRLEVEN = 0; // CLEAR 5.3 by using bit band
// P5->OUT = (P5->OUT&~0x08)|0x08; // SET LED ENABLE
SCOPEOUT = 0; // TEST POINT
// Input: time to wait in usec
// Output: sensor readings
  return result;
}


// ------------Reflectance_Center------------
// Read the two center sensors
// Turn on the 8 IR LEDs
// Pulse the 8 sensors high for 10 us
// Make the sensor pins input
// wait t us
// Read sensors
// Turn off the 8 IR LEDs
// Input: time to wait in usec
// Output: 0 (off road), 1 off to left, 2 off to right, 3 on road
// (Left,Right) Sensors
// 1,1          both sensors   on line
// 0,1          just right     off to left
// 1,0          left left      off to right
// 0,0          neither        lost
// Assumes: Reflectance_Init() has been called
uint8_t Reflectance_Center(uint32_t time){
    // write this as part of Lab 6
  return 0; // replace this line
}


int isBitSet(int number, int bitPosition) {
    int bitMask = 1 << bitPosition;
    return (number & bitMask) != 0;
}

int isBitClear(int number, int bitPosition) {
    int bitMask = 1 << bitPosition;
    return (number & bitMask) == 0;
}

int countSetBits(int number) {
    int count = 0;

    while (number != 0) {
        count += number & 1;
        number >>= 1;
    }

    return count;
}

// Perform sensor integration
// Input: data is 8-bit result from line sensor
// Output: position in 0.1mm relative to center of line
// <some 0’s, some 1’s>, e.g., 00000111 (off to the left)
//< some 1’s, some 0’s>, e.g., 11110000 (off to the right)

int32_t Reflectance_Position(uint8_t data){
int32_t result = 0; // LOOK UP TABLE
int32_t W[] = {334, 238, 142, 48, -48, -142, -238, -334};
int i;

switch(data) {

case 0b00000000:
    result = 0;
    break;
case 0b11111111:
    result = 255;
    break;
default:
    for(i=0; i<8; i++){
        if (isBitSet(data,i) != 0)
        {
            result = result + W[i];
        }
    }
    if (countSetBits(data) >= 1){
    result = result / countSetBits(data);
    }
break;
}

 return result; // replace this line
}


// ------------Reflectance_Start------------
// Begin the process of reading the eight sensors
// Turn on the 8 IR LEDs
// Pulse the 8 sensors high for 10 us
// Make the sensor pins input
// Input: none
// Output: none
// Assumes: Reflectance_Init() has been called
void Reflectance_Start(void){
    // write this as part of Lab 10
}


// ------------Reflectance_End------------
// Finish reading the eight sensors
// Read sensors
// Turn off the 8 IR LEDs
// Input: none
// Output: sensor readings
// Assumes: Reflectance_Init() has been called
// Assumes: Reflectance_Start() was called 1 ms ago
uint8_t Reflectance_End(void){
    // write this as part of Lab 10
 return 0; // replace this line
}


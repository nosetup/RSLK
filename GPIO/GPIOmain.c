// GPIOmain.c
// Runs on MSP432
// Initialize four GPIO pins as outputs.  Continually generate output to
// drive simulated stepper motor.
// Daniel Valvano
// September 23, 2017

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

// P4.3 is an output to LED3, negative logic
// P4.2 is an output to LED2, negative logic
// P4.1 is an output to LED1, negative logic
// P4.0 is an output to LED0, negative logic


#include <stdint.h>
#include "msp.h"
#include "..\inc\Clock.h"
#include "..\inc\TExaS.h"
#define P2LED // modify to reflect hardware
// #define P4LED // original example
//#define P1Motor // modify to reflect hardware

void GPIO_Init(void){
  // initialize P4.3-P4.0 and make them outputs
  // nld unsure if old board, but P2.3-P2.0 are LED Output
  // P4.3-P4.0 are Switches
#ifdef P2LED
  P2->SEL0 &= ~0x0F;
  P2->SEL1 &= ~0x0F;            // configure stepper motor/LED pins as GPIO
  P2->DIR |= 0x0F;              // make stepper motor/LED pins out
#endif
#ifdef P4LED
  P4->SEL0 &= ~0x0F;
  P4->SEL1 &= ~0x0F;
  P4->DIR |= 0x0F;
#endif

#ifdef P1Motor
  // configure stepper motor PWM  P2.6 PWMR, P2.7 PWML
  P2->SEL0 &= ~0xC0;
  P2->SEL1 &= ~0xC0;
  P2->DIR |= 0xC0;

  // configure stepper motor PWM  P3.6 NSLEEPR , P3.7 NSLEEPL
  P3->SEL0 &= ~0xC0;
  P3->SEL1 &= ~0xC0;
  P3->DIR |= 0xC0;


  // configure stepper motor DIR  P5.5 DIRR , P5.4 DIRL
  P5->SEL0 &= ~0x30;
  P5->SEL1 &= ~0x30;
  P5->DIR |= 0x30;

#endif

}

int main(void){
  Clock_Init48MHz();
  GPIO_Init();
  TExaS_Init(LOGICANALYZER_P4);
  while(1){

#ifdef P2LED
      P2->OUT = (P2->OUT&~0x0F)|10; // 1010, LED is 0101
      Clock_Delay1ms(100);
      P2->OUT = (P2->OUT&~0x0F)|9;  // 1001, LED is 0110
      Clock_Delay1ms(100);
      P2->OUT = (P2->OUT&~0x0F)|5;  // 0101, LED is 1010
      Clock_Delay1ms(100);
      P2->OUT = (P2->OUT&~0x0F)|6;  // 0110, LED is 1001
      Clock_Delay1ms(100);
#endif

#ifdef P1Motor // P2 = MOTOR ENABLE, P3 = N_SLEEP, P5 = MOTOR DIR

// GO FORWARD BOTH
    P5->OUT = (P5->OUT&~0x30)|0x00; // SET BOTH MOTOR DIR FORWARD
    Clock_Delay1ms(100);
    P2->OUT = (P2->OUT&~0xC0)|0xC0; // SET MOTOR ENABLE
    Clock_Delay1ms(100);
    P3->OUT = (P3->OUT&~0xC0)|0xC0;  // NO SLEEP
    Clock_Delay1ms(100);
// COAST
    P3->OUT = (P3->OUT&~0xC0)|0x00;  // SLEEP
    Clock_Delay1ms(5000);

    // set left wheel forward
    P2->OUT = (P2->OUT&~0x80)|0x80; // SET MOTOR ENABLE
    Clock_Delay1ms(100);
    P3->OUT = (P3->OUT&~0x80)|0x80;  // NO SLEEP
    Clock_Delay1ms(500);
    P3->OUT = (P3->OUT&~0x80)|0x00;  // SLEEP

    // set right wheel, forward
    P2->OUT = (P2->OUT&~0x40)|0x40; // SET MOTOR ENABLE
    Clock_Delay1ms(100);
    P3->OUT = (P3->OUT&~0x40)|0x40;  // NO SLEEP
    Clock_Delay1ms(500);
    P3->OUT = (P3->OUT&~0x40)|0x40;  // SLEEP


// BRAKE
    P2->OUT = (P2->OUT&~0xC0)|0x00;
    Clock_Delay1ms(100);
    P3->OUT = (P3->OUT&~0xC0)|0xC0;  // NO SLEEP
    Clock_Delay1ms(100);

    // set left wheel rev
    P5->OUT = (P5->OUT&~0x10)|0x10; // SET MOTOR REV
    Clock_Delay1ms(100);
    P2->OUT = (P2->OUT&~0x80)|0x80; // SET MOTOR ENABLE
    Clock_Delay1ms(100);
    P3->OUT = (P3->OUT&~0x80)|0x80;  // NO SLEEP
    Clock_Delay1ms(500);
    P3->OUT = (P3->OUT&~0x80)|0x00;  // SLEEP
    Clock_Delay1ms(100);
    P5->OUT = (P5->OUT&~0x10)|0x00; // SET MOTOR FWD

    // set right wheel, rev
    P5->OUT = (P5->OUT&~0x20)|0x20; // SET MOTOR REV
    Clock_Delay1ms(100);
    P2->OUT = (P2->OUT&~0x40)|0x40; // SET MOTOR ENABLE
    Clock_Delay1ms(100);
    P3->OUT = (P3->OUT&~0x40)|0x40;  // NO SLEEP
    Clock_Delay1ms(500);
    P3->OUT = (P3->OUT&~0x40)|0x40;  // SLEEP
    Clock_Delay1ms(100);
    P5->OUT = (P5->OUT&~0x20)|0x00; // SET MOTOR FWD

    // BRAKE
        P2->OUT = (P2->OUT&~0xC0)|0x00; // SET MOTOR DISABLE
        Clock_Delay1ms(100);
        P3->OUT = (P3->OUT&~0xC0)|0xC0;  // NO SLEEP
        Clock_Delay1ms(100);
        // configure stepper motor DIR  P5.5 DIRR , P5.4 DIRL


    Clock_Delay1ms(2000);



#endif
  }
}

// Program 2.13 from Volume 2

//#define STEPPER  (*((volatile uint8_t *)0x40004C23))  /* Port 4 Output, bits 3-0 are stepper motor */
#ifdef P2LED
#define STEPPER  (*((volatile uint8_t *)0x40004C03))  /* Port 2 Output, bits 3-0 are LED Output */
#endif
#ifdef P1Motor // P2.6 PWMR, P2.7 PWML, P3.6 NSLPR , P3.7 NSLPL
#define STEPPER  (*((volatile uint8_t *)0x40004C23))
#endif
static void step(uint8_t n){
  STEPPER = (STEPPER&~0x0F)|n;  // output to stepper causing it to step once
  Clock_Delay1ms(100);          // 100ms delay 10 steps/sec
}
int main2(void){ // reset clears P4REN, P4DS, P4SEL0, P4SEL1
  Clock_Init48MHz();
  TExaS_Init(LOGICANALYZER_P4);
  GPIO_Init();
  while(1){
    step(5);  // motor is 0101
    step(6);  // motor is 0110
    step(10); // motor is 1010
    step(9);  // motor is 1001
  }
}

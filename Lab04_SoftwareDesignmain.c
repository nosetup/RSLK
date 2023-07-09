// Lab04_SoftwareDesignmain.c
// Runs on MSP432
// Solution to Software Design lab
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
#include <stdint.h>
#include <stdio.h>
#define DEBUG  // Uncomment this line to enable debug mode nld
// #include "Solution.h" // Use this too call solution

// Power up Launchpad with uUSB. Add +5V shunt between debugger and msp432
// Remove Launchpad from RSLK Robot nld.

enum scenario {
    Error = 0,
    LeftTooClose = 1,
    RightTooClose = 2,
    LeftRightTooClose = 3, // added NLD
    CenterTooClose = 4,
    LeftCenterTooClose = 5,  // added NLD
    RightCenterTooClose = 6,  // added NLD
    LeftRightCenterTooClose = 7,  // added NLD
    Straight = 8,
    LeftTurn = 9,
    RightTurn = 10,
    TeeJoint = 11,
    LeftJoint = 12,
    RightJoint = 13,
    CrossRoad = 14,
    Blocked = 15,
    Unknown = 16,
};
typedef enum scenario scenario_t;

#define SIDEMAX 354    // largest side distance to wall in mm
#define SIDEMIN 212    // smallest side distance to wall in mm

#define CENTEROPEN 600 // distance to wall between open/blocked
#define CENTERMIN 150  // min distance to wall in the front

#define SENSORMAX 800  // max appliciable distance (mm) for sensor
#define SENSORMIN 50  //  min appliciable distance (mm) for sensor


int32_t TestSensorVal(int32_t n){
    int32_t TestSensor = n;
            if ( n < SENSORMIN )
            {
                TestSensor = (int)Error;
            }
            if ( n > SENSORMAX )
            {
                TestSensor = (int)Error;
            }
    return TestSensor;
}


scenario_t Classify(int32_t Left, int32_t Center, int32_t Right){
    int PS_State = (int)Unknown;
    scenario_t result= Unknown;

    // write this code

    // check for "errors" invalid sensor values nld
  if (TestSensorVal(Left) == 0)
  {
      PS_State = (int)Error;
  }
  if (TestSensorVal(Center) == 0)
  {
      PS_State = (int)Error;
  }
  if (TestSensorVal(Right) == 0)
  {
      PS_State = (int)Error;
  }

  if (PS_State !=  Error)
  {
      PS_State = (int)Error;
      //check for "too close" warnings
      if (Left < SIDEMIN)
      {
          PS_State = PS_State | (int)LeftTooClose;
      }
      if (Right < SIDEMIN)
      {
          PS_State = PS_State | (int)RightTooClose;
      }
      if (Center < CENTERMIN)
      {
          PS_State = PS_State | (int)CenterTooClose;
      }

      if (Center > CENTERMIN && Left >= SIDEMIN && Right >= SIDEMIN)
      {
          PS_State = (int)Straight;
      }

      if (Left >= SIDEMAX && CENTERMIN <= Center && Center < CENTEROPEN && Right >= SIDEMIN)
      {
          PS_State = (int)LeftTurn;
      }


      if (Right >= SIDEMAX && CENTERMIN <= Center && Center < CENTEROPEN && Left >= SIDEMIN)
      {
          PS_State = (int)RightTurn;
      }

      if (CENTERMIN <= Center && Center < CENTEROPEN && Right >= SIDEMAX && Left >= SIDEMAX)
      {
          PS_State = (int)TeeJoint;
      }
      if (Left >= SIDEMAX && Center >= CENTEROPEN && Right >= SIDEMIN)
      {
          PS_State = (int)LeftJoint;
      }

      if (Right >= SIDEMAX && Center >= CENTEROPEN && Left >= SIDEMIN)
      {
          PS_State = (int)RightJoint;
      }

      if (Center >= CENTEROPEN && Right >= SIDEMAX && Left >= SIDEMAX)
      {
          PS_State = (int)CrossRoad;
      }


      if (SIDEMIN <= Left && Left < SIDEMAX && SIDEMIN <= Right && Right < SIDEMAX && CENTERMIN <= Center && Center < CENTEROPEN)
      {
          PS_State = (int)Blocked;
      }

  }

// lets see where we at
      switch (PS_State) {
           case (int)Error:
               result = Error;
               break;
           case (int)LeftTooClose:
               result = LeftTooClose;
               break;
           case (int)RightTooClose:
               result = RightTooClose;
               break;
           case (int)LeftRightTooClose:
               result = LeftRightTooClose;
               break;
           case (int)CenterTooClose:
               result = CenterTooClose;
               break;
           case (int)LeftCenterTooClose:
               result = LeftCenterTooClose;
               break;
           case (int)RightCenterTooClose:
               result = RightCenterTooClose;
               break;
           case (int)LeftRightCenterTooClose:
               result = LeftRightCenterTooClose;
               break;
           case (int)Straight:
               result = Straight;
               break;
           case (int)LeftTurn:
               result = LeftTurn;
               break;
           case (int)RightTurn:
               result = RightTurn;
               break;
           case (int)TeeJoint:
               result = TeeJoint;
               break;
           case (int)LeftJoint:
               result = LeftJoint;
               break;
           case (int)RightJoint:
               result = RightJoint;
               break;
           case (int)CrossRoad:
               result = CrossRoad;
               break;
           case (int)Blocked:
               result = Blocked;
               break;
           default:
               printf("Unknown scenario.\n");
               result = Unknown;
               break;
       }


  return result;
}

#define IRSlope 1195172
#define IROffset -1058
#define IRMax 2552
#define ADCMax 120000
#define DistanceMax 800
#define DistanceMin 10

int32_t Convert(int32_t n){
    // write this code
    int32_t distancemm = 0, IROffsetd = 0;

    if (n > ADCMax)
    {
        distancemm = DistanceMin;
    }
    else
    {
        if( n < IRMax)
        {
            distancemm = DistanceMax;
        }
        else
        {
        IROffsetd = n + IROffset;
            if (IROffsetd <= 0)
            {
                // invalid value present nld
                // set distancemm = 0;
            }
            else
            {
                distancemm = IRSlope / IROffsetd;

            }
        }
    }
  return distancemm; // replace this line
}
// ***********testing of Convert*********
int32_t const ADCBuffer[16]={2000, 2733, 3466, 4199, 4932, 5665, 6398, 7131, 7864, 8597, 9330, 10063, 10796, 11529, 12262, 12995};
int32_t const DistanceBuffer[16]={800, 713, 496, 380, 308, 259, 223, 196, 175, 158, 144, 132, 122, 114, 106, 100};
void Program4_1(void){
  int i;
  int32_t adc,distance,errors,diff;
  errors = 0;


  for(i=0; i<16; i++){
    adc = ADCBuffer[i];
    distance = Convert(adc); // call to your function
    diff = distance-DistanceBuffer[i]; // determine if error present by comparing to Answer array
    if((diff<-1)||(diff>1)){
      errors++;

#ifdef DEBUG
      printf("Error in index %d\n", i);
#endif
    }

#ifdef DEBUG
    //print results
    // note this function print the integer number using the %d format specifier
    //printf("Index is: %d\n", i);
    //printf("ADC is: %d\n", adc);
    //printf("Distance in mm is: %d\n", distance);
    //printf("Error Present in this test : %d\n", diff);
#endif
  }

#ifdef DEBUG
  printf("Total Errors Present: %d\n", errors);
#endif

  while(1){};
}
// ***********end of testing of Convert*********



// ***********testing of classify
// create a driver for sensor SHARP GP2Y0A21YK0F
//
scenario_t Solution(int32_t Left, int32_t Center, int32_t Right);
int32_t const CornerCases[18]={49,50,51,149,150,151,211,212,213,353,354,355,599,600,601,799,800,801};
#ifdef DEBUG
int32_t testcounter;
int32_t errors;
int32_t corrects;
int32_t state0counter;
int32_t state1counter;
int32_t state2counter;
int32_t state3counter;
int32_t state4counter;
int32_t state5counter;
int32_t state6counter;
int32_t state7counter;
int32_t state8counter;
int32_t state9counter;
int32_t state10counter;
int32_t state11counter;
int32_t state12counter;
int32_t state13counter;
int32_t state14counter;
int32_t state15counter;
#endif
void Program4_2(void){
  enum scenario result,truth;
  int i,j,k, errors;
  int32_t left, right, center; // sensor readings
  errors = 0;
  for(i=0; i<18; i++){

    left = CornerCases[i];
    for(j=0; j<18; j++){
      center = CornerCases[j];
      for(k=0; k<18; k++){
        right = CornerCases[k];
        result = Classify(left,center,right); // your solution in mm
        truth = Solution(left,center,right);  // correct answer in mm
#ifdef DEBUG
        testcounter++;
#endif

        if(result != truth){
           errors++; // error counter
#ifdef DEBUG
           if (result == 0)
           {
               state0counter++;
           }

           if (result == 1)
           {
               state1counter++;
           }
           if (result == 2)
           {
               state2counter++;
           }
           if (result == 3)
           {
               state3counter++;
           }
           if (result == 4)
           {
               state4counter++;
           }
           if (result == 5)
           {
               state5counter++;
           }
           if (result == 6)
           {
               state6counter++;
           }
           if (result == 7)
           {
               state7counter++;
           }
           if (result == 8)
           {
               state8counter++;
           }
           if (result == 9)
           {
               state9counter++;
           }
           if (result == 10)
           {
               state10counter++;
           }
           if (result == 11)
           {
               state11counter++;
           }
           if (result == 12)
           {
               state12counter++;
           }
           if (result == 13)
           {
               state13counter++;
           }
           if (result == 14)
           {
               state14counter++;
           }
           if (result == 15)
           {
               state15counter++;
           }
           printf("Left Value: %d\n", left);
           printf("Right Value: %d\n", right);
           printf("Center Value: %d\n", center);
           printf("Evaluation Value is: %d\n", result);
           printf("Correct Solution is: %d\n", truth);
#endif
        } else // this is what i got right
        {
#ifdef DEBUG
            corrects++;
            //printf("Index i is: %d\n", i); // Print index Value
            //printf("Index j is: %d\n", j); // Print index Value
            //printf("Index k is: %d\n", k); // Print index Value
            //printf("Evaluation Value is: %d\n", result);
            //printf("Correct Solution is: %d\n", truth);
#endif
        }
      }
    }
  }
#ifdef DEBUG
  printf("Total Test Counter: %d\n", testcounter);
  printf("Total Corrects Counter: %d\n", corrects);
  printf("Total Errors Counter: %d\n", errors);
  printf("Total Zero Counter: %d\n", state0counter);
  printf("Total 1 Counter: %d\n", state1counter);
  printf("Total 2 Counter: %d\n", state2counter);
  printf("Total 3 Counter: %d\n", state3counter);
  printf("Total 4 Counter: %d\n", state4counter);
  printf("Total 5 Counter: %d\n", state5counter);
  printf("Total 6 Counter: %d\n", state6counter);
  printf("Total 7 Counter: %d\n", state7counter);
  printf("Total 8 Counter: %d\n", state8counter);
  printf("Total 9 Counter: %d\n", state9counter);
  printf("Total 10 Counter: %d\n", state10counter);
  printf("Total 11 Counter: %d\n", state11counter);
  printf("Total 12 Counter: %d\n", state12counter);
  printf("Total 13 Counter: %d\n", state13counter);
  printf("Total 14 Counter: %d\n", state14counter);
  printf("Total 15 Counter: %d\n", state15counter);
#endif
  while(1){
  }
}

void Program4_3(void){ // will take over 16 hours to complete
  enum scenario result,truth;
  int32_t left, right, center, errors; // sensor readings
  errors = 0;
  printf("Starting Program4_3, this test will take over 16 hours to complete \n");
  printf("if debug is enabled, it will take over 24 hours to complete \n"); // warning notice nld
  for(left=0; left<1000; left++){
    for(center=0; center<1000;  center++){
      for(right=0; right<1000; right++){
        result = Classify(left,center,right); // your solution
        truth = Solution(left,center,right);  // correct answer
        if(result != truth){
           errors++;
#ifdef DEBUG
           if (result == 0)
           {
               state0counter++;
           }

           if (result == 1)
           {
               state1counter++;
           }
           if (result == 2)
           {
               state2counter++;
           }
           if (result == 3)
           {
               state3counter++;
           }
           if (result == 4)
           {
               state4counter++;
           }
           if (result == 5)
           {
               state5counter++;
           }
           if (result == 6)
           {
               state6counter++;
           }
           if (result == 7)
           {
               state7counter++;
           }
           if (result == 8)
           {
               state8counter++;
           }
           if (result == 9)
           {
               state9counter++;
           }
           if (result == 10)
           {
               state10counter++;
           }
           if (result == 11)
           {
               state11counter++;
           }
           if (result == 12)
           {
               state12counter++;
           }
           if (result == 13)
           {
               state13counter++;
           }
           if (result == 14)
           {
               state14counter++;
           }
           if (result == 15)
           {
               state15counter++;
           }
           printf("Left Value: %d\n", left);
           printf("Right Value: %d\n", right);
           printf("Center Value: %d\n", center);
           printf("Evaluation Value is: %d\n", result);
           printf("Correct Solution is: %d\n", truth);
#endif
        }
      }
    }
  }
#ifdef DEBUG
  printf("Total Test Counter: %d\n", testcounter);
  printf("Total Corrects Counter: %d\n", corrects);
  printf("Total Errors Counter: %d\n", errors);
  printf("Total Zero Counter: %d\n", state0counter);
  printf("Total 1 Counter: %d\n", state1counter);
  printf("Total 2 Counter: %d\n", state2counter);
  printf("Total 3 Counter: %d\n", state3counter);
  printf("Total 4 Counter: %d\n", state4counter);
  printf("Total 5 Counter: %d\n", state5counter);
  printf("Total 6 Counter: %d\n", state6counter);
  printf("Total 7 Counter: %d\n", state7counter);
  printf("Total 8 Counter: %d\n", state8counter);
  printf("Total 9 Counter: %d\n", state9counter);
  printf("Total 10 Counter: %d\n", state10counter);
  printf("Total 11 Counter: %d\n", state11counter);
  printf("Total 12 Counter: %d\n", state12counter);
  printf("Total 13 Counter: %d\n", state13counter);
  printf("Total 14 Counter: %d\n", state14counter);
  printf("Total 15 Counter: %d\n", state15counter);
#endif
  while(1){
  }
}


void main(void){
// run one of these nld
//  Program4_1();
// Program4_2();
  Program4_3();
}

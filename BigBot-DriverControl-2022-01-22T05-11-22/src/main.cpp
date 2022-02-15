/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\binar                                            */
/*    Created:      Thu Jan 20 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>

using namespace vex;

#define DDRIVE_MIN -100 //The minimum value x or y can be.
#define DDRIVE_MAX 100  //The maximum value x or y can be.
#define MOTOR_MIN_PWM -100 //The minimum value the motor output can be.
#define MOTOR_MAX_PWM 100 //The maximum value the motor output can be.


/////////////////////// Connection Defenitions ////////////////////////////
//       ---- MOTORS ----
//     L1DRIVETRAINMOT   PORT1
//     L2DRIVETRAINMOT   PORT2
//     R1DRIVETRAINMOT   PORT3
//     R2DRIVETRAINMOT   PORT4
//     SCOOPMOT          PORT5
//     INTAKEMOT         PORT6
//     SWEEPERMOT        PORT7
//    ---- CONTROLLER ----
//#define  DRIVEAXIS         cont.Axis2
//#define  TURNAXIS          cont.Axis1
#define  RIGHTSIDE         cont.Axis2
#define  LEFTSIDE          cont.Axis3
//#define  SCOOPAXIS         cont.Axis3
//#define  SWEEPERAXIS       cont.Axis4
#define  TOGGLEINTAKE      cont.ButtonX
#define  BREAKMODETOGGLE   cont.ButtonY
#define  TURN180DEG        cont.ButtonA
#define  SLIDERACTUATE     cont.ButtonB
#define  FINEMOVEFWD       cont.ButtonUp
#define  FINEMOVEBACK      cont.ButtonDown
#define  FINEMOVELEFT      cont.ButtonLeft
#define  FINEMOVERIGHT     cont.ButtonRight
#define  UNUSED1           cont.ButtonL1
#define  UNUSED2           cont.ButtonL2
#define  UNUSED3           cont.ButtonR1
#define  UNUSED4           cont.ButtonR2


/////////////////////// Define Variables ////////////////////////////

bool intakeState = false;
bool allowMotion = true;

float map(float input, float instart, float instop, float outstart, float outstop) {
  return outstart + (outstop - outstart) * ((input - instart) / (instop - instart));
}

// Driver Control Event Callback
//
//  When Driver Is Active:
//  - Start Driver Event Loop
//  - Change Screen
void beginDriver() {
  //mot_scoopMotor.resetRotation();
  allowMotion = true;
  cont.Screen.clearScreen();
  cont.Screen.setCursor(0, 0);
  cont.Screen.print("Driver Active.");
}
/*
void toggleIntake() {  // Turn On/Off Ring Intake
  if (intakeState) {
    mot_intakeMotor.stop(coast);
    return;
  } 
  mot_intakeMotor.spin(forward, 100, pct);
}
*/
void TankDrive(float xAxis, float yAxis){
  // Major ispiration from: https://home.kendra.com/mauser/joystick.html
  // Massive thanks to kendra.com
  // 
  // Procedure:
  //   Invert X
  //   Calculate R+L (Call it V): V =(100-ABS(X)) * (Y/100) + Y
  //   Calculate R-L (Call it W): W= (100-ABS(Y)) * (X/100) + X
  //   Calculate R: R = (V+W) /2
  //   Calculate L: L= (V-W)/2
  //   Do any scaling on R and L your hardware may require.
  //   Send those values to your Robot.
  //   Repeat
  //single stick drive
  /*int X = xAxis;
  int Y = yAxis;
  int V = (100 - abs(X)) * (Y/100) + Y;
  int W = (100 - abs(Y)) * (X/100) + X;
  int leftMotSpeed = (V + W)/2;
  int rightMotSpeed = (V - W)/2;
  mot_dtLeftFront.spin(reverse, leftMotSpeed, pct);
  mot_dtLeftBack.spin(reverse, leftMotSpeed, pct);
  mot_dtRightFront.spin(forward, rightMotSpeed, pct);
  mot_dtRightBack.spin(forward, rightMotSpeed, pct);
  */

  //basic tank drive
  mot_dtRightFront.spin(forward, xAxis, pct);
  mot_dtRightBack.spin(forward, xAxis, pct);
  mot_dtLeftFront.spin(forward, yAxis, pct);
  mot_dtLeftBack.spin(forward, yAxis, pct);
}
/*
void ScoopDrive(int pos) {
  mot_scoopMotor.spinTo(map(pos, -100, 100, 0, 80), deg, 50, velocityUnits::pct);
}
*/
void eventLoop() {
  while(allowMotion == true) {
    TankDrive(cont.Axis3.position(), cont.Axis2.position());

    //simple claw controls
    if(cont.ButtonA.pressing()){
      mot_frontClaw.spin(forward, 100, pct);
    }else if(cont.ButtonB.pressing()){
      mot_frontClaw.spin(reverse, 100, pct);
    }else{
      mot_frontClaw.spin(forward, 0, pct);
    }
    //simple button controls
    if(cont.ButtonL1.pressing()){
      mot_arm.spin(forward, 100, pct);
    }else if(cont.ButtonR1.pressing()){
      mot_arm.spin(reverse, 100, pct);
    }else{
      mot_arm.spin(forward, 0, pct);
    }

  }
}

/////////////////////// MAIN FUNTIONS ////////////////////////////

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  comp.drivercontrol(beginDriver);
 // TOGGLEINTAKE.pressed(toggleIntake);        // Handle (  X   Button) Toggle Intake Button Pressed   
  // BREAKMODETOGGLE.pressed(toggleBreakMode);  // Handle (  Y   Button) Toggle Break Mode Button Pressed 
  // TURN180DEG.pressed(turn180Deg);            // Handle (  A   Button) Toggle Intake Pressed      
  // SLIDERACTUATE.pressed(doSliderMotion);     // Handle (  B   Button) Toggle Intake Pressed   

  // Begin Program Logic
  cont.Screen.clearScreen();
  cont.Screen.setCursor(0, 0);
  cont.Screen.print("Auto.");
  eventLoop();
}
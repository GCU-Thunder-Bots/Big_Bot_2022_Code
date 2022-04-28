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
using signature = vision::signature;
using code = vision::code;

#define DDRIVE_MIN -100 //The minimum value x or y can be.
#define DDRIVE_MAX 100  //The maximum value x or y can be.
#define MOTOR_MIN_PWM -100 //The minimum value the motor output can be.
#define MOTOR_MAX_PWM 100 //The maximum value the motor output can be.

//BigBot-DriverControl-2022-01-22T05-11-22
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
#define  degrees           rotationUnits::deg
#define  pct               velocityUnits::pct

/////////////////////// Define Variables ////////////////////////////

bool intakeState = false;
bool allowMotion = true;
bool allowDriverOperated = false;
bool clawOpen = true;
bool backClawOpen = true;
float speedControls = 2;

float map(float input, float instart, float instop, float outstart, float outstop) {
  return outstart + (outstop - outstart) * ((input - instart) / (instop - instart));
}






// Driver Control Event Callback
//
//  When Driver Is Active:
//  - Start Driver Event Loop
//  - Change Screen

void Init(){
  cont.Screen.clearScreen();
  cont.Screen.setCursor(0, 0);
  cont.Screen.print("Initing.");
  //reset wheel rotations 
  mot_dtRightFront.resetRotation();
  mot_dtRightBack.resetRotation();
  mot_dtLeftFront.resetRotation();
  mot_dtLeftBack.resetRotation();

}
void beginDriver() {
  allowMotion = true;
  cont.Screen.clearScreen();
  cont.Screen.setCursor(0, 0);
  cont.Screen.print("Driver Active.");
  cont.Screen.setCursor(1,0);
}

void driveWheels(float deg, float speed, bool waitToFinish){
  if(waitToFinish){
    mot_dtRightFront.resetRotation();
    mot_dtRightBack.resetRotation();
    mot_dtLeftFront.resetRotation();
    mot_dtLeftBack.resetRotation();
    mot_dtRightFront.spinTo(deg, degrees, speed, pct, false);
    mot_dtRightBack.spinTo(deg, degrees, speed, pct, false);
    mot_dtLeftFront.spinTo(deg, degrees, speed, pct, false);
    mot_dtLeftBack.spinTo(deg, degrees, speed, pct, true);
  }else{
    mot_dtRightFront.resetRotation();
    mot_dtRightBack.resetRotation();
    mot_dtLeftFront.resetRotation();
    mot_dtLeftBack.resetRotation();
    mot_dtRightFront.spinTo(deg, degrees, speed, pct, false);
    mot_dtRightBack.spinTo(deg, degrees, speed, pct, false);
    mot_dtLeftFront.spinTo(deg, degrees, speed, pct, false);
    mot_dtLeftBack.spinTo(deg, degrees, speed, pct, false);
  }
}

void turnWheels(float deg, float speed, bool isLeft, bool waitToFinish){
  if(waitToFinish){
    if(isLeft){
    mot_dtRightFront.resetRotation();
    mot_dtRightBack.resetRotation();
    mot_dtLeftFront.resetRotation();
    mot_dtLeftBack.resetRotation();
    mot_dtRightFront.spinTo(-deg, degrees, -speed, pct, false);
    mot_dtRightBack.spinTo(-deg, degrees, -speed, pct, false);
    mot_dtLeftFront.spinTo(deg, degrees, speed, pct, false);
    mot_dtLeftBack.spinTo(deg, degrees, speed, pct, true);
    }else{
      mot_dtRightFront.resetRotation();
      mot_dtRightBack.resetRotation();
      mot_dtLeftFront.resetRotation();
      mot_dtLeftBack.resetRotation();
      mot_dtRightFront.spinTo(deg, degrees, speed, pct, false);
      mot_dtRightBack.spinTo(deg, degrees, speed, pct, false);
      mot_dtLeftFront.spinTo(-deg, degrees, -speed, pct, false);
      mot_dtLeftBack.spinTo(-deg, degrees, -speed, pct, true);
    }
  }else{
    if(isLeft){
    mot_dtRightFront.resetRotation();
    mot_dtRightBack.resetRotation();
    mot_dtLeftFront.resetRotation();
    mot_dtLeftBack.resetRotation();
    mot_dtRightFront.spinTo(-deg, degrees, -speed, pct, false);
    mot_dtRightBack.spinTo(-deg, degrees, -speed, pct, false);
    mot_dtLeftFront.spinTo(deg, degrees, speed, pct, false);
    mot_dtLeftBack.spinTo(deg, degrees, speed, pct, true);
    }else{
      mot_dtRightFront.resetRotation();
      mot_dtRightBack.resetRotation();
      mot_dtLeftFront.resetRotation();
      mot_dtLeftBack.resetRotation();
      mot_dtRightFront.spinTo(deg, degrees, speed, pct, false);
      mot_dtRightBack.spinTo(deg, degrees, speed, pct, false);
      mot_dtLeftFront.spinTo(-deg, degrees, -speed, pct, false);
      mot_dtLeftBack.spinTo(-deg, degrees, -speed, pct, true);
    }
  }
}

void programmingSkills(){
  /*driveWheels(100, 100, false);
    moveArm(40, 25, false);
  driveWheels(6500, 100, true);
  mot_frontClaw.spin(reverse, 100, pct);
  wait(1, timeUnits::sec);
  driveWheels(-5400, -100, true);
  mot_frontClaw.spin(forward, 100, pct);
  wait(1, timeUnits::sec);
  driveWheels(-500, -100, true);*/



  //drive to blue mobile goal
  driveWheels(1500, 100, true);
  mot_frontClaw.spin(reverse, 100, pct);
  wait(1, timeUnits::sec);
  driveWheels(-340, -70, true);
  turnWheels(940, 100, true, true);
  //score blue goal and neutral goal
  //square up on wall
  driveWheels(5800, 100, true);
  //release blue goal
  mot_frontClaw.spin(forward, 100, pct);
  wait(2, timeUnits::sec);
  mot_frontClaw.stop();

  //return to start wall
  driveWheels(-6500, -100, true);
  //avoid balance board
  driveWheels(1500, 100, true);
  //turn to large goal
  turnWheels(450, 100, false, true);

  //score large goal
  driveWheels(5000, 100, true);
  
  //go score red goal
  /*
  driveWheels(-1000, -100, true);
  turnWheels(1400, 100, false, true);
  driveWheels(4000, 100, true);

  mot_frontClaw.spin(forward, 100, pct);
  wait(1, timeUnits::sec);
  driveWheels(-5000, 100, true);
  */







}
void autonomous(){
  cont.Screen.clearScreen();
  cont.Screen.setCursor(0, 0);
  cont.Screen.print("Auto Active.");
  
  //drive forward small amount
  mot_dtRightFront.resetRotation();
  mot_dtRightBack.resetRotation();
  mot_dtLeftFront.resetRotation();
  mot_dtLeftBack.resetRotation();
  

  //AUTO CODE HERE

  //Drive to alliance mobile goal

  //Open claw to drop rings in alliance mobile goal

  //Back up small amount 

  //Turn 90 degrees towards neutral goals

  //Go forward to small neutral goal

  //Grab small neutral goal with front/back claw 

  //Turn 90 degrees towards large neutral goal

  //Drive forward to large neutral goal

  //Grab large neutral goal

  //Turn 90 degrees towards team zone

  //Drive into team zone 









  allowDriverOperated = true;
}

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
  mot_dtRightFront.spin(forward, xAxis/speedControls, pct);
  mot_dtRightBack.spin(forward, xAxis/speedControls, pct);
  mot_dtLeftFront.spin(forward, yAxis/speedControls, pct);
  mot_dtLeftBack.spin(forward, yAxis/speedControls, pct);
}

void eventLoop() {
  while(allowMotion == true && allowDriverOperated) {
    TankDrive(cont.Axis3.position(), cont.Axis2.position());

    //simple claw controls
    if(cont.ButtonA.pressing()){
      mot_frontClaw.spin(forward, 100, pct);
      clawOpen =true;
    }else if(cont.ButtonB.pressing()){
      mot_frontClaw.spin(reverse, 100, pct);
      clawOpen = false;
    }else{
      if(clawOpen){
        mot_frontClaw.spin(forward, 0, pct);
      }
    }


    if(cont.ButtonX.pressing()){
      mot_backClaw.spin(forward, 100, pct);
      backClawOpen = true;
    }else if(cont.ButtonY.pressing()){
      mot_backClaw.spin(reverse, 100, pct);
      backClawOpen = false;
    }else{
      if(backClawOpen){
        mot_backClaw.spin(forward, 0, pct);
      }
    }
  }
}




/////////////////////// MAIN FUNTIONS ////////////////////////////

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  Init();
  //programmingSkills();
  //comp.autonomous(autonomous);
  //wait for 45 seconds for auto to complete
  //wait(45, timeUnits::sec);
  //wait for auto completion
  //autonomous();
  beginDriver();
  allowDriverOperated = true;
  eventLoop();

 // TOGGLEINTAKE.pressed(toggleIntake);        // Handle (  X   Button) Toggle Intake Button Pressed   
  // BREAKMODETOGGLE.pressed(toggleBreakMode);  // Handle (  Y   Button) Toggle Break Mode Button Pressed 
  // TURN180DEG.pressed(turn180Deg);            // Handle (  A   Button) Toggle Intake Pressed      
  // SLIDERACTUATE.pressed(doSliderMotion);     // Handle (  B   Button) Toggle Intake Pressed   

  // Begin Program Logic
  /*cont.Screen.clearScreen();
  cont.Screen.setCursor(0, 0);
  cont.Screen.print("Auto.");*/
  //begins driving procedure
  //comp.drivercontrol(eventLoop);
  while(1){
    wait(1, timeUnits::sec);
  }
}
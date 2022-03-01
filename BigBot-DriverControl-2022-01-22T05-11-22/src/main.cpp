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
#define  degrees           rotationUnits::deg
#define  pct               velocityUnits::pct

/////////////////////// Define Variables ////////////////////////////

bool intakeState = false;
bool allowMotion = true;
bool allowDriverOperated = false;
float speedControls = 2;

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
  cont.Screen.setCursor(1,0);
}

void autonomous(){
  cont.Screen.clearScreen();
  cont.Screen.setCursor(0, 0);
  cont.Screen.print("Auto Active.");
  
  //set arm position neutral
  mot_arm.spinTo(40, degrees, 25, pct, true);
  ring_cup.spinTo(30, degrees, 25, pct, true);

  mot_arm.startSpinTo(500, degrees, 75, pct);
  while(mot_arm.isSpinning()){
    ring_cup.spinTo(ring_cup.rotation(degrees)+10.5, degrees, 75, pct, true);
  }

  //drive forward small amount
  mot_dtRightFront.resetRotation();
  mot_dtRightBack.resetRotation();
  mot_dtLeftFront.resetRotation();
  mot_dtLeftBack.resetRotation();
  mot_dtRightFront.spinTo(-200, degrees, -50, pct, false);
  mot_dtRightBack.spinTo(-200, degrees, -50, pct, false);
  mot_dtLeftFront.spinTo(-200, degrees, -50, pct,false);
  mot_dtLeftBack.spinTo(-200, degrees, -50, pct, false);
  this_thread::sleep_for(2);

  ring_cup.spinTo(30, degrees, 15, pct, true);



  //reset arm to default before driver mode
  mot_arm.spinTo(40, degrees, 25, pct, true);
  ring_cup.spinTo(30, degrees, 25, pct, true);
  //start driver mode
  allowDriverOperated = true;
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
  mot_dtRightFront.spin(forward, xAxis/speedControls, pct);
  mot_dtRightBack.spin(forward, xAxis/speedControls, pct);
  mot_dtLeftFront.spin(forward, yAxis/speedControls, pct);
  mot_dtLeftBack.spin(forward, yAxis/speedControls, pct);
}
/*
void ScoopDrive(int pos) {
  mot_scoopMotor.spinTo(map(pos, -100, 100, 0, 80), deg, 50, velocityUnits::pct);
}
*/
void eventLoop() {

  bool raising = false;
  cont.Screen.clearScreen();
    ring_cup.resetRotation();
    //mot_arm.spinTo(40, rotationUnits::deg, 25, pct, false);
  while(allowMotion == true && allowDriverOperated) {
    TankDrive(cont.Axis3.position(), cont.Axis2.position());

    if(cont.ButtonX.pressing()){
      if(speedControls ==1){
        speedControls =5;
      }else{
        speedControls = 1;
      }
    }

    //simple claw controls
    if(cont.ButtonA.pressing()){
      mot_frontClaw.spin(forward, 100, pct);
    }else if(cont.ButtonB.pressing()){
      mot_frontClaw.spin(reverse, 100, pct);
    }else{
      mot_frontClaw.spin(forward, 0, pct);
    }
    //grab
    if(cont.ButtonL2.pressing()){
      //spin to 100 degrees at 25% power and don't wait to finish
      ring_cup.spinTo(115, rotationUnits::deg, 15, pct, false );
    }
    //pick up
    else if(cont.ButtonR2.pressing()){
      //spin to 30 degrees at 45% power and don't wait to finish
      ring_cup.spinTo(30, rotationUnits::deg, 25, pct, false);
      mot_arm.spinTo(40, rotationUnits::deg, 25, pct, false);
    }

    if(cont.ButtonL1.pressing()){
      raising = true;
      //ring_cup.spinTo(200, rotationUnits::deg, 5, velocityUnits::pct, false);
    }
    while(raising){
       mot_arm.spinTo(1245, rotationUnits::deg, 75, pct, false);
     // mot_arm.StartspinTo(1270, rotationUnits::deg, 75, velocityUnits::pct, false);
      if(mot_arm.isSpinning()){
        ring_cup.spinTo(ring_cup.rotation(rotationUnits::deg)+10.5, rotationUnits::deg, 75, pct, true);
        cont.Screen.clearScreen();
        cont.Screen.setCursor(0,0);
        cont.Screen.print(ring_cup.rotation(rotationUnits::deg));

      }
      if(!mot_arm.isSpinning()){
        //move ring cup 119.6
        
        raising = false;
      }
       
    }

    if(cont.ButtonR1.pressing()){
      if(!(ring_cup.rotation(rotationUnits::deg) >= 115 && ring_cup.rotation(rotationUnits::deg) <=145)){
      ring_cup.spinTo(140, rotationUnits::deg, 15, pct);
      }else{
        ring_cup.spinTo(260, rotationUnits::deg);
      }
    }

   
    cont.Screen.clearScreen();
    cont.Screen.setCursor(0, 0);
    cont.Screen.print("Ring Cup: ");
    cont.Screen.print( ring_cup.rotation(rotationUnits::deg));
    cont.Screen.setCursor(10, 0);
    cont.Screen.print( " : Mot_arm: ");
    cont.Screen.print(mot_arm.rotation(rotationUnits::deg));

    /*
    cont.Screen.clearScreen();
    cont.Screen.setCursor(0, 0);
    cont.Screen.print(mot_arm.rotation(rotationUnits::deg));
*/
    //simple button controls
    /*if(cont.ButtonL1.pressing()){
      mot_arm.spin(forward, 100, pct);
    }else if(cont.ButtonR1.pressing()){
      mot_arm.spin(reverse, 100, pct);
    }else{
      mot_arm.spin(forward, 0, pct);
    }
    if(cont.ButtonL2.pressing()){
      ring_cup.spin(forward, 10, pct);
    }else if(cont.ButtonR2.pressing()){
      ring_cup.spin(reverse, 10, pct);
    }else{
      ring_cup.spin(forward, 0, pct);
    }
    
    */
  }
}




/////////////////////// MAIN FUNTIONS ////////////////////////////

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  comp.autonomous(autonomous);
  //wait for 45 seconds for auto to complete
  this_thread::sleep_for(45);
  //wait for auto completion
  comp.drivercontrol(beginDriver);
 // TOGGLEINTAKE.pressed(toggleIntake);        // Handle (  X   Button) Toggle Intake Button Pressed   
  // BREAKMODETOGGLE.pressed(toggleBreakMode);  // Handle (  Y   Button) Toggle Break Mode Button Pressed 
  // TURN180DEG.pressed(turn180Deg);            // Handle (  A   Button) Toggle Intake Pressed      
  // SLIDERACTUATE.pressed(doSliderMotion);     // Handle (  B   Button) Toggle Intake Pressed   

  // Begin Program Logic
  cont.Screen.clearScreen();
  cont.Screen.setCursor(0, 0);
  cont.Screen.print("Auto.");
  //begins driving procedure
  eventLoop();
}